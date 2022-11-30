///*
#include <Arduino.h>
#include "BME280.h"

// use pio run -t upload -t monitor

#define CS_pin 10
//BME280 BME(CS_pin, SPI, 4000000); //ik ik stop bugging me
BME280 BME(&Wire, 1000000); // i2c protocol constructor
int LED = 13;

void setup() {
    pinMode(LED, OUTPUT);
    Serial.begin(115200);
    while (!Serial) {}
    delay(2);
    BME.begin();
    Serial.println("1");            // These are for debugging the startup process
    delay(2);
    BME.software_reset();
    Serial.println("2");
    delay(2);
    BME.default_configuration();
    Serial.println("3");

    //uint8_t chipID = const_values::BME280_chip_ID;
    Serial.println("4");
    if (BME.am_i_BME280()){
        Serial.println("BME280 found!");
    }
    else{
        Serial.println("5");
        uint8_t id = BME.who_am_i();
        Serial.printf("%X\n", id);
        Serial.println("BME280 not found :(");
        Serial.println("Protocol stopped, please restart");
        while(1); //endless loop
    }

}

void loop() {
    digitalWrite(LED, HIGH);            // LED on indicates measurement started
    float temp = BME.getTemperature();
    float press = BME.getPressure();
    float hum = BME.getHumidity();

    Serial.printf("Data: %f C, %f Pa, %f gm-3", temp, press, hum);
    Serial.println();
    digitalWrite(LED, LOW);             // LED off indicates measurement ended
    delay(500);
}
