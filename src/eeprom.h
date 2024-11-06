#include <EEPROM.h>
uint8_t eeAddress = 0;
void eepromSave() {
	//    miniPaquete.idPaquete = (int16_t)(paquete.idPaquete); //No guardamos
	//    el orden ya que se guardan todos los datos seguidos desde el principio
	miniPaquete.altitud =
		(int8_t)(paquete.altitud / 10); // Dividimos el valor de la altitud para
										// que quepa en un byte
	miniPaquete.temperatura = (int8_t)paquete.temperatura;
	miniPaquete.acMaxima = (int8_t)acMaxima5;
	miniPaquete.acMinima = (int8_t)acMinima5;
	EEPROM.put(eeAddress, miniPaquete);
	eeAddress += sizeof(miniPaquete);
	Serial.println("grabando");
	acMaxima5 = 0;
	acMinima5 = 255;
}

void eepromGet() {
	String s = "";
	for (uint16_t i = 0; i < (1024 / sizeof(miniPaquete)); i++) {
		EEPROM.get(i * sizeof(miniPaquete), miniPaquete);
		s += String(miniPaquete.altitud) + "," +
			 String(miniPaquete.temperatura) + "," +
			 String(miniPaquete.acMaxima) + "," + String(miniPaquete.acMinima);
		Serial.println(s);
		s = "";
	}
}