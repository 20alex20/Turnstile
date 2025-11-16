#ifndef DISTANCESENSOR_H
#define DISTANCESENSOR_H

#include <Arduino.h>

/**
 * Класс для управления ультразвуковым дальномером HC-SR04
 */
class DistanceSensor {
private:
    uint8_t trigPin;
    uint8_t echoPin;
    
public:
    /**
     * Конструктор
     * @param trigPin - пин для передачи сигнала
     * @param echoPin - пин для приема сигнала
     */
    DistanceSensor(uint8_t trigPin, uint8_t echoPin);
    
    /**
     * Инициализация датчика
     */
    void init();
    
    /**
     * Измерение расстояния
     * @return расстояние в см (0 если ошибка)
     */
    float measureDistance();
    
    /**
     * Инициализация ширины прохода
     * Измеряет базовое расстояние при старте
     */
    float calibrate();
};

#endif
