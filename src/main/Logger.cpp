#include "Logger.h"
#include <SD.h>

Logger::Logger(const char* filename, TimeManager* timeManager) {
    this->filename = filename;
    this->timeManager = timeManager;
    this->serialLogging = false;
}

String Logger::formatCardID(byte* uid, byte uidSize) {
    String str = "";
    for (byte i = 0; i < uidSize; i++) {
        if (uid[i] < 0x10) {
            str += "0";
        }
        String hex = String(uid[i], HEX);
        hex.toUpperCase();
        str += hex;
        if (i < uidSize - 1) {
            str += " ";
        }
    }
    return str;
}

void Logger::log(byte* uid, byte uidSize, bool allowed, bool direcyionEntry, bool passed) {
    // Получаем текущее время
    DateTime dt;
    timeManager->getCurrentDateTime(dt);
    String timestamp = timeManager->formatDateTime(dt);
    
    // Форматируем ID карты
    String cardID = formatCardID(uid, uidSize);
    
    // Создаем строку лога
    String logLine = timestamp + " " + cardID + " : ";

    if (direcyionEntry) {
        logLine += "Вход ";
    } else {
        logLine += "Выход ";
    }

    if (allowed) {
        logLine += "разрешен";
        if (!passed) {
            logLine += " НЕ ПРОШЕЛ";
        }
    } else {
        logLine += "запрещен";
    }
    
    // Отправляем в Serial, если включено
    if (serialLogging) {
        Serial.println(logLine);
    }
    
    // Записываем в файл
    File file = SD.open(filename, FILE_WRITE);
    if (file) {
        file.seek(file.size()); // Переходим в конец файла
        file.println(logLine);
        file.close();
    }
}

void Logger::setSerialLogging(bool enabled) {
    serialLogging = enabled;
}
