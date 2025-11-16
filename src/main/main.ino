#include <SPI.h>

#include "RFIDReader.h"
#include "Display.h"
#include "DoorController.h"
#include "DistanceSensor.h"
#include "IDStorage.h"
#include "Turnstile.h"
#include "TimeManager.h"
#include "Logger.h"
#include "CommandHandler.h"

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
TimeManager* timeManager;
Logger* logger;
CommandHandler* commandHandler;
Turnstile* turnstile;

void setup() {
    Serial.begin(9600);
    delay(100);
    
    // Инициализация TimeManager и запрос времени
    Serial.println("Initializing time manager...");
    timeManager = new TimeManager();
    timeManager->requestDateTime();
    
    // Инициализация SPI для SD и RFID
    Serial.println("Initializing SPI...");
    SPI.begin();
    
    // Инициализация дисплея
    Serial.println("Initializing display...");
    display = new Display(LCD_CS, LCD_CD, LCD_WR, LCD_RESET);
    display->init();
    Serial.println("TFT display initialized");
    
    // Инициализация SD карты
    Serial.println("Initializing SD card...");
    idStorage = new IDStorage("ids.data", SD_CS);
    if (!idStorage->init()) {
        Serial.println("SD card initialization failed!");
        while (1); // Остановка программы
    }
    Serial.println("SD card initialized");
    
    // Инициализация Logger
    Serial.println("Initializing logger...");
    logger = new Logger("logs.txt", timeManager);
    Serial.println("Logger initialized");
    
    // Инициализация CommandHandler
    Serial.println("Initializing command handler...");
    commandHandler = new CommandHandler(idStorage, logger);
    Serial.println("Command handler initialized");
    
    // Инициализация RFID модулей
    Serial.println("Initializing RFID readers...");
    rfidEntry = new RFIDReader(RFID_ENTRY_SS, RFID_ENTRY_RST);
    rfidExit = new RFIDReader(RFID_EXIT_SS, RFID_EXIT_RST);
    rfidEntry->init();
    rfidExit->init();
    Serial.println("RFID readers initialized");
    
    // Инициализация дальномера
    Serial.println("Initializing distance sensor...");
    distanceSensor = new DistanceSensor(TRIG_PIN, ECHO_PIN);
    distanceSensor->init();
    Serial.println("Distance sensor initialized");
    
    // Инициализация сервопривода
    Serial.println("Initializing door controller...");
    door = new DoorController(SERVO_PIN);
    door->init();
    Serial.println("Door controller initialized");
    
    // Создание и инициализация турникета
    Serial.println("Creating turnstile system...");
    turnstile = new Turnstile(rfidEntry, rfidExit, display, door, distanceSensor, idStorage, logger);
    turnstile->init();
    
    Serial.println("Turnstile system ready!");
}

String serialBuffer = "";

void loop() {
    // Обработка команды из Serial
    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\n') {
            if (serialBuffer.length() > 0) {
                commandHandler->handleCommand(serialBuffer);
                serialBuffer = "";
            }
        } else if (c >= 32 && c <= 126) {  // Печатные символы
            serialBuffer += c;
        }
    }
    
    // Основной цикл турникета
    turnstile->loop();
    delay(1); // Небольшая задержка для стабильности
}
