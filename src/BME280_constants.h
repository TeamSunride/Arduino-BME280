//
// Created by Max Bird on 19/10/2022.
// TeamSunride
//


#ifndef BME280_CONSTANTS_H
#define BME280_CONSTANTS_H

enum BME280_CONSTANTS {
    /*static const uint8_t TEMP_DIG_ADDR   = 0x88;
    static const uint8_t PRESS_DIG_ADDR  = 0x8E;
    static const uint8_t HUM_DIG_ADDR1   = 0xA1;
    static const uint8_t HUM_DIG_ADDR2   = 0xE1;*/

    static const uint8_t RESET_VALUE	         = 0xB6;
    static const uint8_t TEMP_DIG_LENGTH         = 6;
    static const uint8_t PRESS_DIG_LENGTH        = 18;
    static const uint8_t HUM_DIG_ADDR1_LENGTH    = 1;
    static const uint8_t HUM_DIG_ADDR2_LENGTH    = 7;
    static const uint8_t DIG_LENGTH              = 32;
    static const uint8_t SENSOR_DATA_LENGTH      = 8;

};
#endif //BME280_CONSTANTS_H