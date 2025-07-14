//Este codigo muestra mi nombre(ismael) en 8 bits con una barra de leds
#include <msp430.h> 
// Los datos son cantidades hexadecimales que definen la cantidad de leds prendidos siendo (0000 0000) todos apagados  y  (1111 1111) todos prendidos
volatile unsigned char Datos_Men[40] = {0x41,0x7f,0x41,0x00,0x00,0x32,0x49,0x49,0x49,0x26,0x00,0x00,0x7f,0x20,0x18,0x20,0x7f,0x00,0x00,0x3f,0x44,0x44,0x44,0x3f,0x00,0x00,0x7f,0x49,0x49,0x49,0x41,0x00,0x00,0x7f,0x01,0x01,0x01,0x01,0x00,0x00};
unsigned int i;

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	//Define todos lo pines del puerto 1 como salidas
	P1DIR = 0XFF; 
	P1SEL0 = 0x00;
	P1SEL1 = 0x00;
	PM5CTL0 &= ~LOCKLPM5;


	//Recorre constantemente el arrego con un pequeño delay para que sea visible al ojo humano
	while(1)
	{
	    for(i=0;i<=38;i++)
	    {
	        P1OUT=Datos_Men[i];
	        __delay_cycles(8000);

	    }
	}
}
