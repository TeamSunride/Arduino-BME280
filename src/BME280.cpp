//
// Created by Max Bird on 17/10/2022.
//
#include "BME280.h"
#include "BME280_constants.h"

//----------------------------------------------COMMUNICATIONS PROTOCOLS-----------------------------------------------
BME280::BME280(TwoWire *pipe, uint32_t freq) { // constructor for I2C protocol

    device = new I2CProtocol(BME280_DEFAULT_I2C_ADDRESS, pipe, freq);

}
BME280::BME280(byte chipSelect, SPIClass& spi, uint freq) { // constructor for SPI protocol

    SPISettings settings = SPISettings(freq, MSBFIRST, SPI_MODE3);
    device = new SPIProtocol(chipSelect, spi, settings, READ_BYTE, WRITE_BYTE);

}
//----------------------------------------------a BUNCH of FUNCTIONS---------------------------------------------------

byte BME280::software_reset() {
    byte data = device->read_reg(BME280_REGISTERs::reset);
    return device->write_reg(BME280_REGISTERs::reset); // or reset sensor somehow
}

byte BME280::who_am_i() {
    return byte data = device->read_reg(BME280_REGISTERS::id);
}


uint8_t BME280::default_configuration() {
    software_reset();
    //sensors off
    //etc
}

//======================================================================================================================
//Configuring data
bool BME280::ReadTrim()
{
    uint8_t ord(0);
    bool success = true;

    // Temp. Dig
    success &= ReadRegister(TEMP_DIG_ADDR, &m_dig[ord], TEMP_DIG_LENGTH);
    ord += TEMP_DIG_LENGTH;

    // Pressure Dig
    success &= ReadRegister(PRESS_DIG_ADDR, &m_dig[ord], PRESS_DIG_LENGTH);
    ord += PRESS_DIG_LENGTH;

    // Humidity Dig 1
    success &= ReadRegister(HUM_DIG_ADDR1, &m_dig[ord], HUM_DIG_ADDR1_LENGTH);
    ord += HUM_DIG_ADDR1_LENGTH;

    // Humidity Dig 2
    success &= ReadRegister(HUM_DIG_ADDR2, &m_dig[ord], HUM_DIG_ADDR2_LENGTH);
    ord += HUM_DIG_ADDR2_LENGTH;

#ifdef DEBUG_ON
    Serial.print("Dig: ");
   for(int i = 0; i < 32; ++i)
   {
      Serial.print(m_dig[i], HEX);
      Serial.print(" ");
   }
   Serial.println();
#endif

    return success && ord == DIG_LENGTH;
}

bool BME280::ReadData
        (
                int32_t data[SENSOR_DATA_LENGTH]
        )
{
    bool success;
    uint8_t buffer[SENSOR_DATA_LENGTH];

    // For forced mode we need to write the mode to BME280 register before reading
    if (BME280_SENSOR_MEASUREMENTS.mode == Mode_Forced)
    {
        WriteSettings();
    }

    // Registers are in order. So we can start at the pressure register and read 8 bytes.
    success = ReadRegister(PRESS_ADDR, buffer, SENSOR_DATA_LENGTH);

    for(int i = 0; i < SENSOR_DATA_LENGTH; ++i)
    {
        data[i] = static_cast<int32_t>(buffer[i]);
    }

///==============================================================================================================

/*float BME280::CalculateHumidity(int32_t raw, int32_t t_fine)                   //copied from https://github.com/finitespace/BME280/blob/master/src/BME280.cpp

{
    // Code based on calibration algorthim provided by Bosch.
    int32_t var1;
    uint8_t   dig_H1 =   m_dig[24];
    int16_t dig_H2 = (m_dig[26] << 8) | m_dig[25];
    uint8_t   dig_H3 =   m_dig[27];
    int16_t dig_H4 = ((int8_t)m_dig[28] * 16) | (0x0F & m_dig[29]);
    int16_t dig_H5 = ((int8_t)m_dig[30] * 16) | ((m_dig[29] >> 4) & 0x0F);
    int8_t   dig_H6 =   m_dig[31];

    var1 = (t_fine - ((int32_t)76800));
    var1 = (((((raw << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * var1)) +
              ((int32_t)16384)) >> 15) * (((((((var1 * ((int32_t)dig_H6)) >> 10) * (((var1 *
                                                                                      ((int32_t)dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
                                           ((int32_t)dig_H2) + 8192) >> 14));
    var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) * ((int32_t)dig_H1)) >> 4));
    var1 = (var1 < 0 ? 0 : var1);
    var1 = (var1 > 419430400 ? 419430400 : var1);

    return ((uint32_t)(var1 >> 12))/1024.0;
}

float BME280::CalculateTemperature              //copied from https://github.com/finitespace/BME280/blob/master/src/BME280.cpp
        (
                int32_t raw,
                int32_t& t_fine,
                TempUnit unit
        )
{
    // Code based on calibration algorthim provided by Bosch.
    int32_t var1, var2, final;
    uint16_t dig_T1 = (m_dig[1] << 8) | m_dig[0];
    int16_t   dig_T2 = (m_dig[3] << 8) | m_dig[2];
    int16_t   dig_T3 = (m_dig[5] << 8) | m_dig[4];
    var1 = ((((raw >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    var2 = (((((raw >> 4) - ((int32_t)dig_T1)) * ((raw >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
    t_fine = var1 + var2;
    final = (t_fine * 5 + 128) >> 8;
    return unit == TempUnit_Celsius ? final/100.0 : final/100.0*9.0/5.0 + 32.0;
}

float BME280::CalculatePressure                  //copied from https://github.com/finitespace/BME280/blob/master/src/BME280.cpp
        (
                int32_t raw,
                int32_t t_fine,
                PresUnit unit
        )
{
    // Code based on calibration algorthim provided by Bosch.
    int64_t var1, var2, pressure;
    float final;

    uint16_t dig_P1 = (m_dig[7]   << 8) | m_dig[6];
    int16_t   dig_P2 = (m_dig[9]   << 8) | m_dig[8];
    int16_t   dig_P3 = (m_dig[11] << 8) | m_dig[10];
    int16_t   dig_P4 = (m_dig[13] << 8) | m_dig[12];
    int16_t   dig_P5 = (m_dig[15] << 8) | m_dig[14];
    int16_t   dig_P6 = (m_dig[17] << 8) | m_dig[16];
    int16_t   dig_P7 = (m_dig[19] << 8) | m_dig[18];
    int16_t   dig_P8 = (m_dig[21] << 8) | m_dig[20];
    int16_t   dig_P9 = (m_dig[23] << 8) | m_dig[22];

    var1 = (int64_t)t_fine - 128000;
    var2 = var1 * var1 * (int64_t)dig_P6;
    var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
    var2 = var2 + (((int64_t)dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) + ((var1 * (int64_t)dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dig_P1) >> 33;
    if (var1 == 0) { return NAN; }                                                         // Don't divide by zero.
    pressure   = 1048576 - raw;
    pressure = (((pressure << 31) - var2) * 3125)/var1;
    var1 = (((int64_t)dig_P9) * (pressure >> 13) * (pressure >> 13)) >> 25;
    var2 = (((int64_t)dig_P8) * pressure) >> 19;
    pressure = ((pressure + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);

    final = ((uint32_t)pressure)/256.0;
*/
    switch(unit){
        case PresUnit_hPa: /* hPa */
            final /= 100.0;
            break;
        case PresUnit_inHg: /* inHg */
            final /= 3386.3752577878;          /* final pa * 1inHg/3386.3752577878Pa */
            break;
        case PresUnit_atm: /* atm */
            final /= 101324.99766353; /* final pa * 1 atm/101324.99766353Pa */
            break;
        case PresUnit_bar: /* bar */
            final /= 100000.0;               /* final pa * 1 bar/100kPa */
            break;
        case PresUnit_mbar: /* mbar */
            final /= 100.0;               /* final pa * 1 bar/100Pa */
            break;
        case PresUnit_torr: /* torr */
            final /= 133.32236534674;            /* final pa * 1 torr/133.32236534674Pa */
            break;
        case PresUnit_psi: /* psi */
            final /= 6894.744825494;   /* final pa * 1psi/6894.744825494Pa */
            break;
        default: /* Pa (case: 0) */
            break;
    }
    return final;
}

