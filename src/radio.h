
#include <SoftwareSerial.h>

#define RX 8
#define TX 9

#define LED 13

SoftwareSerial radio(RX, TX);

void setupRadio() { radio.begin(9600); }
String datosRadio() {
	String s = "";
	// Construimos un mensaje de texto sumando los valores interpretados como un
	// objeto String (NOTA: String NO ES IGUAL cadena de texto)
	s = s + paquete.idPaquete + ";" + paquete.temperatura + ";" +
		paquete.altitud + ";" + paquete.acMaxima + ";" + paquete.acMinima +
		"\n";
	return s;
}

void radioInfo() {
	paquete.idPaquete++; // incrementamos el valor de paquete a enviar
	// paquete.temperatura = bmp.readTemperature();
	// paquete.altitud = bmp.readAltitude(1013.25);
	String strPaquete =
		datosRadio();	  // Crea el paquete completo con la información;
	char radiopacket[64]; // Tenemos una cadena de texto
	sprintf(radiopacket, "%s",
			strPaquete.c_str()); // Metemos en la cadena de texto la parte que
								 // nos interesa del mensaje(tipo String)
	Serial.print("enviando ");
	Serial.print(radiopacket);
	// Enviar el mensaje al DESTINO (DEST_ADDRESS)
	radio.println(radiopacket);
	// Se graban en la eeprom datos cada 5 paquetes enviados guardando lo máximo
	// y minimo durante esos 5 segundos
	if (paquete.idPaquete % TIEMPO_SALVADO == 0) {
		eepromSave();
		Serial.println("Grabando");
	}
	setupDatos();
}