//Programa para hacer un dimer capturando las frecuencias de la señal de entrada y desproducinendola con un delay en la salida 
#include <msp430.h>
#include <LCD16x2fr16MHz.h>


const char mensaje[16] = {" Tiempo disparo "};
unsigned int i=0,delay=2000,disparo=2000,cont=0;

char DMillar, UMillar, Centena, Decena, Unidad,funcion = 'N';
char apagado=0;

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	DCO_16MHz_REFO();


    TA0CTL = TASSEL_2+ID_3+MC_2;

    TA0CCTL1 = CM_3+CAP+CCIE;
    P1DIR &= ~BIT1;
    P1SEL1 |= BIT1;
    P1SEL0 &= ~BIT1;

    TA0CCTL2 = CCIE;
    TA0CCR2 = 2000;
    disparo = TA0CCR2;



    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= BIT0;
    P1SEL0 &= ~BIT0;
    P1SEL1 &= ~BIT0;
    P1OUT &= ~BIT0;


    P1DIR &= ~(BIT4|BIT3|BIT2);
    P1REN |= (BIT4|BIT3|BIT2);


    P1IE |= (BIT4|BIT3|BIT2);
    P1IES &= ~(BIT4|BIT3|BIT2);
    P1IFG = 0x00;


    __bis_SR_register(GIE);


	Ini_Lcd();
    for (i = 0; i <= 16; i++) {
        Dato_Lcd(mensaje[i]);
    }

    while(1)
    {



        DMillar = 0x30;
        UMillar = 0x30;
        Centena = 0x30;
        Decena = 0x30;
        Unidad = 0x30;
        disparo = TA0CCR2;
        while(disparo >= 10000)
        {
            disparo -= 10000;
           DMillar++;
        }
        while(disparo >= 1000)
        {
            disparo -= 1000;
           UMillar++;
        }
        while(disparo >= 100)
        {
           disparo -= 100;
           Centena++;
        }
        while(disparo >= 10)
        {
            disparo -= 10;
           Decena++;
        }
        Unidad += disparo;
        Cmd_Lcd(0xC0);
        Dato_Lcd(' ');
        Dato_Lcd(DMillar);
        Dato_Lcd(UMillar);
        Dato_Lcd(Centena);
        Dato_Lcd(Decena);
        Dato_Lcd(Unidad);

        Cmd_Lcd(0xC8);
        Dato_Lcd('F');
        Dato_Lcd(funcion);






    }
}





#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A(void) {
    switch(TA0IV)
    {
    case 2:

        TA0CCR2 = TA0CCR1 + delay;
        disparo = TA0CCR2;
        break; //TACCR1
    case 4:
        if(!apagado)
        {
            P1OUT |= BIT0;
            __delay_cycles(320);
            P1OUT &= ~BIT0;
        }

        break; //TACCR2
    case 14: break; //TAIFG

    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {


    if(P1IFG & BIT4)
    {
        P1IFG &= ~BIT4;

        if(delay<14000)
        delay += 1000;
    }
    if(P1IFG & BIT3)
    {

        P1IFG &= ~BIT3;
        if(delay > 2000)
        delay -= 1000;
    }

    if(P1IFG & BIT2)
    {
        P1IFG &= ~BIT2;

        cont++;
        if(cont==1)
        {
            apagado=1;
            funcion='1';
        }
        if(cont==2)
        {
            apagado=0;
            delay=14000;
            funcion='2';
        }
        if(cont==3)
        {
            delay=8000;
            funcion='3';

        }
        if(cont==4)
        {
            delay=2000;
            funcion='4';
            cont=0;
        }


    }



}



