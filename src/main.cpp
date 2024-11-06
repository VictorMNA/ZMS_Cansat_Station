#include <Arduino.h>
#include <Ticker.h> //Sistema no bloqueante

#include "config.h"
#include "eeprom.h"
#include "radio.h"

Ticker sendRadioTicker(radioSendInfo, 1000, 0, MILLIS);
Ticker getRadioTicker(radioSendInfo, 1000, 0, MILLIS);

void setup() {
	// inicializacion puerto serie// Dont put this on the stack:
	Serial.begin(9600);
	DUMPSLN("Cansat Station ZMS");
	// inicializacion pines
	pinMode(LED, OUTPUT);
	setupRadio();
	setupDatos();			 // Ponemos los datos a valores iniciales.
	sendRadioTicker.start(); // Inicializamos el evento no bloqueante
	getRadioTicker.start();
	eepromGet();
}

void loop() {
	sendRadioTicker.update();
	// getRadioTicker.update();
}
