// Programa para controlar el llenado de cisternas con 3 sensores y controlar una bannda transportadora con interrupciones
#include <msp430.h> 

char on=0;
char llenando=0;
/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;

	P1DIR = 0x00;
	P1SEL0 = 0x00;
	P1SEL1 = 0x00;

	P2DIR |= (BIT6 | BIT5);
	P2SEL0 = 0x00;
	P2SEL1 = 0x00;

    PM5CTL0 &= ~LOCKLPM5;
    P2OUT &= ~(BIT6+BIT5);

    P1REN |= (BIT3|BIT6|BIT7);             // Habilitar resistencia pull-up/down
    P1OUT &= ~(BIT3|BIT6|BIT7);


    P1IES |= (BIT3|BIT6|BIT7);             // Interrupción en flanco de bajada (cuando se presiona el botón)
    P1IFG &= ~(BIT3|BIT6|BIT7);            // Limpiar bandera de interrupción
    P1IE |= (BIT3|BIT6|BIT7);              // Habilitar interrupción en P1.3

    __bis_SR_register(GIE);    // Habilitar interrupciones globales

    while(1)
    {
        if((P1IN & BIT4) == 0)
        {
            P2OUT &= ~BIT5;
        }
	
        if(((P1IN & BIT2) == BIT2)&&((P1IN & BIT5) == 0)&&((P1IN & BIT4) == 0))
        {
            P2OUT |= BIT6;
            llenando=1;
        }
        if(((P1IN & BIT2) == 0)||((P1IN & BIT5) == BIT5))
        {
            P2OUT &= ~BIT6;
            llenando=0;
        }
        if(((P1IN & BIT2) == BIT2)&&llenando)
        {
            P2OUT |= BIT6;
        }
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
    if (P1IFG & BIT3)
    {
        P1IFG &= ~BIT3;
        P2OUT  ^= BIT5;
        on = ~on;
        __delay_cycles(25000);
    }

    else if (P1IFG & BIT6)
    {
        P1IFG &= ~BIT6;
        if(on)
        {
            __delay_cycles(450000);
            P2OUT |= BIT5;

        }
        __delay_cycles(25000);
    }

    else if (P1IFG & BIT7)
    {
        P1IFG &= ~BIT7;
        if(on)
        {
            P2OUT &= ~BIT5;
        }
        __delay_cycles(25000);
    }

}
