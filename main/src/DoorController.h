#ifndef DOORCONTROLLER_H
#define DOORCONTROLLER_H

#include <Arduino.h>
#include <Servo.h>

/**
 * Класс для управления дверью турникета (сервоприводом)
 */
class DoorController {
private:
    Servo servo;
    uint8_t pin;
    
public:
    static const uint8_t POS_CLOSED = 90;      // Закрыто
    static const uint8_t POS_OPENED_IN = 0;    // Открыто для входа
    static const uint8_t POS_OPENED_OUT = 180; // Открыто для выхода

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
