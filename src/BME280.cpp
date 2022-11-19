//
// Created by Max Bird on 17/10/2022.
//
#include "BME280.h"
#include "BME280_constants.h"
#include "BME280_registers.h"

//----------------------------------------------COMMUNICATIONS PROTOCOLS-----------------------------------------------
BME280::BME280(TwoWire *pipe, uint32_t freq) { // constructor for I2C protocol

    device = new I2CProtocol(BME280_DEFAULT_I2C_ADDRESS, pipe, freq);

}
BME280::BME280(byte chipSelect, SPIClass& spi, uint freq) { // constructor for SPI protocol

    SPISettings settings = SPISettings(freq, MSBFIRST, SPI_MODE3);
    device = new SPIProtocol(chipSelect, spi, settings, READ_BYTE, WRITE_BYTE);
}
//----------------------------------------------------------------------------------------------------------------------

byte BME280::read_reg(BME280_REGISTERS regAddress) {
    return device->read_reg(regAddress);
}

uint8_t BME280::write_reg(BME280_REGISTERS regAddress, byte data) {
    return device->write_reg(regAddress, data);
}

uint8_t BME280::who_am_i() {
    uint8_t data = device->read_reg(BME280_REGISTERS::id);
    return data;
}

bool BME280::am_i_BME280(){
    if (who_am_i() == const_values::BME280_chip_ID){
        return true;
    }
    else{
        return false;
    }
}

void BME280::software_reset() {
    //int current_mode = Mode::Mode_Sleep;
    device->write_reg(BME280_REGISTERS::reset, BME280_REGISTERS::RESET_VALUE);
}

//---------------------------------------------Initialisation----------------------------------------------------------

/*void BME280::InitializeFilter()
{
    // Force an unfiltered measurement to populate the filter buffer.
    // This fixes a bug that causes the first read to always be 28.82 °C 81732.34 hPa.
    int current_mode = BME280::Mode(Mode_Normal);

    writeSettings();

    float dummy;
    read(dummy, dummy, dummy);

    //BME280::defaultSettings().filter = filter;
}*/

void BME280::set_status(int pos, int val){
    byte data = device->read_reg(BME280_REGISTERS::status);
    setBit(&data, pos, val);
    device->write_reg(BME280_REGISTERS::status, data);
}


//----------------------------------------------------------------------------------------------------------------------



//---------------------------------------------Sensor Setup---------------------------------------------------

void calculateCtrlRegisters(uint8_t& ctrlHum, uint8_t& config, uint8_t& ctrlMeas){


    ctrlHum = (uint8_t)BME280::defaultSettings().humOSR;

    ctrlMeas = ((uint8_t)BME280::defaultSettings().tempOSR << 5) | ((uint8_t)BME280::defaultSettings().presOSR << 2) | (uint8_t)BME280::defaultSettings().mode;

    config = ((uint8_t)BME280::defaultSettings().standbyTime << 5) | ((uint8_t)BME280::defaultSettings().filter << 2) | (uint8_t)BME280::defaultSettings().spiEnable;

}

void BME280::writeSettings() {

    uint8_t ctrlHum, config, ctrlMeas;

    calculateCtrlRegisters(ctrlHum, config, ctrlMeas);

    device->write_reg(BME280_REGISTERS::ctrl_hum, ctrlHum);
    device->write_reg(BME280_REGISTERS::config, config);
    device->write_reg(BME280_REGISTERS::ctrl_meas, ctrlMeas);
}

void BME280::default_configuration(){
    writeSettings();
}

//---------------------------------------------Configure data---------------------------------------------------------

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
// t_fine carries fine temperature as global value
int32_t t_fine;
float configureTemp(int32_t raw_T, unsigned short dig_T1, short dig_T2, short dig_T3){

    int32_t var1, var2, T;

    var1 = ((((raw_T >>3) - ((int32_t)dig_T1<<1))) * ((int32_t)dig_T2)) >> 11;
    var2 = (((((raw_T >>4) - ((int32_t)dig_T1)) * ((raw_T >>4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;

    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    //set_status(3, 1);
    return T;
}

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
// Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa

float configurePress(int32_t adc_P, unsigned short dig_P1, short dig_P2, short dig_P3, short dig_P4, short dig_P5, short dig_P6, short dig_P7, short dig_P8, short dig_P9) {
    int64_t var1, var2, p;
    var1 = ((int64_t ) t_fine) - 128000;
    var2 = var1 * var1 * (int64_t ) dig_P6;
    var2 = var2 + ((var1 * (int64_t ) dig_P5) << 17);
    var2 = var2 + (((int64_t ) dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t ) dig_P3) >> 8) + ((var1 * (int64_t ) dig_P2) << 12);
    var1 = (((((int64_t ) 1) << 47) + var1)) * ((int64_t ) dig_P1) >> 33;
    if (var1 == 0) {
        return 0; // avoid exception caused by division by zero
    }
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t ) dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t ) dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7)<<4);
    return (uint32_t) p;
}

// Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional bits).
// Output value of “47445” represents 47445/1024 = 46.333 %RH
float configureHum(int32_t adc_H, unsigned char dig_H1, short dig_H2, unsigned short dig_H3, short dig_H4, short dig_H5, char dig_H6) {
    int32_t v_x1_u32r;
    v_x1_u32r = (t_fine - ((int32_t) 76800));

    v_x1_u32r = (((((adc_H << 14) - (((int32_t) dig_H4) << 20) - (((int32_t) dig_H5) *
                                                                       v_x1_u32r)) +((int32_t) 16384)) >> 15) *(
            ((((((v_x1_u32r *
                  ((int32_t) dig_H6)) >> 10) * (((v_x1_u32r * ((int32_t) dig_H3)) >> 11) +
                                                     ((int32_t) 32768))) >> 10) + ((int32_t) 2097152)) *
             ((int32_t) dig_H2) +
             8192) >> 14));
    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
                               ((int32_t) dig_H1)) >> 4));
    v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
    v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
    return (uint32_t)(v_x1_u32r >> 12);
}

//---------------------------------------------Get data---------------------------------------------------------------

float BME280::getTemperature(){
    uint8_t msb = device->read_reg(BME280_REGISTERS::temp_msb);
    uint8_t lsb = device->read_reg(BME280_REGISTERS::temp_lsb);
    uint8_t xlsb = device->read_reg(BME280_REGISTERS::temp_xlsb);

    uint32_t Temp;
    Temp = (msb << 12) | (lsb << 4) | (xlsb >> 4); // By Tom Danvers of Leicster

    unsigned short dig_T1 = device->read_reg(BME280_DIG_REGISTERS::T_DIG1);
    short dig_T2 = device->read_reg(BME280_DIG_REGISTERS::T_DIG2);
    short dig_T3 = device->read_reg(BME280_DIG_REGISTERS::T_DIG3);

    //set_status(3, 1);
    return configureTemp(Temp, dig_T1, dig_T2, dig_T3);
}

float BME280::getPressure(){
    uint8_t msb = device->read_reg(BME280_REGISTERS::press_msb);
    uint8_t lsb = device->read_reg(BME280_REGISTERS::press_lsb);
    uint8_t xlsb = device->read_reg(BME280_REGISTERS::press_xlsb);

    uint32_t Press;
    Press = (msb << 12) | (lsb << 4) | (xlsb >>4 );

    unsigned short dig_P1 = device->read_reg(BME280_DIG_REGISTERS::P_DIG1);
    short dig_P2 = device->read_reg(BME280_DIG_REGISTERS::P_DIG2);
    short dig_P3 = device->read_reg(BME280_DIG_REGISTERS::P_DIG3);
    short dig_P4 = device->read_reg(BME280_DIG_REGISTERS::P_DIG4);
    short dig_P5 = device->read_reg(BME280_DIG_REGISTERS::P_DIG5);
    short dig_P6 = device->read_reg(BME280_DIG_REGISTERS::P_DIG6);
    short dig_P7 = device->read_reg(BME280_DIG_REGISTERS::P_DIG7);
    short dig_P8 = device->read_reg(BME280_DIG_REGISTERS::P_DIG8);
    short dig_P9 = device->read_reg(BME280_DIG_REGISTERS::P_DIG9);


    return configurePress(Press, dig_P1, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9);
}

float BME280::getHumidity(){
    uint8_t msb = device->read_reg(BME280_REGISTERS::hum_msb);
    uint8_t lsb = device->read_reg(BME280_REGISTERS::hum_lsb);

    uint16_t Hum;
    Hum = (lsb << 8) | (msb >> 8);

    unsigned char dig_H1 = device->read_reg(BME280_DIG_REGISTERS::H_DIG1);
    short dig_H2 = device->read_reg(BME280_DIG_REGISTERS::H_DIG2);
    unsigned char dig_H3 = device->read_reg(BME280_DIG_REGISTERS::H_DIG3);
    short dig_H4 = device->read_reg(BME280_DIG_REGISTERS::H_DIG4);
    short dig_H5 = device->read_reg(BME280_DIG_REGISTERS::H_DIG5);
    char dig_H6 = device->read_reg(BME280_DIG_REGISTERS::H_DIG6);

    return configureHum(Hum, dig_H1, dig_H2, dig_H3, dig_H4, dig_H5, dig_H6);
}

