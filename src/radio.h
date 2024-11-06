#include <RHDatagram.h>
#include <RH_RF69.h>
#include <SPI.h>

// ------------------------------------------ //
// #include <SoftwareSerial.h>
/*
  Radio usada APC 220
  RX 8
  TX 9

*/
// SoftwareSerial radio(RX, TX);
// void setupRadioUSB() { radio.begin(9600); }

// #define RX 8
// #define TX 9
// ------------------------------------------ //
#define LED 13

/*//Tamaño del mensaje
  #define RH_RF69_MAX_ENCRYPTABLE_PAYLOAD_LEN 64 //tamaño del mensaje
  codificable #define RH_RF69_HEADER_LEN 4 //tamaño de la cabecera #define
  RH_RF69_MAX_MESSAGE_LEN (RH_RF69_MAX_ENCRYPTABLE_PAYLOAD_LEN -
  RH_RF69_HEADER_LEN)
*/

// Creamos un "objeto" radio con el trabajaremos.
RH_RF69 rf69(RFM69_CS, RFM69_INT);
RHDatagram rf69_manager(rf69, MY_ADDRESS);

bool setupRadio() {
	bool radioOk = false;

	pinMode(RFM69_RST, OUTPUT);
	digitalWrite(RFM69_RST, LOW);

	DUMPSLN("Transmisón desde estacion");

	// manual reset para activar la radio.
	digitalWrite(RFM69_RST, HIGH);
	delay(10);
	digitalWrite(RFM69_RST, LOW);
	delay(10);

	if (rf69_manager.init()) {
		radioOk = true;
	}

	if (!rf69.setFrequency(RF69_FREQ)) {
		DUMPSLN("setFrequency failed \n CHECK RADIO");
	}

	rf69.setTxPower(20); // rango de 14-20 arg must be true for 69HCW

	// Tenemos una clave de cifrado para los mensajes, debe ser la misma en el
	// emisor y el receptor
	uint8_t key[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
					 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
	rf69.setEncryptionKey(key);

	pinMode(LED, OUTPUT);

	DUMP("RFM69 radio @", (int)RF69_FREQ);
	DUMPSLN(" MHz");
	return radioOk;
}

String datosRadio() {
	String s = "";
	// Construimos un mensaje de texto sumando los valores interpretados como un
	// objeto String (NOTA: String NO ES IGUAL cadena de texto)
	s = s + paquete.idPaquete + ";" + paquete.temperatura + ";" +
		paquete.altitud + ";" + paquete.acMaxima + ";" + paquete.acMinima +
		"\n";
	return s;
}

void radioSendInfo() {

	DUMPSLN("Sent Packet To Satellite ");
	paquete.idPaquete++; // incrementamos el valor de paquete a enviar
	String strPaquete =
		datosRadio();	  // Crea el paquete completo con la información;
	char radiopacket[64]; // Tenemos una cadena de texto
	sprintf(radiopacket, "%s",
			strPaquete.c_str()); // Metemos en la cadena de texto la parte que
								 // nos interesa del mensaje(tipo String)
	DUMP("enviando ", radiopacket);

	// Esta funcion de espera bloquea el programa
	if (rf69_manager.sendto((uint8_t *)radiopacket, strlen(radiopacket),
							DEST_ADDRESS)) {
		DUMPSLN("Sent Packet To Satellite");

		/*
		if (rf69_manager.waitPacketSent()) {
			paquete.idPaquete++; // Asumiendo que 'paquete' está definido en
								 // algún lugar del sketch
			setupDatos(); // Asumiendo que esta función prepara los datos
						  // para el próximo envío
		} else {
			DUMPSLN("Failed to send packet");
		}
		*/
	};

	setupDatos();
}

void respondToSender(uint8_t *data, uint8_t len) {
	// Enviar una respuesta al emisor, si es necesario
	if (rf69_manager.sendto(data, len, rf69.headerFrom())) {
		rf69_manager.waitPacketSent();
		DUMPSLN("Response sent");
	} else {
		DUMPSLN("Response send failed");
	}
}

void radioReceiveInfo() {
	if (rf69_manager.available()) {
		uint8_t buf[RH_RF69_MAX_MESSAGE_LEN];
		uint8_t len = sizeof(buf);

		// Intenta recibir un mensaje
		if (rf69_manager.recvfrom(buf, &len, nullptr)) {
			DUMP("Received message: ", (char *)buf);

			// Aquí puedes añadir más lógica para procesar el mensaje
			// Por ejemplo, responder al emisor
			respondToSender(buf, len);
		} else {
			// Mensaje recibido, pero hay un error
			DUMPSLN("Receive failed");
		}
	} else {
		DUMPSLN("RF69 Manager is not available")
	}
}
