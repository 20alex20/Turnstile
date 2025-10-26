#include <SPI.h>

#include "../RFIDReader.h"
#include "../Display.h"
#include "../DoorController.h"
#include "../DistanceSensor.h"
#include "../IDStorage.h"
#include "../Turnstile.h"

// Пины для TFT LCD (через 74HC595)
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RESET A4

// Пины для RFID модулей
#define RFID_ENTRY_SS 2
#define RFID_ENTRY_RST 3
#define RFID_EXIT_SS 4
#define RFID_EXIT_RST 5

// Пины для дальномера
#define TRIG_PIN 6
#define ECHO_PIN 7

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
Turnstile* turnstile;

void setup() {
    Serial.begin(9600);
    delay(100);
    
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
    turnstile = new Turnstile(rfidEntry, rfidExit, display, door, distanceSensor, idStorage);
    turnstile->init();
    
    Serial.println("Turnstile system ready!");
}

void loop() {
    turnstile->loop();
    delay(1); // Небольшая задержка для стабильности
}
