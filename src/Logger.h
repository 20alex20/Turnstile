#ifndef LOGGER_H
#define LOGGER_H

#include <SD.h>
#include "TimeManager.h"

/**
 * Класс для логирования событий
 */
class Logger {
private:
    const char* filename;
    TimeManager* timeManager;
    bool serialLogging;
    
    /**
     * Форматирование ID карты в строку
     */
    String formatCardID(byte* uid, byte uidSize);
    
public:
    /**
     * Конструктор
     * @param filename - имя файла логов
     * @param timeManager - указатель на TimeManager
     */
    Logger(const char* filename, TimeManager* timeManager);
    
    /**
     * Инициализация логгера
     */
    void init();
    
    /**
     * Логирование события
     * @param uid - ID карты
     * @param uidSize - размер ID
     * @param allowed - разрешен ли проход
     * @param passed - прошел ли человек
     */
    void log(byte* uid, byte uidSize, bool allowed, bool passed);
    
    /**
     * Включить/выключить логирование в Serial
     */
    void setSerialLogging(bool enabled);
};

#endif

