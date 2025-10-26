#include "CommandHandler.h"
#include <Arduino.h>

CommandHandler::CommandHandler(IDStorage* idStorage, Logger* logger) {
    this->idStorage = idStorage;
    this->logger = logger;
}

bool CommandHandler::parseCardID(const String& str, byte* uid, byte& uidSize) {
    String cleanStr = str;
    cleanStr.trim();
    
    // Подсчитываем количество байт (разделитель - пробел)
    int count = 1;
    for (int i = 0; i < cleanStr.length(); i++) {
        if (cleanStr.charAt(i) == ' ') {
            count++;
        }
    }
    
    if (count < 1 || count > 10) {
        return false;
    }
    
    uidSize = count;
    
    // Парсим каждый байт
    int pos = 0;
    for (byte i = 0; i < uidSize; i++) {
        String byteStr = "";
        
        // Находим следующий пробел или конец строки
        int spacePos = cleanStr.indexOf(' ', pos);
        if (spacePos == -1) {
            byteStr = cleanStr.substring(pos);
        } else {
            byteStr = cleanStr.substring(pos, spacePos);
            pos = spacePos + 1;
        }
        
        // Конвертируем в байт
        byte b = 0;
        for (int j = 0; j < byteStr.length(); j++) {
            char c = byteStr.charAt(j);
            byte val = 0;
            if (c >= '0' && c <= '9') {
                val = c - '0';
            } else if (c >= 'A' && c <= 'F') {
                val = c - 'A' + 10;
            } else if (c >= 'a' && c <= 'f') {
                val = c - 'a' + 10;
            } else {
                return false;
            }
            b = b * 16 + val;
        }
        
        uid[i] = b;
    }
    
    return true;
}

String CommandHandler::formatUIDForCommand(byte* uid, byte uidSize) {
    String result = "";
    for (byte i = 0; i < uidSize; i++) {
        if (uid[i] < 0x10) {
            result += "0";
        }
        result += String(uid[i], HEX);
        result.toUpperCase();
        if (i < uidSize - 1) {
            result += " ";
        }
    }
    return result;
}

void CommandHandler::handleCommand(const String& command) {
    String cmd = command;
    cmd.trim();
    cmd.toLowerCase();
    
    if (cmd == "get logs") {
        // Читаем и отправляем все логи
        File file = SD.open("logs.txt", FILE_READ);
        if (file) {
            while (file.available()) {
                String line = file.readStringUntil('\n');
                Serial.println(line);
            }
            file.close();
        } else {
            Serial.println("Logs file not found");
        }
        
    } else if (cmd == "get ids") {
        // Получаем все ID
        String ids = idStorage->getAllIDs();
        Serial.print(ids);
        
    } else if (cmd.startsWith("put ")) {
        // Извлекаем ID из команды
        String idStr = cmd.substring(4);
        byte uid[10];
        byte uidSize;
        
        if (parseCardID(idStr, uid, uidSize)) {
            if (idStorage->addID(uid, uidSize)) {
                Serial.println("OK");
            } else {
                Serial.println("ERROR: ID already exists");
            }
        } else {
            Serial.println("ERROR: Invalid ID format");
        }
        
    } else if (cmd.startsWith("remove ")) {
        // Извлекаем ID из команды
        String idStr = cmd.substring(7);
        byte uid[10];
        byte uidSize;
        
        if (parseCardID(idStr, uid, uidSize)) {
            if (idStorage->removeID(uid, uidSize)) {
                Serial.println("OK");
            } else {
                Serial.println("ERROR: ID not found");
            }
        } else {
            Serial.println("ERROR: Invalid ID format");
        }
        
    } else if (cmd == "clear logs") {
        // Очищаем логи
        SD.remove("logs.txt");
        Serial.println("OK");
        
    } else if (cmd == "clear ids") {
        // Очищаем ID
        idStorage->clearAll();
        Serial.println("OK");
        
    } else if (cmd == "serial enable") {
        // Включаем логирование в Serial
        logger->setSerialLogging(true);
        Serial.println("OK");
        
    } else if (cmd == "serial disable") {
        // Выключаем логирование в Serial
        logger->setSerialLogging(false);
        Serial.println("OK");
        
    } else {
        Serial.println("ERROR: Unknown command");
    }
}

