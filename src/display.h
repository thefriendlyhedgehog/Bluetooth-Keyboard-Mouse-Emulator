
#ifndef DISPLAY_H
#define DISPLAY_H

#include <M5Cardputer.h>

void setupDisplay(bool portraitMode = false);
void displayWelcomeScreen();
void displaySelectionScreen(bool usbMode, int timeout = -1);
void displayMainScreen(bool usbMode, bool mouseMode, bool bluetoothStatus, bool gyroMode, bool portraitMode);
void modeIndicator(bool usbMode, bool bluetoothStatus, bool portraitMode);
void drawDeviceRect(bool mouseMode, bool gyroMode, bool portraitMode);

#endif
