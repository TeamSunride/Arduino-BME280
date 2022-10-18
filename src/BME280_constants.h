//
// Created by Max Bird on 19/10/2022.
// TeamSunride
//C++ please have mercy

static const uint8_t CTRL_HUM_ADDR   = 0xF2;
static const uint8_t CTRL_MEAS_ADDR  = 0xF4;
static const uint8_t CONFIG_ADDR     = 0xF5;
static const uint8_t PRESS_ADDR      = 0xF7;
static const uint8_t TEMP_ADDR       = 0xFA;
static const uint8_t HUM_ADDR        = 0xFD;
static const uint8_t TEMP_DIG_ADDR   = 0x88;
static const uint8_t PRESS_DIG_ADDR  = 0x8E;
static const uint8_t HUM_DIG_ADDR1   = 0xA1;
static const uint8_t HUM_DIG_ADDR2   = 0xE1;
static const uint8_t ID_ADDR         = 0xD0;
static const uint8_t RESET_ADDR	= 0xE0;

static const uint8_t RESET_VALUE	= 0xB6;

static const uint8_t TEMP_DIG_LENGTH         = 6;
static const uint8_t PRESS_DIG_LENGTH        = 18;
static const uint8_t HUM_DIG_ADDR1_LENGTH    = 1;
static const uint8_t HUM_DIG_ADDR2_LENGTH    = 7;
static const uint8_t DIG_LENGTH              = 32;
static const uint8_t SENSOR_DATA_LENGTH      = 8;

