#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

struct DateTime {
    uint8_t day, month, hour, minute, second;
    uint16_t year;
};

class Logger {
private:
    const char* _filename;
    DateTime startTime;
    unsigned long startMillis;
    byte fmtDT(const DateTime& dt, char* buf);
    byte fmtID(byte* uid, byte size, char* buf);
    void getDT(DateTime& dt);
public:
    const char* filename;
    Logger(const char* fn);
    void requestDateTime();
    void log(byte* uid, byte size, bool allowed, bool entry, bool passed);
    void setSerialLogging(bool en);
};

#endif
