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

  // SPI initialization code
  initSPI(0b111, 0, 1);

  while(1) {
    setPrecision(12);
    float temp = getTemp();

    char currentTempStr[45];
    sprintf(currentTempStr, "The current temperature is: %.4f deg C\n", temp);
    printf(currentTempStr);
  }
}
