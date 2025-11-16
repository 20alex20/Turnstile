#include "Display.h"

#define LCD_RD A0

Display::Display(uint8_t csPin, uint8_t cdPin, uint8_t wrPin, uint8_t resetPin) {
    tft = new Adafruit_TFTLCD(csPin, cdPin, wrPin, LCD_RD, resetPin);
}

void Display::init() {
    // Сброс дисплея
    tft->reset();
    
    // Инициализация дисплея
    tft->begin(0x9341);
    tft->setRotation(1);
    
    // Очистка экрана голубым цветом
    tft->fillScreen(COLOR_BLUE);
}

void Display::showWaitingMessage() {
    tft->fillScreen(COLOR_BLUE);
    tft->setCursor(40, 100);
    tft->setTextColor(COLOR_WHITE);
    tft->setTextSize(3);
    tft->println("  Приложите");
    tft->setCursor(40, 150);
    tft->println("     карту");
}

void Display::showWelcomeMessage() {
    tft->fillScreen(COLOR_GREEN);
    tft->setCursor(30, 100);
    tft->setTextColor(COLOR_BLACK);
    tft->setTextSize(3);
    tft->println("Добро");
    tft->setCursor(30, 150);
    tft->println("пожаловать");
}

void Display::showGoodbyeMessage() {
    tft->fillScreen(COLOR_GREEN);
    tft->setCursor(30, 100);
    tft->setTextColor(COLOR_BLACK);
    tft->setTextSize(3);
    tft->println("До");
    tft->setCursor(20, 150);
    tft->println("свидания");
}

void Display::showAccessDeniedMessage() {
    tft->fillScreen(COLOR_RED);
    tft->setCursor(20, 100);
    tft->setTextColor(COLOR_WHITE);
    tft->setTextSize(2);
    tft->println("Проход");
    tft->setCursor(30, 130);
    tft->println("запрещен");
}

void Display::showNoPassageMessage() {
    tft->fillScreen(COLOR_YELLOW);
    tft->setCursor(5, 100);
    tft->setTextColor(COLOR_BLACK);
    tft->setTextSize(2);
    tft->println("Проход не");
    tft->setCursor(5, 130);
    tft->println("был осуществлен");
}
