#include "display.h"


void drawDeviceRect(bool mouseMode, bool gyroMode, bool portraitMode) {
    int w = M5Cardputer.Display.width();
    int h = M5Cardputer.Display.height();

    if (portraitMode) {
        // Portrait: Stacked layout (Keyboard top, Mouse bottom)
        // panels centered in left 115px (x=8, w=100)
        int pX = 8;
        int pW = 100;
        int pH = (h - 80) / 2 - 5;
        
        // Keyboard Panel (Top)
        M5Cardputer.Display.drawRoundRect(pX, 70, pW, pH, 3, mouseMode ? TFT_WHITE : TFT_GREEN);
        // Mouse Panel (Bottom)
        M5Cardputer.Display.drawRoundRect(pX, 70 + pH + 5, pW, pH, 3, mouseMode ? TFT_GREEN : TFT_WHITE);
        
        if (mouseMode && gyroMode) {
            M5Cardputer.Display.setTextColor(TFT_CYAN);
            M5Cardputer.Display.setTextSize(1);
            M5Cardputer.Display.setCursor(pX + 5, 70 + pH + 5 + pH - 15);
            M5Cardputer.Display.print("GYRO");
        }
    } else {
        // Landscape: Side-by-side layout (Standard)
        M5Cardputer.Display.drawRoundRect(10, 70, w / 2 - 15, h - 80, 3, mouseMode ? TFT_WHITE : TFT_GREEN);
        M5Cardputer.Display.drawRoundRect(w / 2 + 5, 70, w / 2 - 15, h - 80, 3, mouseMode ? TFT_GREEN : TFT_WHITE);
        
        if (mouseMode && gyroMode) {
            M5Cardputer.Display.setTextColor(TFT_CYAN);
            M5Cardputer.Display.setTextSize(1);
            M5Cardputer.Display.setCursor(w / 2 + 12, h - 20);
            M5Cardputer.Display.print("GYRO");
        }
    }
}

void drawMouseIcon(uint8_t x, uint8_t y) {
    uint8_t w = 25;
    uint8_t h = 35;
    M5Cardputer.Display.fillRoundRect(x, y, w, h, 5, TFT_WHITE);
    M5Cardputer.Display.drawLine(x + w / 2, y, x + w / 2, y + h / 2, TFT_BLACK);
}

void drawKeyboardIcon(uint8_t x, uint8_t y) {
    M5Cardputer.Display.fillRect(x, y, 40, 20, TFT_WHITE);
    for (int i=0; i<5; i++) {
        M5Cardputer.Display.fillRect(x + 2 + i*8, y + 2, 6, 6, TFT_BLACK);
        M5Cardputer.Display.fillRect(x + 2 + i*8, y + 10, 6, 6, TFT_BLACK);
    }
    M5Cardputer.Display.drawRect(x, y, 40, 20, TFT_WHITE);
}

void modeIndicator(bool usbMode, bool bluetoothStatus, bool portraitMode) {
    int w = M5Cardputer.Display.width();
    int h = M5Cardputer.Display.height();

    if (portraitMode) {
        // Dual Vertical indicator on the right side
        int barX = 115;
        int barY = 70;
        int barW = 15;
        int barH = h - 80;
        int halfH = barH / 2;
        
        uint16_t usbColor = usbMode ? TFT_GREEN : TFT_DARKGREY;
        uint16_t btColor  = (!usbMode && bluetoothStatus) ? TFT_GREEN : (!usbMode ? TFT_RED : TFT_DARKGREY);

        // USB Zone (Top half)
        M5Cardputer.Display.drawRoundRect(barX, barY, barW, halfH - 2, 3, usbColor);
        M5Cardputer.Display.setTextColor(usbColor);
        M5Cardputer.Display.setTextSize(1);
        const char* uTxt = "USB";
        for(int i=0; i<3; i++) {
            M5Cardputer.Display.setCursor(barX + 4, barY + 10 + i*15);
            M5Cardputer.Display.print(uTxt[i]);
        }

        // BT Zone (Bottom half)
        M5Cardputer.Display.drawRoundRect(barX, barY + halfH + 2, barW, halfH - 2, 3, btColor);
        M5Cardputer.Display.setTextColor(btColor);
        const char* bTxt = "BT";
        for(int i=0; i<2; i++) {
            M5Cardputer.Display.setCursor(barX + 4, barY + halfH + 20 + i*15);
            M5Cardputer.Display.print(bTxt[i]);
        }
    } else {
        uint16_t color = (bluetoothStatus || usbMode) ? TFT_GREEN : TFT_RED;
        int rectW = 104;
        int rectH = 20;
        int x = 10, y = 39;
        M5Cardputer.Display.drawRoundRect(x, y, rectW, rectH, 5, color);
        M5Cardputer.Display.setTextColor(color);
        M5Cardputer.Display.setTextSize(1.6);

        if (usbMode) {
            M5Cardputer.Display.setCursor(x + 40, y + 4);
            M5Cardputer.Display.print("USB");
        } else {
            M5Cardputer.Display.setCursor(x + 13, y + 4);
            M5Cardputer.Display.print("Bluetooth");
        }
    }
}

void setupDisplay(bool portraitMode) {
    M5Cardputer.Display.setRotation(portraitMode ? 0 : 1);
    M5Cardputer.Display.fillScreen(TFT_BLACK);
}

void displayWelcomeScreen() {
    M5Cardputer.Display.setRotation(1); // Force landscape for welcome
    M5Cardputer.Display.fillScreen(TFT_BLACK);
    M5Cardputer.Display.drawRect(9, 47, 220, 40, TFT_LIGHTGRAY);
    M5Cardputer.Display.setTextColor(TFT_LIGHTGRAY);
    M5Cardputer.Display.setCursor(18, 55);
    M5Cardputer.Display.setTextSize(2);
    M5Cardputer.Display.printf("M5 ADV - KB-Mouse"); 

    M5Cardputer.Display.setCursor(60, 120);
    M5Cardputer.Display.setTextSize(1);
    M5Cardputer.Display.printf("v2.11 - ADV Edition");
    delay(2000);
}

void displayMainScreen(bool usbMode, bool mouseMode, bool bluetoothStatus, bool gyroMode, bool portraitMode) {
    M5Cardputer.Display.setRotation(portraitMode ? 0 : 1);
    M5Cardputer.Display.fillScreen(TFT_BLACK);
    
    int w = M5Cardputer.Display.width();
    
    // Header
    M5Cardputer.Display.fillRoundRect(5, 5, w - 10, 25, 5, TFT_LIGHTGREY);
    M5Cardputer.Display.setCursor(portraitMode ? 8 : 19, 10);
    M5Cardputer.Display.setTextSize(portraitMode ? 1.3 : 2); // Tightened for portrait
    M5Cardputer.Display.setTextColor(TFT_BLACK);
    M5Cardputer.Display.print("M5 ADV - KB-Mouse"); 
 
    // Indicators
    modeIndicator(usbMode, bluetoothStatus, portraitMode);
    
    if (!portraitMode) {
        M5Cardputer.Display.setTextColor(TFT_LIGHTGREY);
        M5Cardputer.Display.drawRoundRect(123, 39, 106, 20, 5, TFT_LIGHTGREY);
        M5Cardputer.Display.setCursor(136, 43);
        M5Cardputer.Display.setTextSize(1.6);
        M5Cardputer.Display.print("GO switch");
    }

    drawDeviceRect(mouseMode, gyroMode, portraitMode);
    
    if (portraitMode) {
        // Panels are centered in 115px wide area (x=8, w=100)
        int panelCenterX = 8 + 50; 
        int pH = (M5Cardputer.Display.height() - 80) / 2 - 5;
        int topPanelMidY = 70 + pH / 2;
        int botPanelMidY = 70 + pH + 5 + pH / 2;
        
        drawKeyboardIcon(panelCenterX - 20, topPanelMidY - 10);
        drawMouseIcon(panelCenterX - 12, botPanelMidY - 17);
    } else {
        drawKeyboardIcon(42, 87);
        drawMouseIcon(165, 80);
    }
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