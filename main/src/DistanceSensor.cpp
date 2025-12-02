#include "DistanceSensor.h"

DistanceSensor::DistanceSensor(uint8_t trigPin, uint8_t echoPin) {
    this->trigPin = trigPin;
    this->echoPin = echoPin;

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    digitalWrite(trigPin, LOW);
}

unsigned long DistanceSensor::measureDistance() {
    // Отправляем короткий импульс
    delayMicroseconds(10);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    return pulseIn(echoPin, HIGH);
}

unsigned long DistanceSensor::calibrate() {
    // Делаем несколько измерений и берем среднее
    unsigned long sum = 0;
    int count = 0;

    for (int i = 0; i < 10; i++) {
        unsigned long dist = measureDistance();
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
