#ifndef TURNSTILE_H
#define TURNSTILE_H

#include <Arduino.h>
#include "RFIDReader.h"
#include "Display.h"
#include "DoorController.h"
#include "DistanceSensor.h"
#include "IDStorage.h"
#include "Logger.h"

/**
 * Состояния турникета
 */
enum TurnstileState {
    STATE_WAITING,              // Ожидание карты
    STATE_PASSAGE_WAITING,      // Ожидание прохода (15 сек)
    STATE_PASSAGE_DETECTED,     // Проход детектирован (1 сек)
    STATE_CLOSING,              // Закрытие двери или показ сообщения (5 сек)
    STATE_MESSAGE,              // Показ сообщения (5 сек)
};

/**
 * Направление движения
 */
enum Direction {
    DIR_BLOCK,      // Закрыто
    DIR_ENTRY,      // Вход (передний RFID)
    DIR_EXIT        // Выход (задний RFID)
};

/**
 * Основной класс турникета
 */
class Turnstile {
private:
    RFIDReader* rfidEntry;         // Передний RFID модуль
    RFIDReader* rfidExit;          // Задний RFID модуль
    Display* display;
    DoorController* door;
    DistanceSensor* distanceSensor;
    IDStorage* idStorage;
    Logger* logger;

    unsigned long stateStartTime;           // Время входа в текущее состояние
    unsigned long lastDistanceMeasureTime;  // Время последнего измерения расстояния
    unsigned int baseDistance;              // Базовая ширина прохода

    TurnstileState state;
    Direction direction;

    // Информация о текущей карте для логирования
    byte uid[10];
    byte uidSize;

    // Таймеры для различных состояний
    static const unsigned long TIMER_PASSAGE = 15000;           // 15 сек на проход
    static const unsigned long TIMER_CLOSING = 1000;            // 5 сек на закрытие
    static const unsigned long TIMER_MESSAGE = 4000;            // 5 сек на сообщение
    static const unsigned long TIMER_PASSAGE_DETECTED = 1000;   // 1 сек после детектирования прохода
    static const unsigned long DISTANCE_MEASURE_INTERVAL = 10;  // Интервал измерения расстояния (10 мс)

    /**
     * Обработка состояния "Ожидание"
     */
    void handleWaiting();

    /**
     * Обработка состояния "Ожидание прохода"
     */
    void handlePassageWaiting();

    /**
     * Обработка состояния "Проход детектирован"
     */
    void handlePassageDetected();

    /**
     * Обработка состояния "Закрытие двери"
     */
    void handleClosing();

    /**
     * Обработка состояния "Показ сообщения"
     */
    void handleMessage();

    /**
     * Переход в состояние
     */
    void setState(TurnstileState newState);

    /**
     * Проверка прохода человека
     */
    void checkPassage();

public:
    /**
     * Конструктор
     */
    Turnstile(RFIDReader* rfidEntry, RFIDReader* rfidExit, 
              Display* display, DoorController* door,
              DistanceSensor* distanceSensor, IDStorage* idStorage, Logger* logger);

    /**
     * Основной цикл обработки
     */
    void loop();
};

#endif
