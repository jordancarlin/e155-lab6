// DS1722.c
// Jordan Carlin
// jcarlin@hmc.edu
// 11/5/2024
// source code for DS1722 temperature sensor

#include "DS1722.h"
// #include <string.h>

// void initTemp(){

// }

// return current temperature
float getTemp(){
  int32_t msb = spiSendReceive(0x01);
  uint32_t lsb = spiSendReceive(0x02);
  float lsbFloat = (float)lsb/256.0;
  float temp = msb + lsbFloat;
  return temp;
  // char tempBits[15] = strcat(msb, lsb);
  // int temp = -128*(int)msb[7] + 64*(int)msb[6] + 32*(int)msb[5] + 16*(int)msb[4] + 8*(int)msb[3] \
  //               + 4*(int)msb[2] + 2*(int)msb[1] + (int)msb[0] + 0.5*(int)lsb[7] + 0.25*(int)lsb[6] \
  //               + 0.125*(int)lsb[5] + 0.0625*(int)lsb[4];
}

// set precision of temperature sensor (8-12 bits)
void setPrecision(int precision){
  char configByte;
  if (precision == 8)  configByte = 0b00000001;
  if (precision == 9)  configByte = 0b00000011;
  if (precision == 10) configByte = 0b00000101;
  if (precision == 11) configByte = 0b00000111;
  if (precision == 12) configByte = 0b00001001;

  int R0, R1, R2;
  R0 = (precision % 2);
  R1 = (precision >= 10);
  R2 = (precision == 12);

  spiSendReceive(0x80);
  spiSendReceive(configByte);
}
