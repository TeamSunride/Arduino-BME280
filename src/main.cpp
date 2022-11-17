#include <Arduino.h>
#include <BME280.h>
#include <BME280_registers.h>

#define SDA_pin 18
#define SCL_pin 16
#define CS_pin 10
BME280 BME(CS_pin, SPI, 4000000); //ik ik stop bugging me
//BME280 BME(&Wire, 1000000); // i2c protocol constructor

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