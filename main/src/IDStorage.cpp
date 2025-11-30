#include "IDStorage.h"
#include <SD.h>

IDStorage::IDStorage(const char* fn, uint8_t ss) : filename(const_cast<char*>(fn)), ssPin(ss) {
}

bool IDStorage::init() {
    return SD.begin(ssPin);
}

bool IDStorage::isIDAllowed(byte* uid, byte uidSize) {
    File file = SD.open(filename, FILE_READ);
    if (!file) {
        return false;
    }
    
    while (file.available()) {
        // Читаем размер следующего ID
        byte idSize = file.read();
        
        if (idSize == 0 || idSize > 10) {
            // Некорректный размер
            break;
        }
        
        // Читаем сам ID
        byte fileID[10];
        for (byte i = 0; i < idSize; i++) {
            if (!file.available()) {
                file.close();
                return false;
            }
            fileID[i] = file.read();
        }
        
        // Проверяем, совпадает ли ID
        if (idSize == uidSize && compareIDs(fileID, uid, idSize)) {
            file.close();
            return true;
        }
    }
    
    file.close();
    return false;
}

bool IDStorage::compareIDs(const byte* id1, const byte* id2, byte size) {
    for (byte i = 0; i < size; i++) {
        if (id1[i] != id2[i]) {
            return false;
        }
    }
    return true;
}

bool IDStorage::addID(byte* uid, byte uidSize) {
    if (isIDAllowed(uid, uidSize)) {
        return false; // ID уже существует
    }
    
    File file = SD.open(filename, FILE_WRITE);
    if (!file) {
        return false;
    }
    
    file.seek(file.size());
    file.write(uidSize);
    file.write(uid, uidSize);
    file.close();
    return true;
}

void IDStorage::printAllIDs() {
    File f = SD.open(filename, FILE_READ);
    if (!f) return;
    char buf[30];
    while (f.available()) {
        byte idSize = f.read();
        if (idSize == 0 || idSize > 10) break;
        byte fileID[10];
        for (byte i = 0; i < idSize; i++) {
            if (!f.available()) {
                f.close();
                return;
            }
            fileID[i] = f.read();
        }
        byte p = 0;
        for (byte i = 0; i < idSize; i++) {
            byte b = fileID[i];
            byte h = (b >> 4) & 0x0F;
            byte l = b & 0x0F;
            buf[p++] = (h < 10) ? ('0' + h) : ('A' + h - 10);
            buf[p++] = (l < 10) ? ('0' + l) : ('A' + l - 10);
            if (i < idSize - 1) buf[p++] = ' ';
        }
        buf[p] = 0;
        Serial.println(buf);
    }
    f.close();
}

bool IDStorage::parseCardID(const char* str, byte* uid, byte& size) {
    size = 1;
    const char* p = str;
    while (*p) {
        if (*p++ == ' ') size++;
    }
    if (size == 0 || size > 10) return false;
    p = str;
    for (byte i = 0; i < size; i++) {
        byte b = 0;
        byte digits = 0;
        while (*p && *p != ' ') {
            char c = *p++;
            byte val = 0;
            if (c >= '0' && c <= '9') val = c - '0';
            else if (c >= 'A' && c <= 'F') val = c - 'A' + 10;
            else if (c >= 'a' && c <= 'f') val = c - 'a' + 10;
            else return false;
            b = b * 16 + val;
            digits++;
        }
        if (digits == 0 || digits > 2) return false;
        uid[i] = b;
        if (*p == ' ') p++;
    }
    return true;
}

void IDStorage::handleCommand(const char* cmd, IDStorage* s, Logger* l) {
    if (strcmp_P(cmd, PSTR("get logs")) == 0) {
        File f = SD.open(l->filename, FILE_READ);
        if (f) {
            char buf[100];
            while (f.available()) {
                int len = f.readBytesUntil('\n', buf, 99);
                buf[len] = 0;
                Serial.println(buf);
            }
            f.close();
        } else {
            Serial.println(F("Logs file not found"));
        }
    } else if (strcmp_P(cmd, PSTR("get ids")) == 0) {
        s->printAllIDs();
    } else if (strncmp_P(cmd, PSTR("put "), 4) == 0) {
        byte uid[10], size;
        if (parseCardID(cmd + 4, uid, size)) {
            Serial.println(s->addID(uid, size) ? F("OK") : F("ERROR: ID already exists"));
        } else {
            Serial.println(F("ERROR: Invalid ID format"));
        }
    } else if (strcmp_P(cmd, PSTR("clear logs")) == 0) {
        SD.remove(l->filename);
        Serial.println(F("OK"));
    } else if (strcmp_P(cmd, PSTR("clear ids")) == 0) {
        SD.remove(s->filename);
        Serial.println(F("OK"));
    } else {
        Serial.println(F("ERROR: Unknown command"));
    }
}
