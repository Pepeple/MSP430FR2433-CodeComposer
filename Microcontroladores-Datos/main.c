#include <msp430.h> 


volatile unsigned char Datos_Men[] = {0x00};// conts para flash y volatile para la ram

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P1DIR = 0XFF;
    P1SEL0 = 0x00;
    P1SEL1 = 0x00;

    P3DIR &= ~BIT2;
    P3SEL0 &= ~BIT2;
    P3SEL1 &= ~BIT2;

    P3REN |= BIT2; //habilita las resistencias pull up y down
    P3OUT |= BIT2;

    PM5CTL0 &= ~LOCKLPM5;

 while(1){


}

}
