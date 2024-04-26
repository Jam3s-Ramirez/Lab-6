/**************************
Universidad del Valle de Guatemala
Programación de Microcrontroladores
Proyecto: Poslab 6
Hardware: ATMEGA328p
Created: 18/04/2024 19:29:40
Author : James Ramírez
***************************/

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

int potenciometro = 0, ascii = 0;	//VARIABLES PARA EL MENÚ
int dato;							//VARIABLE PARA EL ADC

void initADC(void);
void initUART9600(void);
void writeUART(char Caracter);
void writeTextUART(char * Texto);
volatile uint8_t  datoTX;
volatile char receivedChar = 0;

char lista[10] = {'0','1','2','3','4','5','6','7','8','9'};
int main(void)
{
	//SE UTILIZA EL PORTB Y PORTD PARA LA CONEXIÓN DE LOS LEDS
	DDRB = 0b00111111;  
	DDRD |= 0b11000000;  
		
	initADC();
	initUART9600();
	sei();
		
	while (1)
	{
		
	ADCSRA |= (1 << ADSC);   //INICIA LA LECTURA DEL ADC
		
	//	MENÚ PRINCIPAL
	if(potenciometro == 0){
	writeTextUART("\n\rBienvenido a mi poslab :)\n\r");
	writeUART(10);
	writeUART(13);
	writeTextUART("Selecciona una de las dos opciones a continuación:\n\r");
	writeUART(10);
	writeUART(13);
	writeTextUART("1. Leer potenciometro\n\r");
	writeUART(10);
	writeUART(13);
	writeTextUART("2. Enviar ASCII\n\r");  
	writeUART(10);
	writeUART(13);
	potenciometro = 1;
	}
	
	if(receivedChar != 0){
		switch(receivedChar)   //ESTE SWITCH CASE MUESTRA EL VALOR DEL POTENCIÓMETRO O ENVIAR UN ASCII DEPENDIENDO DE LA OPCIÓN SELECCIONADA
		{
			case '1':
				writeTextUART("\n\r------------------------------------------\n\r");
				writeTextUART("EL VALOR DEL POTENCIÓMETRO ES:");
				
				// SEGMENTACIÓN DE LOS BITS PARA MOSTRAR LOS VALORES A TRAVÉS DE LOS LEDS
				uint8_t ValueLow = dato & 0b00111111; 
				uint8_t ValueHigh = (dato >> 6) & 0b11;

				PORTB = ValueLow;

				PORTD = (PORTD & ~0b11000000) | (ValueHigh << 6);
				
				// SEGMETACIÓN DEL VALOR DEL POTENCIÓMETRO PARA CONVERTIRLO Y MOSTRARLO EN DECIMALES A TRAVÉS DE LA INTERFAZ
				int millares = dato/1000;
				int centenas = (dato-(millares*1000))/100;
				int decenas = (dato- (millares*1000 + centenas*100))/10;
				int unidades = dato-(millares*1000 + centenas*100 + decenas*10 );
				
				switch(centenas){ 
					case 0:
						writeUART(lista[0]);
						break;
					case 1:
						writeUART(lista[1]);
						break;
					case 2:
						writeUART(lista[2]);
						break;
					case 3:
						writeUART(lista[3]);
						break;
					case 4:
						writeUART(lista[4]);
						break;
					case 5:
						writeUART(lista[5]);
						break;
					case 6:
						writeUART(lista[6]);
						break;
					case 7:
						writeUART(lista[7]);
						break;
					case 8:
						writeUART(lista[8]);
						break;
					case 9:
						writeUART(lista[9]);
						break;
				}
				
				switch(decenas){
					case 0:
					writeUART(lista[0]);
					break;
					case 1:
					writeUART(lista[1]);
					break;
					case 2:
					writeUART(lista[2]);
					break;
					case 3:
					writeUART(lista[3]);
					break;
					case 4:
					writeUART(lista[4]);
					break;
					case 5:
					writeUART(lista[5]);
					break;
					case 6:
					writeUART(lista[6]);
					break;
					case 7:
					writeUART(lista[7]);
					break;
					case 8:
					writeUART(lista[8]);
					break;
					case 9:
					writeUART(lista[9]);
					break;
				}
				
				switch(unidades){ 
					case 0:
					writeUART(lista[0]);
					break;
					case 1:
					writeUART(lista[1]);
					break;
					case 2:
					writeUART(lista[2]);
					break;
					case 3:
					writeUART(lista[3]);
					break;
					case 4:
					writeUART(lista[4]);
					break;
					case 5:
					writeUART(lista[5]);
					break;
					case 6:
					writeUART(lista[6]);
					break;		
					case 7:
					writeUART(lista[7]);
					break;
					case 8:
					writeUART(lista[8]);
					break;
					case 9:
					writeUART(lista[9]);
					break;
				}
				
				potenciometro = 0;   //COLOCA VALOR 0 PARA ENTRAR DE NUEVO EN EL IF Y MOSTRAR EL MENÚ PRINCIPAL
				receivedChar  = 0;   //SALE DEL IF 
				writeTextUART("\n\r------------------------------------------\n\r");
			break;
			
			case '2':
				receivedChar = 0;	//COLOCAR EL VALOR DE ASCII
				writeTextUART("\n\rIngrese un valor\n\r");
				ascii = 1;		
				break;
		}
	}
}

}

void initUART9600(void){
	
	cli ();
	//PORTB COMO SALIDAS
	DDRB = 0xFF;   // Todo como salidas
	PORTB = 0x00;
	
	//PASO 1: RX SE TOMA COMO ENTRADA Y TX COMO SALIDA
	DDRD &= ~(1<<DDD0);
	DDRD |= (1<<DDD1);
	
	//PASO 2: CONFIGURACION DEL UCSR0A
	UCSR0A = 0;
	
	//PASO 3: CONFIGURACION DEL UCSR0B, SE HABILITA ISR DE RECEPCION Y SE HABILITAN RX Y TX
	UCSR0B = 0;
	UCSR0B |= (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	
	//PASO 4: CONFIGURACION DE UCSR0C COMO ASINCRONO, SIN PARIDAD, 1 BIT DE STOP, 8 BITS DE DATA
	UCSR0C = 0;
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	
	//PASO 5: CONFIGURAR VELOCIDAD DE BAUDRATE: 9600
	UBRR0 = 103;
}


void initADC(void){
	ADMUX = 6;
	//REFERENCIA AVCC = 5V
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);
	
	//JUSTIFICACION A LA IZQUIERDA
	ADMUX |= (1<<ADLAR);
	
	ADCSRA = 0;
	
	//HABILITA INTERRUPCION
	ADCSRA	|= (1<<ADIE);
	
	//HABILITA PREESCALER A 128 - 125kHz
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
	//HABILITA EL ADC
	ADCSRA |= (1<<ADEN);
}


ISR(ADC_vect)
{
	dato = ADCH;   //SE CARGA EL VALOR DEL ADC
	ADCSRA |= (1<<ADIF); //SE RESETEA LA BANDERA DE LA INTERRUPCIÓN
	
}

void writeUART(char Caracter)
{
	while(!(UCSR0A & (1<<UDRE0)));  
	UDR0 = Caracter;
		
}


void writeTextUART(char * Texto){
	uint8_t i;
	for(i = 0; Texto[i]!= '\0'; i++){
		while(!(UCSR0A & (1<<UDRE0)));
			UDR0 = Texto[i];
	}
}

ISR(USART_RX_vect)
{
	receivedChar = UDR0; 
	
	if (ascii == 1){    //ESCRIBIR ASCII
		// SEGMENTACIÓN DE LOS BITS PARA MOSTRAR LOS VALORES A TRAVÉS DE LOS LEDS
		uint8_t ValueLow = receivedChar & 0b00111111; 
		uint8_t ValueHigh = (receivedChar >> 6) & 0b11;

		PORTB = ValueLow;

		PORTD = (PORTD & ~0b11000000) | (ValueHigh << 6);
		ascii = 0;  
		potenciometro = 0; 
	}
	
	while(!(UCSR0A & (1<<UDRE0)));
		UDR0 = receivedChar;
		
}