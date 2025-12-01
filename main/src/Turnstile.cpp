#include "Turnstile.h"

Turnstile::Turnstile(RFIDReader* rfidEntry, RFIDReader* rfidExit, 
              Display* display, DoorController* door,
              DistanceSensor* distanceSensor, IDStorage* idStorage, Logger* logger) {
    this->rfidEntry = rfidEntry;
    this->rfidExit = rfidExit;
    this->display = display;
    this->door = door;
    this->distanceSensor = distanceSensor;
    this->idStorage = idStorage;
    this->logger = logger;
    this->lastDistanceMeasureTime = 0;
    this->uidSize = 0;

    // Инициализация дисплея
    display->init();

    // Показать начальное сообщение
    display->showWaitingMessage();

    // Закрыть дверь
    door->closeDoor();

    // Калибровать дальномер
    baseDistance = distanceSensor->calibrate();

    // Установить начальное состояние
    setState(STATE_WAITING);
}

void Turnstile::loop() {
    switch (state) {
        case STATE_WAITING:
            handleWaiting();
            break;
        case STATE_PASSAGE_WAITING:
            handlePassageWaiting();
            break;
        case STATE_PASSAGE_DETECTED:
            handlePassageDetected();
            break;
        case STATE_CLOSING:
        case STATE_ERROR_MESSAGE:
        case STATE_ACCESS_DENIED:
            handleClosing();
            break;
    }
}

void Turnstile::setState(TurnstileState newState) {
    state = newState;
    stateStartTime = millis();
}

void Turnstile::handleWaiting() {
    bool cardReady = false;

    // Проверяем передний RFID (вход)
    if (rfidEntry->readCardID(uid, uidSize)) {
        direction = DIR_ENTRY;
        cardReady = false;
        door->openForEntry();
    }
    // Проверяем задний RFID (выход)
    else if (rfidExit->readCardID(uid, uidSize)) {
        direction = DIR_EXIT;
        cardReady = false;
        door->openForExit();
    }

    if (cardReady) {
        if (idStorage->isIDAllowed(uid, uidSize)) {
            setState(STATE_PASSAGE_WAITING);
            display->showGoodbyeMessage();
        } else {
            // Логируем "Проход запрещен"
            logger->log(uid, uidSize, false, direction == DIR_ENTRY, false);
            setState(STATE_ACCESS_DENIED);
            display->showAccessDeniedMessage();
        }
    }
}

void Turnstile::handlePassageWaiting() {
    // Проверяем, не истекло ли время ожидания
    if (millis() - stateStartTime >= TIMER_PASSAGE) {
        // Время истекло, проход не был осуществлен
        logger->log(uid, uidSize, true, direction == DIR_ENTRY, false);
        setState(STATE_ERROR_MESSAGE);
        display->showNoPassageMessage();
        door->closeDoor();
        return;
    }

    // Проверяем проход каждые 10 мс
    checkPassage();
}

void Turnstile::handlePassageDetected() {
    unsigned long elapsed = millis() - stateStartTime;

    // Измеряем расстояние каждые 10 мс
    checkPassage();

    // Проверяем, не истекло ли время ожидания
    if (elapsed >= TIMER_PASSAGE_DETECTED) {
        // Проход завершен, логируем успешный проход
        logger->log(uid, uidSize, true, direction == DIR_ENTRY, true);
        setState(STATE_CLOSING);
        return;
    }
}

void Turnstile::handleClosing() {
    if (millis() - stateStartTime >= TIMER_CLOSING) {
        // Время показа ошибки истекло
        // Время показа сообщения истекло
        // Время на закрытие истекло, возвращаемся в режим ожидания
        setState(STATE_WAITING);
        display->showWaitingMessage();
    }
}

void Turnstile::checkPassage() {
    if (millis() - lastDistanceMeasureTime >= DISTANCE_MEASURE_INTERVAL) {
        float currentDistance = distanceSensor->measureDistance();

        // Проверяем, уменьшилось ли расстояние от базового
        // Человек проходит, когда расстояние становится меньше базового
        if (currentDistance > 0 && currentDistance < baseDistance * 0.95) {
            // Обнаружен проход - человек вошел в зону
            setState(STATE_PASSAGE_DETECTED);
        }

        lastDistanceMeasureTime = millis();
    }
}
