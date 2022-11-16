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

    void software_reset();

    byte who_am_i();

    void set_status(int pos, int val);

    void writeSettings();

    // GETS

    float getTemperature();
    float getPressure();
    float getHumidity();

    //COnfigures

    float configureTemp(int32_t raw_T, unsigned short dig_T1, short dig_T2, short dig_T3);


    // ENUMS

    enum OSR
    {
        OSR_Off =  0,
        OSR_X1  =  1,
        OSR_X2  =  2,
        OSR_X4  =  3,
        OSR_X8  =  4,
        OSR_X16 =  5
    };

    enum Mode
    {
        Mode_Sleep  = 0,
        Mode_Forced = 1,
        Mode_Normal = 3
    };

    enum StandbyTime
    {
        StandbyTime_500us   = 0,
        StandbyTime_62500us = 1,
        StandbyTime_125ms   = 2,
        StandbyTime_250ms   = 3,
        StandbyTime_50ms    = 4,
        StandbyTime_1000ms  = 5,
        StandbyTime_10ms    = 6,
        StandbyTime_20ms    = 7
    };

    enum Filter
    {
        Filter_Off = 0,
        Filter_2   = 1,
        Filter_4   = 2,
        Filter_8   = 3,
        Filter_16  = 4
    };

    enum SpiEnable
    {
        SpiEnable_False = 0,
        SpiEnable_True = 1
    };



};
#endif //ARDUINO_BME280_BME280_H