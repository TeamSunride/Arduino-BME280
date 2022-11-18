//
// Created by Max Bird on 19/10/2022.
// TeamSunride
//


#ifndef BME280_CONSTANTS_H
#define BME280_CONSTANTS_H

#define WRITE_BYTE 0b00000000
#define READ_BYTE 0b10000000

#define BME280_DEFAULT_I2C_ADDRESS 0x76

enum BME280_CONSTANTS {

    OSR_RATE_SKIPPED                = 000,
    OSR_RATE_1                      = 001,
    OSR_RATE_2                      = 010,
    OSR_RATE_4                      = 011,
    OSR_RATE_8                      = 100,
    OSR_RATE_16                     = 101, // others
    chip_id                         = 0x60 // from id reg

};



struct const_values{
public:
    static const uint8_t BME280_chip_ID           = 0x60;
    static const uint8_t TEMP_DIG_LENGTH         = 6;
    static const uint8_t PRESS_DIG_LENGTH        = 18;
    static const uint8_t HUM_DIG_ADDR1_LENGTH    = 1;
    static const uint8_t HUM_DIG_ADDR2_LENGTH    = 7;
    static const uint8_t DIG_LENGTH              = 32;
    static const uint8_t SENSOR_DATA_LENGTH      = 8;
};
#endif //BME280_CONSTANTS_H