#include "TimeManager.h"

TimeManager::TimeManager() {
    startTime.day = 1;
    startTime.month = 1;
    startTime.year = 2024;
    startTime.hour = 0;
    startTime.minute = 0;
    startTime.second = 0;
    startMillis = 0;
}

void TimeManager::requestDateTime() {
    Serial.println("Please enter current date and time (format: DD:MM:YYYY HH:MM:SS)");
    Serial.println("Example: 25:12:2024 14:30:00");
    Serial.print("> ");
    
    while (!Serial.available()) {
        delay(10);
    }
    
    String input = Serial.readString();
    input.trim();
    
    // Парсинг строки "DD:MM:YYYY HH:MM:SS"
    if (input.length() >= 19) {
        // День
        startTime.day = (input.charAt(0) - '0') * 10 + (input.charAt(1) - '0');
        // Месяц
        startTime.month = (input.charAt(3) - '0') * 10 + (input.charAt(4) - '0');
        // Год
        startTime.year = (input.charAt(6) - '0') * 1000 + 
                        (input.charAt(7) - '0') * 100 + 
                        (input.charAt(8) - '0') * 10 + 
                        (input.charAt(9) - '0');
        // Час
        startTime.hour = (input.charAt(11) - '0') * 10 + (input.charAt(12) - '0');
        // Минута
        startTime.minute = (input.charAt(14) - '0') * 10 + (input.charAt(15) - '0');
        // Секунда
        startTime.second = (input.charAt(17) - '0') * 10 + (input.charAt(18) - '0');
        
        startMillis = millis();
        
        Serial.print("Time set: ");
        Serial.println(formatDateTime(startTime));
    } else {
        Serial.println("Invalid format, using default time");
        startMillis = millis();
    }
}

void TimeManager::setDateTime(uint8_t day, uint8_t month, uint16_t year, 
                             uint8_t hour, uint8_t minute, uint8_t second) {
    startTime.day = day;
    startTime.month = month;
    startTime.year = year;
    startTime.hour = hour;
    startTime.minute = minute;
    startTime.second = second;
    startMillis = millis();
}

void TimeManager::getCurrentDateTime(DateTime& dt) {
    unsigned long elapsedMillis = millis() - startMillis;
    
    dt.day = startTime.day;
    dt.month = startTime.month;
    dt.year = startTime.year;
    dt.hour = startTime.hour;
    dt.minute = startTime.minute;
    dt.second = startTime.second;
    
    // Добавляем прошедшие секунды
    unsigned long totalSeconds = elapsedMillis / 1000;
    unsigned long seconds = totalSeconds % 60;
    unsigned long minutes = (totalSeconds / 60) % 60;
    unsigned long hours = (totalSeconds / 3600) % 24;
    unsigned long days = totalSeconds / 86400;
    
    dt.second += seconds;
    if (dt.second >= 60) {
        dt.second -= 60;
        minutes++;
    }
    
    dt.minute += minutes;
    if (dt.minute >= 60) {
        dt.minute -= 60;
        hours++;
    }
    
    dt.hour += hours;
    if (dt.hour >= 24) {
        dt.hour -= 24;
        days++;
    }
    
    dt.day += days;
    
    // Простой расчет дней в месяце
    uint8_t daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool isLeapYear = (dt.year % 4 == 0 && (dt.year % 100 != 0 || dt.year % 400 == 0));
    if (isLeapYear && dt.month == 2) {
        daysInMonth[1] = 29;
    }
    
    while (dt.day > daysInMonth[dt.month - 1]) {
        dt.day -= daysInMonth[dt.month - 1];
        dt.month++;
        if (dt.month > 12) {
            dt.month = 1;
            dt.year++;
        }
    }
}

String TimeManager::formatDateTime(const DateTime& dt) {
    String str = "";
    
    // День
    if (dt.day < 10) str += "0";
    str += dt.day;
    str += ":";
    
    // Месяц
    if (dt.month < 10) str += "0";
    str += dt.month;
    str += ":";
    
    // Год
    str += dt.year;
    str += " ";
    
    // Час
    if (dt.hour < 10) str += "0";
    str += dt.hour;
    str += ":";
    
    // Минута
    if (dt.minute < 10) str += "0";
    str += dt.minute;
    str += ":";
    
    // Секунда
    if (dt.second < 10) str += "0";
    str += dt.second;
    
    return str;
}

