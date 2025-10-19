#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>

// Пины TFT для UNO с микросхемой 74HC595
// 74HC595 подключена к пинам: A5 (DS), 8 (ST_CP), 9 (SH_CP)
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void setup(void) {
  Serial.begin(9600);
  Serial.println("TFT LCD Test with 74HC595");

  // Инициализация TFT
  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(1);
  
  // Тест различных цветов
  tft.fillScreen(0x0000); // Черный экран
  delay(1000);
  
  tft.fillScreen(0xF800); // Красный экран
  delay(1000);
  
  tft.fillScreen(0x07E0); // Зеленый экран
  delay(1000);
  
  tft.fillScreen(0x001F); // Синий экран
  delay(1000);
  
  tft.fillScreen(0x0000); // Черный экран
  
  // Тест текста
  tft.setCursor(40, 50);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);
  tft.println("74HC595 Test!");
  
  tft.setCursor(40, 80);
  tft.setTextColor(0xF800);
  tft.setTextSize(1);
  tft.println("Data via Shift Register");
  
  Serial.println("TFT LCD initialized successfully!");
}

void loop(void) {
  // Тест рисования пикселей
  static uint16_t color = 0xF800;
  static int x = 0;
  static int y = 0;
  
  tft.drawPixel(x, y, color);
  
  x++;
  if (x >= tft.width()) {
    x = 0;
    y++;
    if (y >= tft.height()) {
      y = 0;
      // Смена цвета
      if (color == 0xF800) color = 0x07E0;
      else if (color == 0x07E0) color = 0x001F;
      else color = 0xF800;
    }
  }
  
  delay(1);
}
