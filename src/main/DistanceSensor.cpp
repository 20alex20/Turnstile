#include "DistanceSensor.h"

DistanceSensor::DistanceSensor(uint8_t trigPin, uint8_t echoPin) {
    this->trigPin = trigPin;
    this->echoPin = echoPin;
}

void DistanceSensor::init() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    digitalWrite(trigPin, LOW);
}

float DistanceSensor::measureDistance() {
    // Отправляем короткий импульс
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Измеряем время прихода эха
    unsigned long duration = pulseIn(echoPin, HIGH);
    
    // Рассчитываем расстояние (скорость звука 343 м/с)
    float distance = duration * 0.0343 / 2.0;
    return distance;
}

float DistanceSensor::calibrate() {
    // Делаем несколько измерений и берем среднее
    float sum = 0;
    int count = 0;
    
    for (int i = 0; i < 10; i++) {
        float dist = measureDistance();
        if (dist > 0) {
            sum += dist;
            count++;
        }
        delay(10);
    }
    
    if (count > 0) {
        return sum / count;
    }
    return 0;
}
