// STM32L432KC_SPI.h
// Jordan Carlin
// jcarlin@hmc.edu
// 11/4/2024
// source code for SPI functions

#include "STM32L432KC_SPI.h"
#include "STM32L432KC_RCC.h"
#include "STM32L432KC_GPIO.h"

/* Enables the SPI peripheral and initializes its clock speed (baud rate), polarity, and phase.
 *    -- br: (0b000 - 0b111). The SPI clk will be the master clock / 2^(BR+1).
 *    -- cpol: clock polarity (0: inactive state is logical 0, 1: inactive state is logical 1).
 *    -- cpha: clock phase (0: data captured on leading edge of clk and changed on next edge,
 *          1: data changed on leading edge of clk and captured on next edge)
 * Refer to the datasheet for more low-level details. */
void initSPI(int br, int cpol, int cpha){
  // enable GPIO pins
  gpioEnable(GPIO_PORT_A);
  gpioEnable(GPIO_PORT_B);
  gpioEnable(GPIO_PORT_C);

  pinMode(SPI_CE, GPIO_OUTPUT);
  pinMode(SPI_SCK, GPIO_ALT);
  pinMode(SPI_MOSI, GPIO_ALT);
  pinMode(SPI_MISO, GPIO_ALT);
  GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL3, 5);
  GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL4, 5);
  GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL5, 5);

  GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED3); // faster GPIO pins needed for SPI

  // Enable SPI system clock
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

  // Configure SPI
  // CR1 configuration
  // set baud rate
  SPI1->CR1 |= _VAL2FLD(SPI_CR1_BR, br);
  // SPI1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA | SPI_CR1_LSBFIRST | SPI_CR1_SSM);
  // set clock polarity
  SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPOL, cpol);
  // set clock phase
  SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPHA, cpha);
  // Leave simplex/duplex at default for now
  // Leave frame format at default
  // Don't set crc integrity checking
  // Leave SSM and SSI at default
  // set as SPI controller
  SPI1->CR1 |= (SPI_CR1_MSTR);

  // CR2 configuration
  // set to 8-bit packets
  SPI1->CR2 |= _VAL2FLD(SPI_CR2_DS, 0b0111);

  // set FIFO full length to 8
  SPI1->CR2 |= (SPI_CR2_FRXTH);

  // Enable peripheral select mode
  SPI1->CR2 |= (SPI_CR2_SSOE);

  // Enable SPI system
  SPI1->CR1 |= (SPI_CR1_SPE);

}

/* Transmits a character (1 byte) over SPI and returns the received character.
 *    -- send: the character to send over SPI
 *    -- return: the character received over SPI */
char spiSendReceive(char send){
  while(!(SPI1->SR & SPI_SR_TXE));
  // write data to transmit FIFO. Use volatile to prevent compiler issues
  *(volatile char *) (&SPI1->DR) = send;
  // wait until receive FIFO is NOT empty (0 when empty)
  while(!(SPI1->SR & SPI_SR_RXNE));
  // read data from receive FIFO
  char receivedData = (volatile char) SPI1->DR;

  return receivedData;
  // digitalWrite(SPI_CE, 0);
}
