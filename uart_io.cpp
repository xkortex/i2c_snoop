//
// Created by mike on 2/19/18.
//

#include <stdio.h>
#include <stdarg.h>
#include <Arduino.h>
#include "uart_io.h"

// Create globals ===================================
Logger logger = Logger();
GenericIO io = GenericIO();

uint32_t G_loggingLevel = 1;

///< V ble.cpp
/* Buffer to hold incoming characters */
uint8_t packetbuf[BUFSIZE+1];
uint8_t outputbuf[BUFSIZE+1];

BLEUart bleuart = BLEUart(); // global, externed in ble.h
///< ^ ble.cpp

// ==============================================

///< V ble.cpp
void bluefruit_init(void) {
    Bluefruit.begin();    // Bluefruit module must be initialized for Nffs to work
    // Since Bluefruit's SOC event handling task is required for flash operation
    io.printf("Bluefruit.begin()\n");
    io.printf("Nffs.begin()\n");

    Bluefruit.setTxPower(4); // Set max power. Accepted values are: -40, -30, -20, -16, -12, -8, -4, 0, 4
    Bluefruit.setName(BLE_DEV_NAME);

    bleuart.begin();        // Configure and start the BLE Uart service

    startAdv();             // Roll the advertisement right into the initialization.

}

void startAdv(void)
{
    io.printf("Starting advert service\n");
    // Advertising packet
    Bluefruit.Advertising.addTxPower();

    // Include the BLE UART (AKA 'NUS') 128-bit UUID
    Bluefruit.Advertising.addService(bleuart);

    // Secondary Scan Response packet (optional)
    // Since there is no room for 'Name' in Advertising packet
    Bluefruit.ScanResponse.addName();

    /* Start Advertising
     * - Enable auto advertising if disconnected
     * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
     * - Timeout for fast mode is 30 seconds
     * - Start(timeout) with timeout = 0 will advertise forever (until connected)
     *
     * For recommended advertising interval
     * https://developer.apple.com/library/content/qa/qa1931/_index.html
     */
    Bluefruit.Advertising.restartOnDisconnect(true);
    Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
    Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
    Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
    io.printf("Advert service started\n" );
    //io.printf((int) &bleuart );
}

///< ^ ble.cpp
// ==================================== Logger ========================

Logger::Logger() {

}

void Logger::begin(long baud) {
    Serial.begin(baud);
}

void Logger::setTimeout(long timeoutMS) {
    Serial.setTimeout(timeoutMS);
}

int Logger::write(uint8_t *buf, size_t len) {
    Serial.write(buf, len);
}

int Logger::printf(const char *fmt, ...) {
    if (G_loggingLevel < 1) {
        return 0;
    }
    char buff[S_BUFSIZE];
    va_list args;
    va_start(args, fmt);
    int return_status = vsnprintf(buff, S_BUFSIZE, fmt, args);
    va_end(args);
    uint8_t *s = (uint8_t *)&buff;
    while (*s) Serial.write(*s++);
    return return_status;
}

// ================================ Generic IO =====================================

/**
 * Warning! Do not try to write more than INT_MAX bytes! Could do something weird!
 */
GenericIO::GenericIO() {

}

void GenericIO::begin(long baud) {
    Serial.begin(baud);
}

void GenericIO::setTimeout(long timeoutMS) {
    Serial.setTimeout(timeoutMS);
}

int GenericIO::write(uint8_t b) {
    Serial.write(b);
}

int GenericIO::write(uint8_t *buf, size_t len) {
    Serial.write(buf, len);
}

int GenericIO::fwrite(IoDescriptor_t io, uint8_t b) {
    int res = -1;
    switch (io) {
        case IO_BLE:
            res = (int) bleuart.write(b);
        case IO_DEFAULT:
        case IO_SERIAL:
        default:
            res = Serial.write(b);
            break;
    }
    return res;
}

int GenericIO::fwrite(IoDescriptor_t io, uint8_t *buf, size_t len) {
    int res = -1;
    switch (io) {
        case IO_BLE:
            res = (int) bleuart.write(buf, len);
        case IO_DEFAULT:
        case IO_SERIAL:
        default:
            res = Serial.write(buf, len);
            break;
    }
    return res;
}

int GenericIO::fprintf(IoDescriptor_t io, const char *fmt, ...) {
    char buff[S_BUFSIZE];
    va_list args;
    va_start(args, fmt);
    int return_status = vsnprintf(buff, S_BUFSIZE, fmt, args);
    va_end(args);
    uint8_t *s = (uint8_t *)&buff;
    while (*s) fwrite(io, *s++);
    return return_status;
}

int GenericIO::printf(const char *fmt, ...) {
    char buff[S_BUFSIZE];
    va_list args;
    va_start(args, fmt);
    int return_status = vsnprintf(buff, S_BUFSIZE, fmt, args);
    va_end(args);
    uint8_t *s = (uint8_t *)&buff;
    while (*s) Serial.write(*s++);
    return return_status;
}

char GenericIO::holdup() {
    Serial.print("Enter any character to continue:");
    while ( !Serial.available() )
    {
        delay(1);
    }
}

size_t GenericIO::readin(uint8_t *buf, size_t bufsize) {
    uint16_t timeout = BT_TIMEOUT_MS;
    uint16_t origtimeout = timeout;
    uint8_t idx = 0;

    memset(buf, 0, bufsize);
    while (timeout--) {
        while (Serial.available() > 0) {
            uint8_t c = Serial.read();
            buf[idx] = c;
            idx++;
            timeout = origtimeout;
            if (idx >= BUFSIZE) break;
        }

        if (timeout == 0) break;
        delay(1);
    }

    buf[idx] = 0; // null terminator
    return idx;
}

void GenericIO::print_hex(int x) {
    if (x & 0xFF00) {
        print_hex((uint8_t) ((x & 0xFF00) >> 8));
    }
    print_hex((uint8_t) (x & 0xFF));
}

void GenericIO::print_hex(uint8_t x) {
    if (hexCount == 0) {
        printf("\n");
    }
    printf("%02x ", x);
    hexCount++;
    hexCount &= (hexWidth - 1);
}