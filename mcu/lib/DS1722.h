// DS1722.h
// Jordan Carlin
// jcarlin@hmc.edu
// 11/5/2024
// header for DS1722 temperature sensor

#ifndef DS1722_H
#define DS1722_H

#include <stdint.h>
#include "STM32L432KC_SPI.h"

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

// void initTemp();

// return current temperature
float getTemp();

// set precision of temperature sensor (8-12 bits)
void setPrecision(int precision);

#endif