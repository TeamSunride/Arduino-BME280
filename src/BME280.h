//
// Created by Max Bird on 18/10/2022. Second rendition at main header file as previous one was a car crash
// TeamSunride
// C++ please have mercy

#ifndef ARDUINO_BME280_BME280_H
#define ARDUINO_BME280_BME280_H

#include "protocol.h"
#include <Wire.h>
#include <SPI.h>
#include "BME280_registers.h"
#include "BME280_constants.h"

#define DEBUG Serial.printf("%s %d\n", __FILE__, __LINE__)

class BME280{
protected:
    protocol *device;

public:
    /**
     * @brief Constructor overload for I2C protocol
     * @param pipe
     * @param freq
     */
    BME280(TwoWire *pipe, uint32_t freq);

    /**
     * @brief Constructor overload for SPI protocol
     * @param chipSelect
     * @param spi
     * @param settings
     */
    BME280(byte chipSelect, SPIClass& spi, uint freq);

    /**
     * @brief begin the device
     */
    void begin() {
        device->protocol_begin();
        // any other set up etc
    }


    // SENSOR ENABLES

    uint8_t enable_temp_sensor(bool enable);
    uint8_t enable_press_sensor(bool enable);
    uint8_t enable_hum_sensor(bool enable);

    // ENUMS

    enum Mode
    {
        Mode_Sleep  = 0,
        Mode_Forced = 1,
        Mode_Normal = 3
    };

    enum TempUnit
    {
        TempUnit_Celsius,
        TempUnit_Fahrenheit
    };

    // More bools

    uint8_t software_reset();

    byte who_am_i();

    bool get_status();
};

#endif //ARDUINO_BME280_BME280_H