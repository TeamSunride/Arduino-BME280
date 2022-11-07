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

    uint8_t software_reset();

    byte who_am_i();


    short ctrlhum(short osr = BME280_CONSTANTS::OSR_RATE_1);
    //short ctrlmeas(//some setup);

    // GETS

    uint32_t getTemperature();
    uint32_t getPressure();
    uint32_t getHumidity();
    // ENUMS

    enum Mode
    {
        Mode_Sleep  = 0,
        Mode_Forced = 1,
        Mode_Normal = 3 // out 11, return 00
    };




    };

};

#endif //ARDUINO_BME280_BME280_H