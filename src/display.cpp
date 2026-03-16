#include "display.h"


void drawDeviceRect(bool mouseMode, bool gyroMode, bool portraitMode) {
    if (mouseMode) {
        // Mouse panel active (green), Keyboard panel white
        M5Cardputer.Display.drawRoundRect(10, 70, M5Cardputer.Display.width() / 2 - 15, M5Cardputer.Display.height() - 80, 3, TFT_WHITE);
        M5Cardputer.Display.drawRoundRect(M5Cardputer.Display.width() / 2 + 5, 70, M5Cardputer.Display.width() / 2 - 15, M5Cardputer.Display.height() - 80, 3, TFT_GREEN);
        
        // Gyro indicator area
        M5Cardputer.Display.setTextSize(1);
        int gyroX = M5Cardputer.Display.width() / 2 + 12;
        int gyroY = M5Cardputer.Display.height() - 20;

        if (gyroMode) {
            M5Cardputer.Display.setTextColor(TFT_CYAN);
            M5Cardputer.Display.setCursor(gyroX, gyroY);
            M5Cardputer.Display.print("GYRO");
            
            if (portraitMode) {
                M5Cardputer.Display.setTextColor(TFT_YELLOW);
                M5Cardputer.Display.setCursor(gyroX, gyroY - 12);
                M5Cardputer.Display.print("PORT");
            } else {
                // Clear PORT area if not in portrait
                M5Cardputer.Display.fillRect(gyroX, gyroY - 12, 30, 10, TFT_BLACK);
            }
        } else {
            // Clear the area and redraw border to fix clipping
            M5Cardputer.Display.fillRect(M5Cardputer.Display.width() / 2 + 10, M5Cardputer.Display.height() - 25, 30, 20, TFT_BLACK);
            M5Cardputer.Display.drawRoundRect(M5Cardputer.Display.width() / 2 + 5, 70, M5Cardputer.Display.width() / 2 - 15, M5Cardputer.Display.height() - 80, 3, TFT_GREEN);
        }
    } else {
        // Keyboard panel active (green), Mouse panel white
        M5Cardputer.Display.drawRoundRect(10, 70, M5Cardputer.Display.width() / 2 - 15, M5Cardputer.Display.height() - 80, 3, TFT_GREEN);
        M5Cardputer.Display.drawRoundRect(M5Cardputer.Display.width() / 2 + 5, 70, M5Cardputer.Display.width() / 2 - 15, M5Cardputer.Display.height() - 80, 3, TFT_WHITE);
        // Clear gyro/port text and redraw white border
        M5Cardputer.Display.fillRect(M5Cardputer.Display.width() / 2 + 10, M5Cardputer.Display.height() - 25, 30, 20, TFT_BLACK);
        M5Cardputer.Display.drawRoundRect(M5Cardputer.Display.width() / 2 + 5, 70, M5Cardputer.Display.width() / 2 - 15, M5Cardputer.Display.height() - 80, 3, TFT_WHITE);
    }
}

void drawMouseIcon(uint8_t x, uint8_t y) {
    uint8_t w = 25;
    uint8_t h = 35;

    M5Cardputer.Display.fillRoundRect(x, y, 25, 35, 5, TFT_WHITE);
    M5Cardputer.Display.drawLine(x + w / 2, y, x + w / 2, y + h / 2, TFT_BLACK);
}

void drawKeyboardIcon(uint8_t x, uint8_t y) {
    M5Cardputer.Display.fillRect(x, y, 40, 20, TFT_WHITE);

    M5Cardputer.Display.fillRect(x + 2,  y + 2,  6, 6, TFT_BLACK);
    M5Cardputer.Display.fillRect(x + 10, y + 2,  6, 6, TFT_BLACK);
    M5Cardputer.Display.fillRect(x + 18, y + 2,  6, 6, TFT_BLACK);
    M5Cardputer.Display.fillRect(x + 26, y + 2,  6, 6, TFT_BLACK);
    M5Cardputer.Display.fillRect(x + 34, y + 2,  6, 6, TFT_BLACK);

    M5Cardputer.Display.fillRect(x + 2,  y + 10, 6, 6, TFT_BLACK);
    M5Cardputer.Display.fillRect(x + 10, y + 10, 6, 6, TFT_BLACK);
    M5Cardputer.Display.fillRect(x + 18, y + 10, 6, 6, TFT_BLACK);
    M5Cardputer.Display.fillRect(x + 26, y + 10, 6, 6, TFT_BLACK);
    M5Cardputer.Display.fillRect(x + 34, y + 10, 6, 6, TFT_BLACK);

    M5Cardputer.Display.drawLine(x,      y,      x,      y + 20, TFT_WHITE);
    M5Cardputer.Display.drawLine(x + 40, y,      x + 40, y + 20, TFT_WHITE);
    M5Cardputer.Display.drawLine(x,      y,      x + 40, y,      TFT_WHITE);
    M5Cardputer.Display.drawLine(x,      y + 20, x + 40, y + 20, TFT_WHITE);
}

void modeIndicator(bool usbMode, bool bluetoothStatus) {
    M5Cardputer.Display.setTextSize(1.6);

    if (bluetoothStatus || usbMode) {
        M5Cardputer.Display.drawRoundRect(10, 39, 104, 20, 5, TFT_GREEN);
        M5Cardputer.Display.setTextColor(TFT_GREEN);
    } else {
        M5Cardputer.Display.drawRoundRect(10, 39, 104, 20, 5, TFT_RED);
        M5Cardputer.Display.setTextColor(TFT_RED);
    }

    if (usbMode) {
        M5Cardputer.Display.setCursor(50, 43);
        M5Cardputer.Display.print("USB");
    } else {
        M5Cardputer.Display.setCursor(23, 43);
        M5Cardputer.Display.print("Bluetooth");
    }
}

void setupDisplay() {
    M5Cardputer.Display.setRotation(1);
    M5Cardputer.Display.fillScreen(TFT_BLACK);
    M5Cardputer.Display.setTextColor(TFT_BLACK);
}

void displayWelcomeScreen() {
    M5Cardputer.Display.drawRect(9, 47, 220, 40, TFT_LIGHTGRAY);
    M5Cardputer.Display.setTextColor(TFT_LIGHTGRAY);
    M5Cardputer.Display.setCursor(18, 58);
    M5Cardputer.Display.setTextSize(2);
    M5Cardputer.Display.printf("M5-Keyboard-Mouse");

    M5Cardputer.Display.setCursor(65, 120);
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.printf("v2.9.5 - ADV Edition");

    delay(2000);
}

void displayMainScreen(bool usbMode, bool mouseMode, bool bluetoothStatus, bool gyroMode, bool portraitMode) {
    M5Cardputer.Display.fillScreen(TFT_BLACK);
    M5Cardputer.Display.fillRoundRect(10, 10, M5Cardputer.Display.width() - 20, 20, 5, TFT_LIGHTGREY);
    M5Cardputer.Display.setCursor(19, 13);
    M5Cardputer.Display.setTextSize(2);
    M5Cardputer.Display.setTextColor(TFT_BLACK);
    M5Cardputer.Display.print("M5-Keyboard-Mouse");
 
    // GO switch label
    M5Cardputer.Display.setTextColor(TFT_LIGHTGREY);
    M5Cardputer.Display.drawRoundRect(123, 39, 106, 20, 5, TFT_LIGHTGREY);
    M5Cardputer.Display.setCursor(136, 43);
    M5Cardputer.Display.setTextSize(1.6);
    M5Cardputer.Display.print("GO switch");
 
    // --- UI indicators for mode state ---
    drawDeviceRect(mouseMode, gyroMode, portraitMode);
    drawMouseIcon(165, 80);
    drawKeyboardIcon(42, 87);
    modeIndicator(usbMode, bluetoothStatus);
}

void displaySelectionScreen(bool mode, int timeout) {
    M5Cardputer.Display.fillScreen(TFT_BLACK);
    M5Cardputer.Display.setTextSize(3);
 
    // USB
    if (mode) {
        M5Cardputer.Display.fillRect(20, 30, 200, 40, TFT_LIGHTGRAY);
        M5Cardputer.Display.drawRect(20, 30, 200, 40, TFT_BLACK);
        M5Cardputer.Display.setTextColor(TFT_BLACK);
    } else {
        M5Cardputer.Display.fillRect(20, 30, 200, 40, TFT_BLACK);
        M5Cardputer.Display.drawRect(20, 30, 200, 40, TFT_LIGHTGRAY);
        M5Cardputer.Display.setTextColor(TFT_LIGHTGRAY);
    }
    M5Cardputer.Display.setCursor(95, 40);
    M5Cardputer.Display.printf("USB");
 
    // Bluetooth
    if (!mode) {
        M5Cardputer.Display.fillRect(20, 80, 200, 40, TFT_LIGHTGRAY);
        M5Cardputer.Display.drawRect(20, 80, 200, 40, TFT_BLACK);
        M5Cardputer.Display.setTextColor(TFT_BLACK);
    } else {
        M5Cardputer.Display.fillRect(20, 80, 200, 40, TFT_BLACK);
        M5Cardputer.Display.drawRect(20, 80, 200, 40, TFT_LIGHTGRAY);
        M5Cardputer.Display.setTextColor(TFT_LIGHTGRAY);
    }
    M5Cardputer.Display.setCursor(42, 90);
    M5Cardputer.Display.printf("Bluetooth");

    if (timeout >= 0) {
        M5Cardputer.Display.setTextSize(1);
        M5Cardputer.Display.setTextColor(TFT_YELLOW);
        M5Cardputer.Display.setCursor(65, 125);
        M5Cardputer.Display.printf("Auto-booting in %ds...", timeout);
    }
}