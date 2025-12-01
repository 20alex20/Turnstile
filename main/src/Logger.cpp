#include "Logger.h"
#include <SD.h>

Logger::Logger(const char* fn) : filename(fn), startMillis(0) {
    startTime.day = 1;
    startTime.month = 1;
    startTime.year = 2025;
    startTime.hour = 0;
    startTime.minute = 0;
    startTime.second = 0;
}

void Logger::requestDateTime() {
    Serial.println(F("Enter date/time (DD.MM.YYYY HH:MM:SS):\n> "));
    while (!Serial.available()) delay(10);
    char buf[30];
    byte idx = 0;
    while (Serial.available() && idx < 29) {
        char c = Serial.read();
        if (c == '\n' || c == '\r') break;
        buf[idx++] = c;
    }
    buf[idx] = 0;
    if (idx >= 19) {
        startTime.day = (buf[0] - '0') * 10 + (buf[1] - '0');
        startTime.month = (buf[3] - '0') * 10 + (buf[4] - '0');
        startTime.year = (buf[6] - '0') * 1000 + (buf[7] - '0') * 100 + (buf[8] - '0') * 10 + (buf[9] - '0');
        startTime.hour = (buf[11] - '0') * 10 + (buf[12] - '0');
        startTime.minute = (buf[14] - '0') * 10 + (buf[15] - '0');
        startTime.second = (buf[17] - '0') * 10 + (buf[18] - '0');
    }
    startMillis = millis();
}

void Logger::getDT(DateTime& dt) {
    unsigned long e = (millis() - startMillis) / 1000;
    dt = startTime;
    dt.second += e % 60;
    if (dt.second >= 60) {
        dt.second -= 60;
        e += 60;
    }
    e /= 60;
    dt.minute += e % 60;
    if (dt.minute >= 60) {
        dt.minute -= 60;
        e += 60;
    }
    e /= 60;
    dt.hour += e % 24;
    if (dt.hour >= 24) {
        dt.hour -= 24;
        e += 24;
    }
    e /= 24;
    if (e == 0) return;
    uint8_t d[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    while (e > 0) {
        uint8_t max = d[dt.month - 1];
        if (dt.month == 2 && (dt.year % 4 == 0 && (dt.year % 100 != 0 || dt.year % 400 == 0))) max = 29;
        if (dt.day + e > max) {
            e -= max - (dt.day - 1);
            dt.day = 1;
            dt.month++;
            if (dt.month > 12) {
                dt.month = 1;
                dt.year++;
            }
        } else {
            dt.day += e;
            e = 0;
        }
    }
}

byte Logger::fmtDT(const DateTime& dt, char* buf) {
    byte p = 0;
    buf[p++] = '0' + (dt.day / 10);
    buf[p++] = '0' + (dt.day % 10);
    buf[p++] = '.';
    buf[p++] = '0' + (dt.month / 10);
    buf[p++] = '0' + (dt.month % 10);
    buf[p++] = '.';
    uint16_t y = dt.year;
    buf[p++] = '0' + (y / 1000);
    buf[p++] = '0' + ((y / 100) % 10);
    buf[p++] = '0' + ((y / 10) % 10);
    buf[p++] = '0' + (y % 10);
    buf[p++] = ' ';
    buf[p++] = '0' + (dt.hour / 10);
    buf[p++] = '0' + (dt.hour % 10);
    buf[p++] = ':';
    buf[p++] = '0' + (dt.minute / 10);
    buf[p++] = '0' + (dt.minute % 10);
    buf[p++] = ':';
    buf[p++] = '0' + (dt.second / 10);
    buf[p++] = '0' + (dt.second % 10);
    buf[p] = 0;
    return p;
}

byte Logger::fmtID(byte* uid, byte size, char* buf) {
    byte p = 0;
    for (byte i = 0; i < size; i++) {
        byte b = uid[i];
        byte h = b >> 4;
        byte l = b & 0x0F;
        buf[p++] = (h < 10) ? ('0' + h) : ('A' + h - 10);
        buf[p++] = (l < 10) ? ('0' + l) : ('A' + l - 10);
        if (i < size - 1) buf[p++] = ' ';
    }
    buf[p] = 0;
    return p;
}

void Logger::log(byte* uid, byte size, bool allowed, bool entry, bool passed) {
    char buf[100];
    DateTime dt;
    getDT(dt);
    byte p = fmtDT(dt, buf);
    buf[p++] = ' ';
    p += fmtID(uid, size, buf + p);
    buf[p++] = ' ';
    buf[p++] = ':';
    buf[p++] = ' ';
    if (entry) {
        strcpy_P(buf + p, PSTR("Entrance "));
        p += 9;
    } else {
        strcpy_P(buf + p, PSTR("Exit "));
        p += 5;
    }
    if (allowed) {
        strcpy_P(buf + p, PSTR("is allowed"));
        p += 10;
        if (!passed) {
            strcpy_P(buf + p, PSTR(" NOT PASS"));
            p += 10;
        }
    } else {
        strcpy_P(buf + p, PSTR("is prohibited"));
        p += 13;
    }
    buf[p] = 0;
    File f = SD.open(filename, FILE_WRITE);
    if (f) {
        f.seek(f.size());
        f.println(buf);
        f.close();
    }
}
