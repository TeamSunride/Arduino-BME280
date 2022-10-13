//
// Created by Max Bird on 12/10/2022.
//

#ifndef BME80_REGISTERS_H
#define BME80_REGISTERS_H


enum BME80_REGISTERS {
    hum_lsb             = 0xFE, // reset reg: 0x00
    hum_msb             = 0xFD, // reset reg: 0x80
    temp_xlsb           = 0xFC, // reset reg: 0x00
    temp_lsb            = 0xFB, // reset reg: 0x00
    temp_msb            = 0xFA, // reset reg: 0x80
    press_xlsb          = 0xF9, // reset reg: 0x00
    press_lsb           = 0xF8, // reset reg: 0x00
    press_msb           = 0xF7, // reset reg: 0x80
    config              = 0xF5, // reset reg: 0x00
    ctrl_meas           = 0xF4, // reset reg: 0x00
    status              = 0xF3, // reset reg: 0x00
    ctrl_hum            = 0xF2, // reset reg: 0x00
    // calib26..calib41    = 0xE1...0xF0, reset reg: 'individual'
    reset               = 0xE0, // reset reg: 0x00
    id                  = 0xD0, // reset reg: 0x60
    // calib00..calib25 = 0x88...0xA1 reset reg: 'individual'
};

#endif //BME80_REGISTERS_H

