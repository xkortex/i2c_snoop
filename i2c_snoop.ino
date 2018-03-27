#include <Arduino.h>
#include <Wire.h>
#include "i2c_snoop.h"
#include "constants.h"
#include "uart_io.h"

int data;

void setup() {
    setup_pins();
    Wire.begin();
    Wire.setClock(I2C_CLOCK);
    io.begin(BAUD);

    delay(500);
    led_off();
}

void loop() {
    data = 0;
    if (Wire.available()) {
        data = Wire.read();
    }
    if (data) {
        
    }
}

void setup_pins() {
    pinMode(P_RED, OUTPUT);
    digitalWrite(P_RED, HIGH);
    pinMode(P_GRN, OUTPUT);
    digitalWrite(P_GRN, HIGH);
    pinMode(P_BLU, OUTPUT);
    digitalWrite(P_BLU, HIGH);
}

void led_off() {
    digitalWrite(P_RED, LOW);
    digitalWrite(P_GRN, LOW);
    digitalWrite(P_BLU, LOW);
}