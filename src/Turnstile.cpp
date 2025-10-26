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
    this->state = STATE_WAITING;
    this->stateStartTime = 0;
    this->lastDistanceMeasureTime = 0;
    this->baseDistance = 0;
    this->currentUIDSize = 0;
    this->currentAllowed = false;
    this->currentPassed = false;
}

void Turnstile::init() {
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
            handleClosing();
            break;
        case STATE_ERROR_MESSAGE:
            handleErrorMessage();
            break;
        case STATE_ACCESS_DENIED:
            handleAccessDenied();
            break;
    }
}

void Turnstile::setState(TurnstileState newState) {
    state = newState;
    stateStartTime = millis();
}

void Turnstile::handleWaiting() {
    byte uid[10];
    byte uidSize;
    
    // Проверяем передний RFID (вход)
    if (rfidEntry->isCardPresent() && rfidEntry->readCardID(uid, uidSize)) {
        // Сохраняем информацию для логирования
        for (byte i = 0; i < uidSize; i++) {
            currentUID[i] = uid[i];
        }
        currentUIDSize = uidSize;
        currentAllowed = idStorage->isIDAllowed(uid, uidSize);
        currentPassed = false;
        
        // Проверяем ID в базе
        if (currentAllowed) {
            direction = DIR_ENTRY;
            setState(STATE_PASSAGE_WAITING);
            display->showWelcomeMessage();
            door->openForEntry();
            return;
        } else {
            // Логируем "Проход запрещен"
            logger->log(currentUID, currentUIDSize, false, false);
            setState(STATE_ACCESS_DENIED);
            display->showAccessDeniedMessage();
            return;
        }
    }

    // Проверяем задний RFID (выход)
    if (rfidExit->isCardPresent() && rfidExit->readCardID(uid, uidSize)) {
        // Сохраняем информацию для логирования
        for (byte i = 0; i < uidSize; i++) {
            currentUID[i] = uid[i];
        }
        currentUIDSize = uidSize;
        currentAllowed = idStorage->isIDAllowed(uid, uidSize);
        currentPassed = false;
        
        // Проверяем ID в базе
        if (currentAllowed) {
            direction = DIR_EXIT;
            setState(STATE_PASSAGE_WAITING);
            display->showGoodbyeMessage();
            door->openForExit();
            return;
        } else {
            // Логируем "Проход запрещен"
            logger->log(currentUID, currentUIDSize, false, false);
            setState(STATE_ACCESS_DENIED);
            display->showAccessDeniedMessage();
            return;
        }
    }
}

void Turnstile::handlePassageWaiting() {
    unsigned long elapsed = millis() - stateStartTime;
    
    // Проверяем, не истекло ли время ожидания
    if (elapsed >= TIMER_PASSAGE) {
        // Время истекло, проход не был осуществлен
        currentPassed = false;
        logger->log(currentUID, currentUIDSize, true, false);
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
    if (millis() - lastDistanceMeasureTime >= DISTANCE_MEASURE_INTERVAL) {
        float currentDistance = distanceSensor->measureDistance();
        
        if (currentDistance > 0 && currentDistance < baseDistance * 0.95) {
            // Все еще регистрируется проход, сбрасываем таймер
            setState(STATE_PASSAGE_DETECTED);
        }
        
        lastDistanceMeasureTime = millis();
    }
    
    // Проверяем, не истекло ли время ожидания
    if (elapsed >= TIMER_PASSAGE_DETECTED) {
        // Проход завершен, логируем успешный проход
        currentPassed = true;
        logger->log(currentUID, currentUIDSize, true, true);
        setState(STATE_CLOSING);
        return;
    }
}

void Turnstile::handleClosing() {
    unsigned long elapsed = millis() - stateStartTime;
    
    if (elapsed >= TIMER_CLOSING) {
        // Время на закрытие истекло, возвращаемся в режим ожидания
        setState(STATE_WAITING);
        display->showWaitingMessage();
    }
}

void Turnstile::handleErrorMessage() {
    unsigned long elapsed = millis() - stateStartTime;

    if (elapsed >= TIMER_ERROR) {
        // Время показа ошибки истекло
        setState(STATE_WAITING);
        display->showWaitingMessage();
    }
}

void Turnstile::handleAccessDenied() {
    unsigned long elapsed = millis() - stateStartTime;

    if (elapsed >= TIMER_ERROR) {
        // Время показа сообщения истекло
        setState(STATE_WAITING);
        display->showWaitingMessage();
    }
}

void Turnstile::checkPassage() {
    if (millis() - lastDistanceMeasureTime < DISTANCE_MEASURE_INTERVAL) {
        return;
    }
    
    float currentDistance = distanceSensor->measureDistance();

    // Проверяем, уменьшилось ли расстояние от базового
    // Человек проходит, когда расстояние становится меньше базового
    if (currentDistance > 0 && currentDistance < baseDistance * 0.95) {
        // Обнаружен проход - человек вошел в зону
        setState(STATE_PASSAGE_DETECTED);
    }
    
    lastDistanceMeasureTime = millis();
}
