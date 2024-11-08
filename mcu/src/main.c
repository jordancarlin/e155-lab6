/*
File: Lab_6_JC.c
Author: Jordan Carlin
Email: jcarlin@hmc.edu
Date: 11/4/24
*/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"

/////////////////////////////////////////////////////////////////
// Provided Constants and Functions
/////////////////////////////////////////////////////////////////

//Defining the web page in two chunks: everything before the current time, and everything after the current time
char* webpageStart = "<!DOCTYPE html><html><head><title>E155 Web Server Demo Webpage</title>\
	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
	</head>\
	<body><h1>E155 Web Server Demo Webpage</h1>";
char* ledStr = "<h1>LED</h1><p>LED Control:</p><form action=\"ledon\"><input type=\"submit\" value=\"Turn the LED on!\"></form>\
	<form action=\"ledoff\"><input type=\"submit\" value=\"Turn the LED off!\"></form>";
char* tempStr = "<h1>Temperature</h1><p>Temperature Precision:</p><form action=\"precision8\"><input type=\"submit\" value=\"8 bits\"></form>\
	<form action=\"precision9\"><input type=\"submit\" value=\"9 bits\"></form><form action=\"precision10\"><input type=\"submit\" value=\"10 bits\"></form>\
  <form action=\"precision11\"><input type=\"submit\" value=\"11 bits\"></form><form action=\"precision12\"><input type=\"submit\" value=\"12 bits\"></form>";
char* webpageEnd   = "</body></html>";

//determines whether a given character sequence is in a char array request, returning 1 if present, -1 if not present
int inString(char request[], char des[]) {
	if (strstr(request, des) != NULL) {return 1;}
	return -1;
}

int updateLEDStatus(char request[])
{
	int led_status = 0;
	// The request has been received. now process to determine whether to turn the LED on or off
	if (inString(request, "ledoff")==1) {
		digitalWrite(LED_PIN, PIO_LOW);
		led_status = 0;
	}
	else if (inString(request, "ledon")==1) {
		digitalWrite(LED_PIN, PIO_HIGH);
		led_status = 1;
	}

	return led_status;
}

void updatePrecision(char request[])
{
	int precision = 9;
	if (inString(request, "precision8")) {
		precision = 8;
	} else if (inString(request, "precision9")) {
		precision = 9;
	} else if (inString(request, "precision10")) {
		precision = 10;
	} else if (inString(request, "precision11")) {
		precision = 11;
	} else if (inString(request, "precision12")) {
		precision = 12;
	}
  setPrecision(precision);
  return;
}

/////////////////////////////////////////////////////////////////
// Solution Functions
/////////////////////////////////////////////////////////////////

int main(void) {
  configureFlash();
  configureClock();

  gpioEnable(GPIO_PORT_A);
  gpioEnable(GPIO_PORT_B);
  gpioEnable(GPIO_PORT_C);

  pinMode(LED_PIN, GPIO_OUTPUT);

  RCC->APB2ENR |= (RCC_APB2ENR_TIM15EN);
  initTIM(TIM15);

  USART_TypeDef * USART = initUSART(USART1_ID, 125000);

  // SPI initialization code
  initSPI(0b111, 0, 1);

  while(1) {
    //char test = spiSendReceive(0x25);
    /* Wait for ESP8266 to send a request.
    Requests take the form of '/REQ:<tag>\n', with TAG begin <= 10 characters.
    Therefore the request[] array must be able to contain 18 characters.
    */

    // Receive web request from the ESP
    char request[BUFF_LEN] = "                  "; // initialize to known value
    int charIndex = 0;

    // Keep going until you get end of line character
    //while(inString(request, "\n") == -1) {
    // // Wait for a complete request to be transmitted before processing
    // while(!(USART->ISR & USART_ISR_RXNE));
    // request[charIndex++] = readChar(USART);
    //}

    // update temp sensor precision
    // updatePrecision(request);
    setPrecision(8);

    // reading temperature
    float temp = getTemp();

    char currentTempStr[45];
    sprintf(currentTempStr, "The current temperature is: %f &degC\n", temp);
    printf(currentTempStr);

    // Update string with current LED state
    int led_status = updateLEDStatus(request);

    char ledStatusStr[20];
    if (led_status == 1)
      sprintf(ledStatusStr,"LED is on!");
    else if (led_status == 0)
      sprintf(ledStatusStr,"LED is off!");

    // finally, transmit the webpage over UART
    sendString(USART, webpageStart); // webpage header code

    // LED
    sendString(USART, ledStr);
    sendString(USART, "<p>");
    sendString(USART, ledStatusStr);
    sendString(USART, "</p>");

    // Temperature
    sendString(USART, tempStr);
    sendString(USART, "<p>");
    // sendString(USART, currentTempStr);
    sendString(USART, "</p>");

    sendString(USART, webpageEnd);
  }
}
