#ifndef IDSTORAGE_H
#define IDSTORAGE_H

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
};

#endif
