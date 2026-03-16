#ifndef USBHID_H
#define USBHID_H

#include "USBHIDMouse.h"
#include "USBHIDKeyboard.h"
#include <M5Cardputer.h>

// Mouse movement speed in arrow-key mode (pixels per tick)
#define MOUSE_SPEED 5
// Scroll wheel speed for Fn+arrow mode
#define SCROLL_SPEED 1
// Gyro sensitivity scale
#define GYRO_SCALE 30.0f
// Gyro deadzone to prevent jitter
#define GYRO_DEADZONE 0.05f

void usbMouseInit();
void usbMouse(bool gyroMode);
void usbKeyboardInit();
void usbKeyboard(bool changed);
void handleUsbMode(bool mouseMode, bool gyroMode, bool portraitMode, bool changed);

#endif