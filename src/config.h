

/*
 * Creamos una unidad de información con formato de estructura
 * Se crea GLOBAL para evitar tener que pasar datos y solo se actualizan en su
 * función
 *
 */

#include "utils.h"

// Hay que indicar la frecuenca de nuestra radio
#define RF69_FREQ 868.0 // esto es un comentario 868//915.0

// Cada radio tiene un identificador
#define DEST_ADDRESS 2 // Esta es el identificador del satelite.

// Cada emisor deberá tener una dirección única
#define MY_ADDRESS 1

// Pines donde están conectadas nuestras placas, definidos por el fabricante

#define RFM69_INT 3 //
#define RFM69_CS 10 //
#define RFM69_RST 4 //
// El tamaño de la estructura es de muy grande (18 Bytes)
struct dato {
	uint16_t idPaquete;
	float temperatura;
	float altitud;
	float acMaxima;
	float acMinima;
} paquete;

// El tamañano de la estructura es de 4 bytes
/*
 * temperatura, aceleración máxima y mínima se guardan como enteros
 * La altitud se divide por 10 así tenemos rangos de altitud sufuciente con un
 * byte Tenemos una estructura de 4 bytes que permiten 256 valores Cada valor se
 * grabará cada 5'' lo que da 21 minutos de datos guardados en la caja negra.
 */

#define TIEMPO_SALVADO 5
struct miniDato {
	int8_t temperatura;
	int8_t altitud;
	int8_t acMaxima;
	int8_t acMinima;
} miniPaquete;

uint8_t acMaxima5 = 0;
uint8_t acMinima5 = 255;
void setupDatos() {
	paquete.acMaxima = 0;
	paquete.acMinima = 2000;
}