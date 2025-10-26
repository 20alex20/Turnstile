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
