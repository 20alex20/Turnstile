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
    this->direction = DIR_BLOCK;
    this->uidSize = 0;

    // Калибровать дальномер
    baseDistance = distanceSensor->calibrate() * 9UL / 10UL;

    // Инициализация дисплея
    display->init();

    // Показать начальное сообщение
    display->showWaitingMessage();

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
            handleClosing();
            break;
        case STATE_MESSAGE:
            handleMessage();
            break;
    }
}

void Turnstile::setState(TurnstileState newState) {
    state = newState;
    stateStartTime = millis();
}

void Turnstile::handleWaiting() {
    if (rfidEntry->readCardID(uid, uidSize)) {  // Проверяем передний RFID (вход)
        direction = DIR_ENTRY;
        if (idStorage->isIDAllowed(uid, uidSize)) {
            setState(STATE_PASSAGE_WAITING);
            door->openForEntry();
            display->showWelcomeMessage();
            return;
        }
    } else if (rfidExit->readCardID(uid, uidSize)) {  // Проверяем задний RFID (выход)
        direction = DIR_EXIT;
        if (idStorage->isIDAllowed(uid, uidSize)) {
            setState(STATE_PASSAGE_WAITING);
            door->openForExit();
            display->showGoodbyeMessage();
            return;
        }
    }

    if (direction != DIR_BLOCK) {
        // Логируем "Проход запрещен"
        logger->log(uid, uidSize, false, direction == DIR_ENTRY, false);
        setState(STATE_MESSAGE);
        display->showAccessDeniedMessage();
    }
}

void Turnstile::handlePassageWaiting() {
    // Проверяем, не истекло ли время ожидания
    if (millis() - stateStartTime >= TIMER_PASSAGE) {
        // Время истекло, проход не был осуществлен
        logger->log(uid, uidSize, true, direction == DIR_ENTRY, false);
        setState(STATE_MESSAGE);
        door->closeDoor();
        display->showNoPassageMessage();
        return;
    }

    // Проверяем проход каждые 10 мс
    checkPassage();
}

void Turnstile::handlePassageDetected() {
    // Измеряем расстояние каждые 10 мс
    checkPassage();

    // Проверяем, не истекло ли время ожидания
    if (millis() - stateStartTime >= TIMER_PASSAGE_DETECTED) {
        // Проход завершен, логируем успешный проход
        logger->log(uid, uidSize, true, direction == DIR_ENTRY, true);
        setState(STATE_CLOSING);
    }
}

void Turnstile::handleClosing() {
    if (millis() - stateStartTime >= TIMER_CLOSING) {
        // Возвращаемся в режим ожидания
        setState(STATE_WAITING);
        door->closeDoor();
        direction = DIR_BLOCK;
        display->showWaitingMessage();
    }
}

void Turnstile::handleMessage() {
    if (millis() - stateStartTime >= TIMER_MESSAGE) {
        // Время показа сообщения истекло
        setState(STATE_WAITING);
        direction = DIR_BLOCK;
        display->showWaitingMessage();
    }
}

void Turnstile::checkPassage() {
    if (millis() - lastDistanceMeasureTime >= DISTANCE_MEASURE_INTERVAL) {
        unsigned long currentDistance = distanceSensor->measureDistance();

        // Проверяем, уменьшилось ли расстояние от базового
        if (currentDistance > 0UL && currentDistance < baseDistance) {
            // Обнаружен проход - человек вошел в зону
            setState(STATE_PASSAGE_DETECTED);
        }

        lastDistanceMeasureTime = millis();
    }
}
