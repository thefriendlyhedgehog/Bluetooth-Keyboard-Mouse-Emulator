#include <M5Cardputer.h>
#include "bluetooth.h"
#include "display.h"
#include "usbHid.h"
#include <USB.h>
#include <Preferences.h>
#include <SD.h>
#include <SPI.h>
#include <FS.h>
#include <esp_bt.h>

Preferences preferences;

bool mouseMode        = false;  // false = keyboard, true = mouse
bool usbMode          = true;
bool gyroMode         = false;
bool portraitMode     = false;
bool lastBluetoothStatus = false;
int  bootTimeout      = 1; // Default 1 second

// -------------------------------------------------------
// Startup: USB or BLE mode selection with Auto-boot
// -------------------------------------------------------
void selectMode() {
    uint32_t startTime = millis();
    int lastRemaining = -1;
    bool interaction = false;
    bool lastUIDrawMode = !usbMode;

    while (true) {
        M5Cardputer.update();
        uint32_t elapsed = millis() - startTime;
        int remaining = bootTimeout - (elapsed / 1000);

        if (!interaction && remaining < 0) break; // Auto-boot!

        // Update UI if mode changed or time remaining changed
        if (lastUIDrawMode != usbMode || (!interaction && lastRemaining != remaining)) {
            displaySelectionScreen(usbMode, interaction ? -1 : remaining);
            lastUIDrawMode = usbMode;
            lastRemaining = remaining;
        }

        if (M5Cardputer.Keyboard.isChange() && M5Cardputer.Keyboard.isPressed()) {
            interaction = true; // Stop countdown on any key press
            Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

            if (M5Cardputer.Keyboard.isKeyPressed('.') || M5Cardputer.Keyboard.isKeyPressed(';')) {
                usbMode = !usbMode;
            }
            if (status.enter) break;
        }
        delay(10);
    }

    // Save preference if it changed
    preferences.begin("settings", false);
    if (preferences.getBool("usbMode", true) != usbMode) {
        preferences.putBool("usbMode", usbMode);
    }
    preferences.end();
}

// -------------------------------------------------------
// Setup
// -------------------------------------------------------
void setup() {
    Serial.begin(115200);
    delay(1000); // Give serial time to attach
    Serial.println("\n\n--- M5 ADV - KB-Mouse v2.11.7 Booting ---");

    auto cfg = M5.config();
    M5Cardputer.begin(cfg, true);

    // Initialise IMU (BMI270 on ADV; gracefully no-ops on original Cardputer)
    M5.Imu.init();

    // 1. Initialize SD Card and load config
    SPIClass* hspi = new SPIClass(HSPI);
    hspi->begin(40, 39, 14, 12); // Cardputer SD pins
    if (SD.begin(12, *hspi, 1000000)) {
        // 1a. Ensure directory exists
        if (!SD.exists("/KBMousePuter")) {
            SD.mkdir("/KBMousePuter");
            Serial.println("SD: Created /KBMousePuter directory");
        }

        // 1b. Load or Create config
        if (!SD.exists("/KBMousePuter/kb_mouse_config.txt")) {
            File configFile = SD.open("/KBMousePuter/kb_mouse_config.txt", FILE_WRITE);
            if (configFile) {
                configFile.println("timeout=1");
                configFile.close();
                Serial.println("SD: Created default kb_mouse_config.txt");
            }
        }

        File configFile = SD.open("/KBMousePuter/kb_mouse_config.txt", FILE_READ);
        if (configFile) {
            while (configFile.available()) {
                String line = configFile.readStringUntil('\n');
                line.trim();
                if (line.startsWith("timeout=")) {
                    bootTimeout = line.substring(8).toInt();
                    if (bootTimeout < 0) bootTimeout = 0;
                }
            }
            configFile.close();
            Serial.printf("SD Config Loaded: timeout=%d\n", bootTimeout);
        }
    } else {
        Serial.println("SD Card not found, using defaults.");
    }

    // 2. Load preferences
    preferences.begin("settings", true);
    usbMode = preferences.getBool("usbMode", true);
    mouseMode = preferences.getBool("mouseMode", false);
    preferences.end();

    Serial.println("System initialized.");

    setupDisplay(portraitMode);
    displayWelcomeScreen();

    Serial.println("Selecting mode...");
    selectMode();
    Serial.printf("Mode selected: %s\n", usbMode ? "USB" : "Bluetooth");

    if (usbMode) {
        Serial.println("Starting USB...");
        usbMouseInit();
        usbKeyboardInit();
        USB.begin();
        Serial.println("USB Started.");
    } else {
        Serial.println("Starting Bluetooth...");
        initBluetooth();
        Serial.println("Bluetooth Started.");
    }

    displayMainScreen(usbMode, mouseMode, getBluetoothStatus(), gyroMode, portraitMode);
    Serial.println("Setup complete, entering loop.");
}

// -------------------------------------------------------
// Main loop
// -------------------------------------------------------
void loop() {
    M5Cardputer.update();

    // BT connection status change → refresh indicator
    auto bluetoothStatus = getBluetoothStatus();
    if (lastBluetoothStatus != bluetoothStatus) {
        modeIndicator(usbMode, bluetoothStatus, portraitMode);
        lastBluetoothStatus = bluetoothStatus;
    }

    // GO button: short press = toggle keyboard/mouse
    //            long press (>500 ms) while in mouse mode = toggle gyro
    if (M5Cardputer.BtnA.pressedFor(500)) {
        // G0 Long Press: USB/Bluetooth Mode Swap
        usbMode = !usbMode;
        Serial.printf("G0 Long Press: Swapping mode to %s\n", usbMode ? "USB" : "Bluetooth");
        
        if (usbMode) {
            deinitBluetooth();
            usbMouseInit();
            usbKeyboardInit();
            USB.begin();
        } else {
            deinitUsb();
            initBluetooth();
        }
        
        // Save preference
        preferences.begin("settings", false);
        preferences.putBool("usbMode", usbMode);
        preferences.end();
        
        displayMainScreen(usbMode, mouseMode, getBluetoothStatus(), gyroMode, portraitMode);
        
        // Wait for release to prevent multiple toggles
        while (M5Cardputer.BtnA.isPressed()) {
            M5Cardputer.update();
            delay(10);
        }
    }
    else if (M5Cardputer.BtnA.wasPressed()) {
        // Short press → toggle keyboard / mouse
        mouseMode = !mouseMode;
        if (!mouseMode) gyroMode = false;  // reset gyro when switching to keyboard

        // Persist mouseMode state
        preferences.begin("settings", false);
        preferences.putBool("mouseMode", mouseMode);
        preferences.end();
        
        // Redraw device panel
        drawDeviceRect(mouseMode, gyroMode, portraitMode);
        delay(50);
    }

    // --- Centralized Keyboard Event Handler ---
    bool changed = M5Cardputer.Keyboard.isChange();
    if (changed && M5Cardputer.Keyboard.isPressed()) {
        // Configuration Toggles via Ctrl (Any Mode)
        bool ctrlPressed = M5Cardputer.Keyboard.isKeyPressed(KEY_LEFT_CTRL) || M5Cardputer.Keyboard.isKeyPressed(KEY_RIGHT_CTRL);
        if (ctrlPressed) {
            bool settingsChanged = false;
            if (M5Cardputer.Keyboard.isKeyPressed('p')) {
                portraitMode = !portraitMode;
                settingsChanged = true;
            } else if (M5Cardputer.Keyboard.isKeyPressed('g')) {
                gyroMode = !gyroMode;
                settingsChanged = true;
            }

            if (settingsChanged) {
                preferences.begin("settings", false);
                preferences.putBool("portraitMode", portraitMode);
                preferences.putBool("gyroMode", gyroMode);
                preferences.end();
                displayMainScreen(usbMode, mouseMode, getBluetoothStatus(), gyroMode, portraitMode);
                delay(200); // Debounce
            }
        }
    }

    if (usbMode) {
        handleUsbMode(mouseMode, gyroMode, portraitMode, changed);
    } else {
        handleBluetoothMode(mouseMode, gyroMode, portraitMode, changed);
    }

    // REMOVED redundant M5Cardputer.update() to fix button event clearing
}
