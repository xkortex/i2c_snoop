#include <Arduino.h>
#include <Wire.h>
#include "i2c_snoop.h"
#include "constants.h"
#include "uart_io.h"

void setup() {
    setup_pins();
    Wire.begin();
    Wire.setClock(I2C_CLOCK);
    io.begin(BAUD);
}

void loop() {

}

void setup_pins() {
    pinMode(P_RED, OUTPUT);
    digitalWrite(P_RED, HIGH);
    pinMode(P_GRN, OUTPUT);
    digitalWrite(P_GRN, HIGH);
    pinMode(P_BLU, OUTPUT);
    digitalWrite(P_BLU, HIGH);


}