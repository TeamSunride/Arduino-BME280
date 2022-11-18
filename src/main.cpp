///*
#include <Arduino.h>
#include <BME280.h>
//#include <BME280_registers.h>
#include <BME280_constants.h>
// use pio run -t upload -t monitor
#define SDA_pin 18
#define SCL_pin 16
#define CS_pin 10
BME280 BME(CS_pin, SPI, 4000000); //ik ik stop bugging me
//BME280 BME(&Wire, 1000000); // i2c protocol constructor
int LED = 13;

void setup() {
    pinMode(LED, OUTPUT);
    Serial.begin(115200);
    while (!Serial) {}
    delay(2);
    BME.begin();
    delay(2);
    BME.software_reset();
    BME.default_configuration();

    if (BME.who_am_i()){
        Serial.println("BME280 found!");
    }
    else{
        uint8_t id = BME.showChipID();
        Serial.printf("%d", id);
        //Serial.println("BME280 not found, check the wiring.");
        //Serial.println("Protocol stopped, please restart");
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

// THIS RETURNS A SENSOR NOT FOUND LINE
/*
#include <Arduino.h>
#include <BME280.h>
#include <BME280_registers.h>

#define SDA_pin 18
#define SCL_pin 16
#define CS_pin 10
BME280 BME(CS_pin, SPI, 4000000); //ik ik stop bugging me
//BME280 BME(&Wire, 1000000); // i2c protocol constructor
int LED = 13;

void setup() {
    pinMode(LED, OUTPUT);
    delay(1000);
    Serial.begin(115200);
    delay(2);
    BME.begin();
    delay(2);
    BME.software_reset();
    BME.default_configuration();

    if (BME.who_am_i() == BME280_REGISTERS::id){
        Serial.println("BME280 found!");
    }
    else{
        Serial.println("BME280 not found, check the wiring.");
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
}*/