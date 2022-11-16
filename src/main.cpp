#include <Arduino.h>
#include <BME280.h>
#include <BME280_registers.h>

#define CS_pin//(s) some pins here
BME280 BME(CS_pin, SPI, 4000000); //ik ik stop bugging me

void setup() {
    delay(1000);
    Serial.begin(115200);

    if (BME.who_am_i() == BME280_REGISTERS::id){
        Serial.println("Sensor found!");
    }
    else{
        Serial.println("BME280 not found, check the wiring.");
        while(1);
    }

}

void loop() {
    float temp = BME.getTemperature();
    float press = BME.getPressure();
    float hum = BME.getHumidity();

    Serial.printf("Data: %f C, %f Pa, %f gm-3", temp, press, hum);
    Serial.println();
    delay(500);
}