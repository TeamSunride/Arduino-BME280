//
// Created by Max Bird on 18/10/2022. Second rendition at main header file as previous one was a car crash
// TeamSunride
// C++ please have mercy

#ifndef ARDUINO_BME280_BME280_H
#define ARDUINO_BME280_BME280_H

#ifndef BME280_H
#define BME280_H

#include "protocol.h"
#include <Wire.h>
#include <SPI.h>
#include "BME280_registers.h"

#define DEBUG Serial.printf("%s %d\n", __FILE__, __LINE__)

struct BME280_SENSOR_MEASUREMENTS {
protected:

    protocol *device;

    /*float CalculateTemperature(
            int32_t raw,
            int32_t& t_fine,
            TempUnit unit = TempUnit_Celsius);

    /////////////////////////////////////////////////////////////////
    /// Calculate the humidity from the BME280 raw data and BME280
    /// trim, return a float.
    float CalculateHumidity(
            int32_t raw,
            int32_t t_fine);

    /////////////////////////////////////////////////////////////////
    /// Calculate the pressure from the BME280 raw data and BME280
    /// trim, return a float.
    float CalculatePressure(
            int32_t raw,
            int32_t t_fine,
            PresUnit unit = PresUnit_hPa);*/

public:
    // MEASUREABLES
    float temperature;
    float humidity;
    float pressure;
    // Settings, many
    /*enum Oversampling : uint8_t {
        OVERSAMPLING_1X = BME280_OVERSAMPLING_1X,
        OVERSAMPLING_2X = BME280_OVERSAMPLING_2X,
        OVERSAMPLING_4X = BME280_OVERSAMPLING_4X,
        OVERSAMPLING_8X = BME280_OVERSAMPLING_8X,
        OVERSAMPLING_16X = BME280_OVERSAMPLING_16X
    };
    enum FilterCoef : uint8_t {
        FILTER_COEF_OFF = BME280_FILTER_COEFF_OFF,
        FILTER_COEF_2 = BME280_FILTER_COEFF_2,
        FILTER_COEF_4 = BME280_FILTER_COEFF_4,
        FILTER_COEF_8 = BME280_FILTER_COEFF_8,
        FILTER_COEF_16 = BME280_FILTER_COEFF_16
    };
    enum StandbyTime : uint8_t {
        STANDBY_TIME_0_5_MS = BME280_STANDBY_TIME_0_5_MS,
        STANDBY_TIME_62_5_MS = BME280_STANDBY_TIME_62_5_MS,
        STANDBY_TIME_125_MS = BME280_STANDBY_TIME_125_MS,
        STANDBY_TIME_250_MS = BME280_STANDBY_TIME_250_MS,
        STANDBY_TIME_500_MS = BME280_STANDBY_TIME_500_MS,
        STANDBY_TIME_1000_MS = BME280_STANDBY_TIME_1000_MS,
        STANDBY_TIME_10_MS = BME280_STANDBY_TIME_10_MS,
        STANDBY_TIME_20_MS = BME280_STANDBY_TIME_20_MS
    };*/
    enum Mode {
        Mode_Sleep = 0,
        Mode_Forced = 1,
        Mode_Normal = 3
    };
};


    class BME280 {
    public:

        BME280(TwoWire *pipe, uint32_t freq);


        BME280(byte chipSelect, SPIClass &spi, uint freq);

        void begin() {
            device->protocol_begin();
            // any other set up etc
        }

        byte who_am_i();

        uint8_t ctrl_status(bool onoroff);

        /*uint8_t enable_temp_sensor(bool enable);

        uint8_t enable_hum_sensor(bool enable);

        uint8_t enable_press_sensor(bool enable);

        uint8_t set_operating_mode(BME280_OPERATING_MODE Mode);*/
        byte software_reset();

        bool ReadTrim();

        bool ReadData(
                int32_t data[8]);
    }