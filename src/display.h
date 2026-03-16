
#ifndef DISPLAY_H
#define DISPLAY_H

#include <M5Cardputer.h>

void setupDisplay();
void displayWelcomeScreen();
void displaySelectionScreen(bool mouseMode, int timeout = -1);
void displayMainScreen(bool usbMode, bool mouseMode, bool bluetoothStatus, bool gyroMode, bool portraitMode);
void modeIndicator(bool usbMode, bool bluetoothStatus);
void drawDeviceRect(bool reverse, bool gyroMode, bool portraitMode);

#endif
