//Programa para controlar un motor y contar sus revolciones por segundo utilizando un sensor infrarojo
#include <msp430.h> 
#include <LCD16x2fr16MHz.h>

char DMillar = 0x30 , UMillar = 0x30 , Centena = 0x30, Decena = 0x30, Unidad = 0x30,  imprimir = 1;
unsigned int Flancos, RPS, cont;

/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	DCO_16MHz_REFO();

	TA0CTL = TASSEL_2+ID_3+MC_1+TAIE;
	TA0CCR0 = 19999;
	TA0CCTL2 = OUTMOD_6;
	TA0CCR2 = 9999;
	P1DIR |= BIT2;
	P1SEL0 &= ~BIT2;
	P1SEL1 |= BIT2;


	P1DIR |= BIT0;
	P1DIR &= ~(BIT1|BIT4|BIT5);
	P1REN |= (BIT4|BIT5);

	P1OUT &= ~BIT0;
    PM5CTL0 &= ~LOCKLPM5;
    P1IE |= (BIT1|BIT4|BIT5);
    P1IES &= ~(BIT1|BIT4|BIT5);
    P1IFG = 0x00;
    __bis_SR_register(GIE);
    Ini_Lcd();
    Dato_Lcd('R');
    Dato_Lcd('P');
    Dato_Lcd('S');
    Dato_Lcd('=');
    while(1)
    {
        if(imprimir==1)
        {

        DMillar = 0x30;
        UMillar = 0x30;
        Centena = 0x30;
        Decena = 0x30;
        Unidad = 0x30;
        while(RPS >= 10000)
        {
           RPS -= 10000;
           DMillar++;
        }
        while(RPS >= 1000)
        {
           RPS -= 1000;
           UMillar++;
        }
        while(RPS >= 100)
        {
           RPS -= 100;
           Centena++;
        }
        while(RPS >= 10)
        {
           RPS -= 10;
           Decena++;
        }
        Unidad += RPS;
        Cmd_Lcd(0x84);
        Dato_Lcd(DMillar);
        Dato_Lcd(UMillar);
        Dato_Lcd(Centena);
        Dato_Lcd(Decena);
        Dato_Lcd(Unidad);
        imprimir = 0;


        }

    }
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A(void) {
    switch(TA0IV)
    {
    case 2: break; //TACCR1
    case 4: break; //TACCR2
    case 14:
        cont++;
        if (cont >= 100){
            P1OUT ^= BIT0;
            cont = 0;
            RPS = Flancos;
            Flancos = 0;
            imprimir = 1;
        }

             break; //TAIFG

    }
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {
    if(P1IFG & BIT1)
    {
        P1IFG &= ~BIT1;
        Flancos++;
    }
    if(P1IFG & BIT4)
    {
        P1IFG &= ~BIT4;

        TA0CCR2 += 1000;
    }
    if(P1IFG & BIT5)
    {
        P1IFG &= ~BIT5;
        TA0CCR2 -= 1000;
    }



}
