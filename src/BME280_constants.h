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
    chip_id                         = 0x60 // check against id reg

};

#endif //BME280_CONSTANTS_H