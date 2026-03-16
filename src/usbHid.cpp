#include "usbHid.h"

USBHIDMouse mouse;
USBHIDKeyboard keyboard;
static float gyroOffsetX = 0, gyroOffsetY = 0;

#include <cmath>
// -------------------------------------------------------
// F-key substitution: Fn + number row  →  F1-F12
// Returns the F-key HID code, or the original code unchanged.
// -------------------------------------------------------
static const uint8_t FN_MAP[][2] = {
    {'1', 0x3A},  // F1
    {'2', 0x3B},  // F2
    {'3', 0x3C},  // F3
    {'4', 0x3D},  // F4
    {'5', 0x3E},  // F5
    {'6', 0x3F},  // F6
    {'7', 0x40},  // F7
    {'8', 0x41},  // F8
    {'9', 0x42},  // F9
    {'0', 0x43},  // F10
    {'-', 0x44},  // F11
    {'=', 0x45},  // F12
};

static uint8_t fnKeySubstitute(uint8_t hidCode) {
    // The hid_keys array already contains HID usage IDs.
    // Number row HID codes: 1=0x1E, 2=0x1F, ... 0=0x27, -=0x2D, ==0x2E
    static const uint8_t numberHID[] = {0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x2D,0x2E};
    static const uint8_t fKeys[]     = {0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,0x40,0x41,0x42,0x43,0x44,0x45};
    for (uint8_t i = 0; i < 12; i++) {
        if (hidCode == numberHID[i]) return fKeys[i];
    }
    return hidCode; // no substitution
}

// -------------------------------------------------------
// USB Mouse init (call once before USB.begin loop)
// -------------------------------------------------------
void usbMouseInit() {
    mouse.begin();
}

// -------------------------------------------------------
// USB Mouse tick
// -------------------------------------------------------
void usbMouse(bool gyroMode, bool portraitMode) {
    int8_t moveX = 0, moveY = 0, wheel = 0, pan = 0;

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
        // --- Fn held: arrow cluster = scroll / pan ---
        if (M5Cardputer.Keyboard.isKeyPressed(';'))  wheel =  SCROLL_SPEED;
        if (M5Cardputer.Keyboard.isKeyPressed('.'))  wheel = -SCROLL_SPEED;
        if (M5Cardputer.Keyboard.isKeyPressed('/'))  pan    =  SCROLL_SPEED;
        if (M5Cardputer.Keyboard.isKeyPressed(','))  pan    = -SCROLL_SPEED;
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

    if (leftClick) {
        mouse.press(MOUSE_BUTTON_LEFT);
    } else {
        mouse.release(MOUSE_BUTTON_LEFT);
    }
    if (rightClick) {
        mouse.press(MOUSE_BUTTON_RIGHT);
    } else {
        mouse.release(MOUSE_BUTTON_RIGHT);
    }

    mouse.move(moveX, moveY, wheel, pan);
}

// -------------------------------------------------------
// USB Keyboard Init
// -------------------------------------------------------
void usbKeyboardInit() {
    keyboard.begin();
}

// -------------------------------------------------------
// USB Keyboard tick
// -------------------------------------------------------
void usbKeyboard() {
    if (!M5Cardputer.Keyboard.isChange()) return;

    Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();

    KeyReport report = {0};
    report.modifiers = status.modifiers;

    // Map Opt key to GUI (Command/Windows)
    if (status.opt) report.modifiers |= (1 << 3);

    uint8_t idx = 0;
    for (auto k : status.hid_keys) {
        if (idx >= 6) break;
        // Substitute F-keys when Fn is held
        uint8_t key = status.fn ? fnKeySubstitute(k) : k;
        report.keys[idx++] = key;
    }

    // Space key (not always in hid_keys)
    if (M5Cardputer.Keyboard.isKeyPressed(' ')) {
        const uint8_t HID_SPACE = 0x2C;
        bool present = false;
        for (uint8_t i = 0; i < idx; ++i) if (report.keys[i] == HID_SPACE) { present = true; break; }
        if (!present && idx < 6) report.keys[idx++] = HID_SPACE;
    }

    if (idx == 0 && report.modifiers == 0) {
        keyboard.releaseAll();
    } else {
        keyboard.sendReport(&report);
    }
}

// -------------------------------------------------------
// Mode dispatcher
// -------------------------------------------------------
void handleUsbMode(bool mouseMode, bool gyroMode, bool portraitMode) {
    if (mouseMode) {
        usbMouse(gyroMode, portraitMode);
    } else {
        usbKeyboard();
    }
    delay(5);
}