//
// Created by Max Bird on 17/10/2022.
//
#include "BME280.h"
#include "BME280_constants.h"
#include "BME280_registers.h"

byte BME280::who_am_i() {
    return byte data = device->read_reg(BME280_REGISTERS::id);
}

/*bool BME280::Initialize()
{
    bool success(true);

    success &= who_am_i();

    if(success)
    {
        success &= ReadTrim();

        if(m_settings.filter != Filter_Off)
        {
            InitializeFilter();
        }

        WriteSettings();
    }

    m_initialized = success;

    return m_initialized;
}*/

//---------------------------------------------Initialisation----------------------------------------------------------


/*void BME280::InitializeFilter()
{
    // Force an unfiltered measurement to populate the filter buffer.
    // This fixes a bug that causes the first read to always be 28.82 °C 81732.34 hPa.
    Filter filter = m_settings.filter;
    m_settings.filter = Filter_Off;

    WriteSettings();

    float dummy;
    read(dummy, dummy, dummy);

    m_settings.filter = filter;
}*/

bool BME280::get_status(){
    byte data = device->read_reg(BME280_REGISTERS::status);
    return getBit(data, 2);
}


//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------COMMUNICATIONS PROTOCOLS-----------------------------------------------
BME280::BME280(TwoWire *pipe, uint32_t freq) { // constructor for I2C protocol

    device = new I2CProtocol(BME280_DEFAULT_I2C_ADDRESS, pipe, freq);

}
BME280::BME280(byte chipSelect, SPIClass& spi, uint freq) { // constructor for SPI protocol

    SPISettings settings = SPISettings(freq, MSBFIRST, SPI_MODE3);
    device = new SPIProtocol(chipSelect, spi, settings, READ_BYTE, WRITE_BYTE);
}
//---------------------------------------------Sensor Initilisation---------------------------------------------------

uint8_t BME280::enable_temp_sensor(bool enable) {
    byte data = device->read_reg(BME280_REGISTERS::ctrl_meas);
    // do ctrl_hum first
}

uint8_t BME280::enable_press_sensor(bool enable){

}

uint8_t BME280::enable_hum_sensor(bool enable) {

}
//---------------------------------------------Configure data---------------------------------------------------------

float calcTemp(byte t_fine){

    BME280_S32_t var1, var2, T;
    var1 = ((((adc_T>>3) – ((BME280_S32_t)dig_T1<<1))) * ((BME280_S32_t)dig_T2)) >> 11;
    var2 = (((((adc_T>>4) – ((BME280_S32_t)dig_T1)) * ((adc_T>>4) – ((BME280_S32_t)dig_T1)))
    >> 12) *
            ((BME280_S32_t)dig_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}
}
//---------------------------------------------Get data---------------------------------------------------------------

uint32_t getTemperature(){
    short msb = device->read_reg(BME280_REGISTERS::temp_msb);
    short lsb = device->read_reg(BME280_REGISTERS::temp_lsb);
    short xlsb = device->read_reg(BME280_REGISTERS::temp_xlsb);


}
