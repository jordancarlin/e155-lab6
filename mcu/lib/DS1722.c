// DS1722.c
// Jordan Carlin
// jcarlin@hmc.edu
// 11/5/2024
// source code for DS1722 temperature sensor

#include "DS1722.h"
#include <stdint.h>
#include <stdio.h>
#include "STM32L432KC_SPI.h"
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_TIM.h"

// void initTemp(){

// }

// return current temperature
float getTemp(){
  spiSendReceive(0x01); // read msb register
  int8_t msb = spiSendReceive(0x00); // send junk
  uint8_t test = spiSendReceive(0x02); // read lsb register
  uint8_t lsb = spiSendReceive(0x00); // send junk
  // char msbc = (char)msb;
  // char lsbc = (char)lsb;
  // printf("SPI DATA: ");
  // printf(msbc);
  // printf(lsbc);
  // printf("\n");
  printf("SPI DATA: %02X %02X\n", msb, lsb);
  float lsbFloat = (float)lsb/256.0;
  float temp = msb + lsbFloat;
  return temp;
}

// set precision of temperature sensor (8-12 bits)
void setPrecision(int precision){
  char configByte;
  if (precision == 8)  configByte = 0xF0;
  if (precision == 9)  configByte = 0xF2;
  if (precision == 10) configByte = 0xF4;
  if (precision == 11) configByte = 0xF5;
  if (precision == 12) configByte = 0xF8;

  spiSendReceive(0x80);
  spiSendReceive(configByte);
  delay_millis(TIM15, 2000);
}
