#include <Arduino.h>
/*
  Radio usada APC 220
  RX 8
  TX 9

*/
#include <Ticker.h> //Sistema no bloqueante

#include "config.h"
#include "eeprom.h"
#include "radio.h"

Ticker radioTicker(radioInfo, 1000, 0, MILLIS);

void setup() {
	// inicializacion puerto serie// Dont put this on the stack:
	Serial.begin(9600);
	DUMPSLN("Cansat Station ZMS");
	// inicializacion pines
	pinMode(LED, OUTPUT);
	setupRadio();
	// barometroOK = setupBMP();
	// imuOK = setupIMU();
	setupDatos();		 // Ponemos los datos a valores iniciales.
	radioTicker.start(); // Inicializamos el evento no bloqueante
	eepromGet();
}

void loop() { radioTicker.update(); }
