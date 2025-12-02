#include "Display.h"

#define DISABLED_LCD_RD A0

Display::Display(uint8_t csPin, uint8_t cdPin, uint8_t wrPin, uint8_t resetPin) {
    tft = new Adafruit_TFTLCD(csPin, cdPin, wrPin, DISABLED_LCD_RD, resetPin);
}

void Display::init() {
    // Сброс дисплея
    tft->reset();

    // Инициализация дисплея
    tft->begin(0x9341);
    tft->setRotation(3);
    tft->setTextSize(3);
}

void Display::showWaitingMessage() {
    tft->fillScreen(COLOR_BLUE);
    tft->setTextColor(COLOR_WHITE);
    tft->setCursor(30, 100);
    tft->print(F("Attach a card"));
}

void Display::showWelcomeMessage() {
    tft->fillScreen(COLOR_GREEN);
    tft->setTextColor(COLOR_BLACK);
    tft->setCursor(30, 100);
    tft->print(F("Welcome"));
}

void Display::showGoodbyeMessage() {
    tft->fillScreen(COLOR_GREEN);
    tft->setTextColor(COLOR_BLACK);
    tft->setCursor(30, 100);
    tft->print(F("Goodbye"));
}

void Display::showAccessDeniedMessage() {
    tft->fillScreen(COLOR_RED);
    tft->setTextColor(COLOR_WHITE);
    tft->setCursor(30, 100);
    tft->print(F("Passage is"));
    tft->setCursor(30, 130);
    tft->print(F("prohibited"));
}

void Display::showNoPassageMessage() {
    tft->fillScreen(COLOR_YELLOW);
    tft->setTextColor(COLOR_BLACK);
    tft->setCursor(30, 100);
    tft->print(F("The passage was"));
    tft->setCursor(30, 130);
    tft->print(F("not carried out"));
}
