#include <SPI.h>
#include "src/RFIDReader.h"
#include "src/Display.h"
#include "src/DoorController.h"
#include "src/DistanceSensor.h"
#include "src/IDStorage.h"
#include "src/Turnstile.h"
#include "src/Logger.h"

// Пины для TFT LCD (через 74HC595)
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RESET A4

// Пины для RFID модулей
#define RFID_ENTRY_SS 7
#define RFID_ENTRY_RST 6
#define RFID_EXIT_SS 5
#define RFID_EXIT_RST 4

// Пины для дальномера
#define TRIG_PIN 3
#define ECHO_PIN 2

// Пин для сервопривода
#define SERVO_PIN A5

// Пин для SD карты (через шилд)
#define SD_CS 10

// Создаем объекты компонентов
RFIDReader* rfidEntry;
RFIDReader* rfidExit;
Display* display;
DoorController* door;
DistanceSensor* distanceSensor;
IDStorage* idStorage;
Logger* logger;
Turnstile* turnstile;

void setup() {
    Serial.begin(9600);
    delay(100);

    // Инициализация SPI для SD и RFID
    SPI.begin();

    // Инициализация дисплея
    display = new Display(LCD_CS, LCD_CD, LCD_WR, LCD_RESET);

    // Инициализация SD карты
    idStorage = new IDStorage("ids.data", SD_CS);
    if (!idStorage->init()) {
        while (1); // Остановка программы
    }

    // Инициализация Logger
    logger = new Logger("logs.txt");
    logger->requestDateTime();

    // Инициализация RFID модулей
    rfidEntry = new RFIDReader(RFID_ENTRY_SS, RFID_ENTRY_RST);
    rfidExit = new RFIDReader(RFID_EXIT_SS, RFID_EXIT_RST);

    // Инициализация дальномера
    distanceSensor = new DistanceSensor(TRIG_PIN, ECHO_PIN);

    // Инициализация сервопривода
    door = new DoorController(SERVO_PIN);

    // Создание и инициализация турникета
    turnstile = new Turnstile(rfidEntry, rfidExit, display, door, distanceSensor, idStorage, logger);
}

char serialBuffer[50];
byte serialPos = 0;

void loop() {
    // Обработка команды из Serial
    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\n' || c == '\r') {
            if (serialPos > 0) {
                serialBuffer[serialPos] = 0;
                IDStorage::handleCommand(serialBuffer, idStorage, logger);
                serialPos = 0;
            }
        } else if (c >= 32 && c <= 126 && serialPos < 49) {
            if (c != ' ' || serialPos > 0) {
                serialBuffer[serialPos++] = c;
            }
        }
    }
    
    // Основной цикл турникета
    turnstile->loop();
    delay(1); // Небольшая задержка для стабильности
}
