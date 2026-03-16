
#include "bluetooth.h"

BLEHIDDevice* hid;
BLECharacteristic* mouseInput;
BLECharacteristic* keyboardInput;
bool bluetoothIsConnected = false;

void MyBLEServerCallbacks::onConnect(BLEServer* pServer) {
    bluetoothIsConnected = true;
}

void MyBLEServerCallbacks::onDisconnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) {
    bluetoothIsConnected = false;
    pServer->disconnect(param->disconnect.conn_id);
    pServer->startAdvertising();
}

bool getBluetoothStatus() {
    return bluetoothIsConnected;
}

// -------------------------------------------------------
static float gyroOffsetX = 0, gyroOffsetY = 0;

// -------------------------------------------------------
// F-key substitution: when Fn held, remap number row HID
// codes to F1–F12 (0x3A–0x45).
// -------------------------------------------------------
static uint8_t fnKeySubstitute(uint8_t hidCode) {
    // F-Keys mapping
    static const uint8_t numberHID[] = {0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x2D,0x2E};
    static const uint8_t fKeys[]     = {0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,0x40,0x41,0x42,0x43,0x44,0x45};
    for (uint8_t i = 0; i < 12; i++) {
        if (hidCode == numberHID[i]) return fKeys[i];
    }
    
    // Physical Case Indicators mapping
    switch (hidCode) {
        case 0x35: return 0x29; // Fn + ` -> Escape
        case 0x33: return 0x52; // Fn + ; -> Up Arrow
        case 0x37: return 0x51; // Fn + . -> Down Arrow
        case 0x36: return 0x50; // Fn + , -> Left Arrow
        case 0x38: return 0x4F; // Fn + / -> Right Arrow
        default:   return hidCode;
    }
}

// -------------------------------------------------------
// BLE Mouse tick
// -------------------------------------------------------
void bluetoothMouse(bool gyroMode, bool portraitMode) {
    if (!bluetoothIsConnected) return;

    int8_t moveX = 0, moveY = 0, wheel = 0, pan = 0;
    uint8_t buttons = 0;

    Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

    if (gyroMode) {
        float ax, ay, az;
        M5.Imu.getAccel(&ax, &ay, &az);

        float dx = ax - gyroOffsetX;
        float dy = ay - gyroOffsetY;

        if (abs(dx) < GYRO_DEADZONE) dx = 0;
        if (abs(dy) < GYRO_DEADZONE) dy = 0;

        if (portraitMode) {
            // Portrait: X controlled by -dy, Y controlled by dx
            moveX = (int8_t)(-dy * GYRO_SCALE);
            moveY = (int8_t)(dx * GYRO_SCALE);
        } else {
            // Landscape: X controlled by -dx, Y controlled by -dy
            moveX = (int8_t)(-dx * GYRO_SCALE);
            moveY = (int8_t)(-dy * GYRO_SCALE);
        }
    } else if (status.fn) {
        // --- Fn held: arrow-related keys = scroll / pan (Bluetooth HID style) ---
        if (M5Cardputer.Keyboard.isKeyPressed(';')) wheel =  1;  // scroll up
        if (M5Cardputer.Keyboard.isKeyPressed('.')) wheel = -1;  // scroll down
        if (M5Cardputer.Keyboard.isKeyPressed('/'))  pan   =  SCROLL_SPEED;  // pan right
        if (M5Cardputer.Keyboard.isKeyPressed(','))  pan   = -SCROLL_SPEED;  // pan left
    } else {
        // --- Normal arrow-key mouse movement ---
        if (M5Cardputer.Keyboard.isKeyPressed('/'))  moveX =  MOUSE_SPEED;
        if (M5Cardputer.Keyboard.isKeyPressed(','))  moveX = -MOUSE_SPEED;
        if (M5Cardputer.Keyboard.isKeyPressed(';'))  moveY = -MOUSE_SPEED;
        if (M5Cardputer.Keyboard.isKeyPressed('.'))  moveY =  MOUSE_SPEED;
    }

// Clicks: default or portrait remapping
    bool leftClick, rightClick, recenter;
    if (portraitMode) {
        leftClick  = M5Cardputer.Keyboard.isKeyPressed('q');
        rightClick = M5Cardputer.Keyboard.isKeyPressed('1');
        recenter   = M5Cardputer.Keyboard.isKeyPressed(0x80); // KEY_LEFT_CTRL
    } else {
        leftClick  = M5Cardputer.Keyboard.isKeyPressed('`');
        rightClick = status.del; // Backspace
        recenter   = status.enter;
    }

    if (recenter && gyroMode) {
        float ax, ay, az;
        M5.Imu.getAccel(&ax, &ay, &az);
        gyroOffsetX = ax; gyroOffsetY = ay;
    }

    if (leftClick)  buttons |= 0x01;
    if (rightClick) buttons |= 0x02;
    
    // Report: buttons, X, Y, Wheel, Pan  (5 bytes, report ID 1)
    uint8_t report[5] = {buttons, (uint8_t)moveX, (uint8_t)moveY, (uint8_t)wheel, (uint8_t)pan};
    mouseInput->setValue(report, sizeof(report));
    mouseInput->notify();
}

// -------------------------------------------------------
// BLE Keyboard tick
// -------------------------------------------------------
void bluetoothKeyboard(bool changed) {
    Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

    bool anyKey = !status.hid_keys.empty() ||
                  M5Cardputer.Keyboard.isKeyPressed(' ') ||
                  status.modifiers != 0 ||
                  status.opt;

    // Only send a report if something changed or keys are still held
    if (!changed && !anyKey) return;

    uint8_t modifiers = status.modifiers;
    if (status.opt) modifiers |= (1 << 3); // Opt → GUI

    uint8_t idx = 0;
    uint8_t keys[6] = {0};

    for (auto k : status.hid_keys) {
        if (idx >= 6) break;
        uint8_t key = status.fn ? fnKeySubstitute(k) : k;
        keys[idx++] = key;
    }

    if (M5Cardputer.Keyboard.isKeyPressed(' ') && idx < 6) {
        uint8_t HID_SPACE = 0x2C;
        bool present = false;
        for (int i = 0; i < idx; i++) if (keys[i] == HID_SPACE) { present = true; break; }
        if (!present) keys[idx++] = HID_SPACE;
    }

    uint8_t report[8] = {modifiers, 0, keys[0], keys[1], keys[2], keys[3], keys[4], keys[5]};
    keyboardInput->setValue(report, sizeof(report));
    keyboardInput->notify();

    // If nothing is held, immediately send a zero report (key-up)
    if (!anyKey) {
        uint8_t emptyReport[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        keyboardInput->setValue(emptyReport, sizeof(emptyReport));
        keyboardInput->notify();
    }

    delay(20);
}

// -------------------------------------------------------
// Send zeroed reports (key-up / mouse-stopped)
// -------------------------------------------------------
void sendEmptyReports() {
    uint8_t emptyMouse[5]    = {0, 0, 0, 0, 0};
    uint8_t emptyKeyboard[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    mouseInput->setValue(emptyMouse, sizeof(emptyMouse));
    mouseInput->notify();
    keyboardInput->setValue(emptyKeyboard, sizeof(emptyKeyboard));
    keyboardInput->notify();
}

// -------------------------------------------------------
// Mode dispatcher
// -------------------------------------------------------
void handleBluetoothMode(bool mouseMode, bool gyroMode, bool portraitMode, bool changed) {
    if (bluetoothIsConnected) {
        bool pressed = M5Cardputer.Keyboard.isPressed();
        // In gyro mode we always need to send mouse reports (device may be tilted)
        if (mouseMode && (gyroMode || pressed)) {
            bluetoothMouse(gyroMode, portraitMode);
        } else if (!mouseMode && (changed || pressed)) {
            bluetoothKeyboard(changed);
        } else {
            sendEmptyReports();
        }
    }
    delay(7);
}

// -------------------------------------------------------
// BLE init
// -------------------------------------------------------
void initBluetooth() {
    BLEDevice::init("M5-Keyboard-Mouse");
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyBLEServerCallbacks());

    hid = new BLEHIDDevice(pServer);
    mouseInput    = hid->inputReport(1);
    keyboardInput = hid->inputReport(2);

    hid->manufacturer()->setValue("M5Stack");
    hid->pnp(0x02, 0x1234, 0x5678, 0x0100);
    hid->hidInfo(0x00, 0x01);
    hid->reportMap((uint8_t*)HID_REPORT_MAP, sizeof(HID_REPORT_MAP));
    hid->startServices();

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->setAppearance(HID_MOUSE);
    pAdvertising->addServiceUUID(hid->hidService()->getUUID());
    pAdvertising->start();

    BLESecurity *pSecurity = new BLESecurity();
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);
    pSecurity->setCapability(ESP_IO_CAP_NONE);
    pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
}

void deinitBluetooth() {
    BLEDevice::deinit();
    delay(1000);
}
