#ifndef DISTANCESENSOR_H
#define DISTANCESENSOR_H

#include <Arduino.h>

/**
 * Класс для управления ультразвуковым дальномером HC-SR04
 */
class DistanceSensor {
private:
    byte trigPin;

public:
    static const uint8_t MAX_DISTANCE = 200;

    /**
     * Конструктор
     * @param trigPin - пин для передачи сигнала
     * @param echoPin - пин для приема сигнала
     */
    DistanceSensor(uint8_t trigPin, uint8_t echoPin);

    /**
     * Измерение расстояния
     * @return расстояние в см
     */
    unsigned long measureDistance();

    /**
     * Инициализация ширины прохода
     * Измеряет базовое расстояние при старте
     */
    unsigned long calibrate();
};

#endif
