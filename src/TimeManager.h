#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <Arduino.h>

/**
 * Структура для хранения даты и времени
 */
struct DateTime {
    uint8_t day;
    uint8_t month;
    uint16_t year;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

/**
 * Класс для управления временем
 */
class TimeManager {
private:
    DateTime startTime;
    unsigned long startMillis;
    
public:
    /**
     * Конструктор
     */
    TimeManager();
    
    /**
     * Запрос даты и времени при запуске
     */
    void requestDateTime();
    
    /**
     * Установить дату и время
     */
    void setDateTime(uint8_t day, uint8_t month, uint16_t year, 
                     uint8_t hour, uint8_t minute, uint8_t second);
    
    /**
     * Получить текущую дату и время
     */
    void getCurrentDateTime(DateTime& dt);
    
    /**
     * Преобразовать дату и время в строку формата "dd:MM:yyyy hh:mm:ss"
     */
    String formatDateTime(const DateTime& dt);
};

#endif

