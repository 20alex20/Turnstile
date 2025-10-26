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
     * @param uidSize - размер UID
     * @return true, если успешно удален
     */
    bool removeID(byte* uid, byte uidSize);
    
    /**
     * Получить все ID из файла
     * @return строка со всеми ID
     */
    String getAllIDs();
    
    /**
     * Очистить файл ids.data
     */
    void clearAll();
};

#endif
