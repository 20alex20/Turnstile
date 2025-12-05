#ifndef IDSTORAGE_H
#define IDSTORAGE_H

#include <Arduino.h>
#include "Logger.h"

/**
 * Класс для работы с файлом разрешенных ID карт на SD карте
 */
class IDStorage {
private:
    char* filename;
    uint8_t ssPin;

    /**
     * Сравнение двух ID
     */
    bool compareIDs(const byte* id1, const byte* id2, byte size);

public:
    /**
     * Конструктор
     * @param filename - имя файла с ID (например, "ids.data")
     */
    IDStorage(const char* filename, uint8_t ssPin);

    /**
     * Инициализация SD карты
     * @return true, если успешно
     */
    bool init();

    /**
     * Проверка наличия ID в файле
     * @param uid - массив с UID
     * @param uidSize - размер UID
     * @return true, если ID найден в файле
     */
    bool isIDAllowed(byte* uid, byte uidSize);

    /**
     * Добавить ID в файл
     * @param uid - массив с UID
     * @param uidSize - размер UID
     * @return true, если успешно добавлен
     */
    bool addID(byte* uid, byte uidSize);

    /**
     * Удалить ID из файла
     * @param uid - массив с UID
     * @param size - размер UID
     * @return true, если успешно удален
    */
    bool removeID(byte* uid, byte size);

    /**
     * Вывести все ID из файла
     */
    void printAllIDs();

    /**
     * Конвертировать строку в ID
     * @param str - строка с ID
     * @param uid - массив с UID
     * @param uidSize - размер UID
     * @return true, если успешно декодирован
     */
    static bool parseCardID(const char* str, byte* uid, byte& size);

    /**
     * @param cmd - строка с командой
     * @param s - объект IDStorage
     * @param l - объект Logger
     * Выполнить команду
     */
    static void handleCommand(const char* cmd, IDStorage* s, Logger* l);
};

#endif
