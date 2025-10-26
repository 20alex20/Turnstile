#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <SD.h>
#include "IDStorage.h"
#include "Logger.h"

/**
 * Класс для обработки Serial команд
 */
class CommandHandler {
private:
    IDStorage* idStorage;
    Logger* logger;
    
    /**
     * Парсинг ID карты из строки
     * @param str - строка с ID в формате "AA BB CC DD"
     * @param uid - массив для записи UID
     * @param uidSize - размер UID
     * @return true, если успешно распарсен
     */
    bool parseCardID(const String& str, byte* uid, byte& uidSize);
    
    /**
     * Форматирование UID в строку для команд
     */
    String formatUIDForCommand(byte* uid, byte uidSize);
    
public:
    /**
     * Конструктор
     */
    CommandHandler(IDStorage* idStorage, Logger* logger);
    
    /**
     * Обработка входящих команд
     */
    void handleCommand(const String& command);
};

#endif

