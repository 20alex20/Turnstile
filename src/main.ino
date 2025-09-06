#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>

// ---------------- TFT пины для шилда UNO
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

// ---------------- Touch пины (не используются в этой программе, оставлены для совместимости)
#define YP A2
#define XM A3
#define YM 9
#define XP 8
TouchScreen ts(XP, YP, XM, YM, 300);

// ---------------- SD
#define SD_CS 10  // пин CS SD на шилде

// ---------------- Настройки текста
#define TEXT_COLOR 0x0000  // черный текст
#define TEXT_SIZE 2

void setup() {
  Serial.begin(9600);

  // Инициализация TFT
  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(1);               // горизонтальная ориентация
  tft.fillScreen(0x07E0);           // зеленый фон (0x07E0 в 16-бит RGB565)

  tft.setTextColor(TEXT_COLOR);
  tft.setTextSize(TEXT_SIZE);

  // Инициализация SD
  Serial.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("Card failed or not present");
    tft.setCursor(10, 10);
    tft.println("SD Card error!");
    while (1); // остановка
  }
  Serial.println("SD OK");

  // Открываем файл
  File myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("Reading test.txt...");

    int cursorX = 10;
    int cursorY = 10;
    tft.setCursor(cursorX, cursorY);

    while (myFile.available()) {
      char c = myFile.read();

      // Если символ переноса строки
      if (c == '\n') {
        cursorY += 16 * TEXT_SIZE;  // перенос на новую строку
        cursorX = 10;
        tft.setCursor(cursorX, cursorY);
      } else {
        tft.write(c);
      }
    }

    myFile.close();
    Serial.println("Done!");
  } else {
    Serial.println("Error opening test.txt");
    tft.setCursor(10, 10);
    tft.println("File not found!");
  }
}

void loop() {
  // ничего не делаем
}

