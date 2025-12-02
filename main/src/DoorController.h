#ifndef DOORCONTROLLER_H
#define DOORCONTROLLER_H

#include <Arduino.h>

/**
 * Класс для управления дверью турникета (сервоприводом)
 */
class DoorController {
private:
    static const uint8_t POS_CLOSED = 75;      // Закрыто
    static const uint8_t POS_OPENED_IN = 10;    // Открыто для входа
    static const uint8_t POS_OPENED_OUT = 160; // Открыто для выхода

    void write(uint8_t angle);

public:
    /**
     * Конструктор
     * @param pin - пин для подключения сервопривода
     */
    DoorController(uint8_t pin);

    /**
     * Открыть дверь для входа (0 градусов)
     */
    void openForEntry();

    /**
     * Открыть дверь для выхода (180 градусов)
     */
    void openForExit();

    /**
     * Закрыть дверь (90 градусов)
     */
    void closeDoor();
};

#endif
