#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include "TimeManager.h"

/**
 * Класс для логирования событий
 */
class Logger {
private:
    TimeManager* timeManager;
    bool serialLogging;
    
    /**
     * Форматирование ID карты в строку
     */
    String formatCardID(byte* uid, byte uidSize);
    
public:
    const char* filename;

    /**
     * Конструктор
     * @param filename - имя файла логов
     * @param timeManager - указатель на TimeManager
     */
    Logger(const char* filename, TimeManager* timeManager);
    
    /**
     * Логирование события
     * @param uid - ID карты
     * @param uidSize - размер ID
     * @param allowed - разрешен ли проход
     * @param direcyionEntry - направление (вход/выход)
     * @param passed - прошел ли человек
     */
    void log(byte* uid, byte uidSize, bool allowed, bool direcyionEntry, bool passed);
    
    /**
     * Включить/выключить логирование в Serial
     */
    void setSerialLogging(bool enabled);
};

#endif
