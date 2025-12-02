#include "DistanceSensor.h"

volatile unsigned long echo_start;
volatile unsigned long echo_end;
volatile bool echo_done;
volatile byte echo_pin;

void echo_isr() {
    if (digitalRead(echo_pin) == HIGH) {
        echo_start = micros();
    } else {
        echo_end = micros();
        echo_done = true;
    }
}

DistanceSensor::DistanceSensor(uint8_t trigPin, uint8_t echoPin) {
    this->trigPin = trigPin;
    echo_pin = echoPin;

    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    digitalWrite(trigPin, LOW);

    attachInterrupt(digitalPinToInterrupt(echoPin), echo_isr, CHANGE);
}

unsigned long DistanceSensor::measureDistance() {
    echo_start = 0UL;
    echo_end = 0UL;
    echo_done = false;

    // Отправляем короткий импульс
    delayMicroseconds(10);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    unsigned long t0 = millis();
    while (!echo_done) {
        if (millis() - t0 > 100) break; // таймаут
    }

    return echo_start != 0UL && echo_end != 0UL ? echo_end - echo_start : 0UL;
}

unsigned long DistanceSensor::calibrate() {
    unsigned long dist = 0;
    while (dist == 0) {
        dist = measureDistance();
    }
    return dist;
}
