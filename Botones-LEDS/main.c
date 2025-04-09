#include <msp430.h> 

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P1DIR |= BIT0+BIT1;
    P1SEL0 &= ~(BIT0+BIT1);
    P1SEL1 &= ~(BIT0+BIT1);

    P2DIR &= ~(BIT3+BIT7); //Bit 3 y bit 7 como entrada sin afectar los demas
    P2SEL0 &= ~(BIT0+BIT1);
    P2SEL1 &= ~(BIT0+BIT1);

    P2REN |= BIT3+BIT7; //habilita las resistencias pull up y down
    P2OUT |= BIT3+BIT7; // para escoger que sea pull up

    PM5CTL0 &= ~LOCKLPM5;

    P1OUT &= ~(BIT0+BIT1); //para que inicien apagados

 while(1){
     if((P2IN & BIT3) == 0) //en 0 por que esta en 1 logico con las resisencia pull up
     {
         if((P1OUT & BIT0)==BIT0){
             P1OUT &= ~BIT0;
         }
         else{
             P1OUT |= BIT0;
         }
         while((P2IN & BIT3) == 0){ //para que se mantega el estado si dejo precionado el boton
         }
     }
     if((P2IN & BIT7) == 0)
     {
         if((P1OUT & BIT1)==BIT1){
             P1OUT &= ~BIT1;
         }
         else{
                  P1OUT |= BIT1;
         }
         while((P2IN & BIT7) == 0){
         }
      }

}

}
