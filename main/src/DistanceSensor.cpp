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
    digitalWrite(trigPin, LOW);
    delayMicroseconds(10);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Измеряем время прихода эха
    unsigned long j = pulseIn(echoPin, HIGH);
    Serial.println(j);
    return j;
}

unsigned long DistanceSensor::calibrate() {
    unsigned long dist = 0;
    while (dist == 0) {
        dist = measureDistance();
    }
    return dist;
}
