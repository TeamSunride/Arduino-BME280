#include <Arduino.h>
#include <BME280.h>
#include <BME280_registers.h>

#define CS_pin//(s) some pins here
BME280 BME(CS_pin, SPI, 4000000); //ik ik stop bugging me

void setup() {
    delay(1000);
    Serial.begin(115200);

    if (BME.who_am_i() != (BME280_REGISTERS::id)){
        Serial.println("BME280 not found, check the wiring.");
        while(1);
    }

}

void loop() {
// write your code here
}