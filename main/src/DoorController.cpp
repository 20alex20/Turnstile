#include "DoorController.h"

DoorController::DoorController(uint8_t pin) {
    closeDoor();
    pinMode(pin, OUTPUT);
}

void DoorController::openForEntry() {
    write(POS_OPENED_IN);
}

void DoorController::openForExit() {
    write(POS_OPENED_OUT);
}

void DoorController::closeDoor() {
    write(POS_CLOSED);
}

void DoorController::write(uint8_t angle) {
    if (angle > 180) angle = 180;

    // Переводим угол в микросекунды: 0° -> 1000 мкс, 180° -> 2000 мкс
    uint16_t pulse_us = map(angle, 0, 180, 500, 2500);

    // Таймер1 — режим Fast PWM 16 бит, TOP = ICR1
    // Частота = 50 Гц → период 20 000 мкс
    // Формула: ICR1 = F_CPU / prescaler / freq - 1
    // Для 50 Гц, prescaler = 8 → ICR1 = 16MHz / 8 / 50 - 1 = 39999
    // OCR1A = pulse_us * 2 (так как 0.5 мкс на тик при 16 MHz / 8)

    cli(); // выключаем прерывания на время настройки

    // Настройка таймера 1 только один раз
    // Fast PWM, TOP = ICR1
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;

    ICR1 = 39999; // 20 мс период

    // Выбираем канал OCR1A для пина 9
    TCCR1A |= (1 << COM1A1); // Non-inverting mode
    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM12) | (1 << WGM13);
    TCCR1B |= (1 << CS11);   // prescaler = 8

    OCR1A = pulse_us * 2; // ширина импульса в "таймерных тиках"

    sei(); // включаем прерывания
}
