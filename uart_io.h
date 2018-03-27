//
// Created by mike on 2/19/18.
//

#ifndef UART_IO_H
#define UART_IO_H

#define S_BUFSIZE 256

#include <Arduino.h>
#include <bluefruit.h>
#include "constants.h"

///< V ble.h
void bluefruit_init(void);
void startAdv(void);


///< Buffer for parsing ble-uart stuff
extern uint8_t packetbuf[];
extern uint8_t outputbuf[];

// The actual ble_uart...thingy. It's a class, but the functions use pointers?
extern BLEUart bleuart;
///< ^ ble

typedef enum IoDescriptor {
    IO_DEFAULT=0, IO_SERIAL, IO_BLE
} IoDescriptor_t;

extern uint32_t G_loggingLevel;

class Logger {
public:
    Logger(void);
    void begin(long baud);
    void setTimeout(long timeoutMS);
    int write(uint8_t *buf, size_t len);
    int printf(const char *format, ...);
};


class GenericIO {
private:
    long timeout = 50;
public:
    GenericIO(void);
    // Stream API

//    virtual int       read       ( void );
//    virtual int       read       ( uint8_t * buf, size_t size );
//            int       read       ( char    * buf, size_t size ) { return read( (uint8_t*) buf, size); }
//    virtual size_t    write      ( uint8_t b );
//    virtual size_t    write      ( const uint8_t *content, size_t len );
//    virtual int       available  ( void );
//    virtual int       peek       ( void );
//    virtual void      flush      ( void );
    void begin(long baud);
    void setTimeout(long timeoutMS);
    // Stream API - the BLEuart uses size_t, but we will use int, because I doubt we will write more than 32k in a go!
    int write(uint8_t b);
    int write(uint8_t *buf, size_t len);
    int fwrite(IoDescriptor_t io, uint8_t b);
    int fwrite(IoDescriptor_t io, uint8_t *buf, size_t len);
    int printf(const char *format, ...);
    int fprintf(IoDescriptor_t io, const char *format, ...);
    char holdup();
    size_t readin(uint8_t *buf, size_t bufsize);

};

extern Logger logger;
extern GenericIO io;

#endif //UART_IO_H
