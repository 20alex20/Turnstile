#include "IDStorage.h"
#include <SD.h>

IDStorage::IDStorage(const char* filename, uint8_t ssPin) {
    this->filename = const_cast<char*>(filename);
    this->ssPin = ssPin;
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
        
        if (idSize <= 0 || idSize > 10) {
            // Некорректный размер
            break;
        }
        
        // Читаем сам ID
        byte fileID[10];
        for (byte i = 0; i < idSize && i < 10; i++) {
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

bool IDStorage::removeID(byte* uid, byte uidSize) {
    File file = SD.open(filename, FILE_READ);
    if (!file) {
        return false;
    }
    
    String tempFileName = String(filename) + ".tmp";
    File tempFile = SD.open(tempFileName.c_str(), FILE_WRITE);
    if (!tempFile) {
        file.close();
        return false;
    }
    
    bool found = false;
    while (file.available()) {
        byte idSize = file.read();
        if (idSize <= 0 || idSize > 10) {
            break;
        }
        
        byte fileID[10];
        for (byte i = 0; i < idSize; i++) {
            if (!file.available()) {
                break;
            }
            fileID[i] = file.read();
        }
        
        if (!(idSize == uidSize && compareIDs(fileID, uid, idSize))) {
            tempFile.write(idSize);
            tempFile.write(fileID, idSize);
        } else {
            found = true;
        }
    }
    
    file.close();
    tempFile.close();

    if (found) {
        SD.remove(filename);

        File file = SD.open(filename, FILE_WRITE);
        if (!file) {
            return false;
        }

        File tempFile = SD.open(tempFileName.c_str(), FILE_READ);
        if (!tempFile) {
            file.close();
            return false;
        }

        // копируем
        while (tempFile.available()) {
            file.write(tempFile.read());
        }

        file.close();
        tempFile.close();
    }

    SD.remove(tempFileName.c_str());

    return found;
}

String IDStorage::getAllIDs() {
    String result = "";
    File file = SD.open(filename, FILE_READ);
    if (!file) {
        return result;
    }
    
    while (file.available()) {
        byte idSize = file.read();
        if (idSize <= 0 || idSize > 10) {
            break;
        }
        
        byte fileID[10];
        for (byte i = 0; i < idSize; i++) {
            if (!file.available()) {
                break;
            }
            fileID[i] = file.read();
        }
        
        // Форматируем ID
        for (byte i = 0; i < idSize; i++) {
            if (fileID[i] < 0x10) {
                result += "0";
            }
            String hex = String(fileID[i], HEX);
            hex.toUpperCase();
            result += hex;
            if (i < idSize - 1) {
                result += " ";
            }
        }
        result += "\n";
    }
    
    file.close();
    return result;
}

void IDStorage::clearAll() {
    SD.remove(filename);
}
