#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_TFTLCD.h>

/**
 * Класс для управления дисплеем TFT
 */
class Display {
private:
    Adafruit_TFTLCD* tft;
    
    // Цвета
    static const uint16_t COLOR_BLUE = 0x001F;
    static const uint16_t COLOR_GREEN = 0x07E0;
    static const uint16_t COLOR_RED = 0xF800;
    static const uint16_t COLOR_YELLOW = 0xFFE0;
    static const uint16_t COLOR_BLACK = 0x0000;
    static const uint16_t COLOR_WHITE = 0xFFFF;
    
public:
    /**
     * Конструктор
     * @param csPin - пин Chip Select
     * @param cdPin - пин Command/Data
     * @param wrPin - пин Write
     * @param resetPin - пин Reset
     */
    Display(uint8_t csPin, uint8_t cdPin, uint8_t wrPin, uint8_t resetPin);
    
    /**
     * Инициализация дисплея
     */
    void init();
    
    /**
     * Показать сообщение "Приложите карту" на голубом фоне
     */
    void showWaitingMessage();
    
    /**
     * Показать сообщение "Добро пожаловать" на зеленом фоне
     */
    void showWelcomeMessage();
    
    /**
     * Показать сообщение "До свидания" на зеленом фоне
     */
    void showGoodbyeMessage();
    
    /**
     * Показать сообщение "Проход запрещен" на красном фоне
     */
    void showAccessDeniedMessage();
    
    /**
     * Показать сообщение "Проход не был осуществлен" на желтом фоне
     */
    void showNoPassageMessage();
};

#endif
