#ifndef RFIDREADER_H
#define RFIDREADER_H

#include <MFRC522.h>

/**
 * Класс для управления одним RFID модулем RC522
 */
class RFIDReader {
private:
    MFRC522* reader;

    /**
     * Остановка чтения карты
     */
    void stopReading();
    
public:
    /**
     * Конструктор
     * @param ssPin - пин Slave Select
     * @param rstPin - пин Reset
     */
    RFIDReader(uint8_t ssPin, uint8_t rstPin);
    
    /**
     * Инициализация модуля
     */
    void init();
    
    /**
     * Проверка наличия новой карты
     * @return true, если обнаружена карта
     */
    bool isCardPresent();
    
    /**
     * Чтение ID карты
     * @param uid - массив для записи UID (до 10 байт)
     * @param uidSize - размер массива uid
     * @return true, если ID успешно прочитан, и размер UID в параметре uidSize
     */
    bool readCardID(byte* uid, byte& uidSize);
};

#endif
