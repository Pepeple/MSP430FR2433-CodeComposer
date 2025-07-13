#include <msp430.h> 
#include <LCD16x2fr16MHz.h>

/**
 * main.c
 */
unsigned char buff_Rx[4]={0,0,0,0};
unsigned char rojo[4]={0,0,0,0};
unsigned char verde[4]={0,0,0,0};
unsigned char azul[4]={0,0,0,0};
unsigned char i;
unsigned int dato=0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    DCO_16MHz_REFO();

    UCA1CTLW0 |= UCSSEL__SMCLK;
    UCA1BRW = 104;
    UCA1MCTLW |= UCOS16 | UCBRF_2 | 0xD600;
    UCA1CTLW0 &= ~UCSWRST;
    UCA1IE |= UCRXIE;
    P2SEL0 |= BIT6 | BIT5;
    P2SEL1 &= ~(BIT6 | BIT5);
    PM5CTL0 &= ~LOCKLPM5;

    TA0CTL=TASSEL_1+MC_1;
    TA0CCR0=255;

    TA0CCTL1= OUTMOD_7;
    TA0CCR1=0;              //BRILLO LED ROJO
    P1DIR |= BIT1;
    P1SEL0 &= ~BIT1;
    P1SEL1 |= BIT1;

    TA0CCTL2=OUTMOD_7;
    TA0CCR2=0;            // BRILLO LED VERDE
    P1DIR |= BIT2;
    P1SEL0 &= ~BIT2;
    P1SEL1 |= BIT2;


    TA1CTL=TASSEL_1+MC_1;
    TA1CCR0=255;

    TA1CCTL1= OUTMOD_7;
    TA1CCR1=0;            //BRILLO LED AZUL
    P1DIR |= BIT5;
    P1SEL0 &= ~BIT5;
    P1SEL1 |= BIT5;

    Ini_Lcd();
    __bis_SR_register(GIE);

      while(1)
      {
          Cmd_Lcd(0x80);
          Dato_Lcd(0x52);
          Dato_Lcd(0x3A);
          Dato_Lcd(rojo[1]);
          Dato_Lcd(rojo[2]);
          Dato_Lcd(rojo[3]);

          Cmd_Lcd(0x86);
          Dato_Lcd(0x47);
          Dato_Lcd(0x3A);
          Dato_Lcd(verde[1]);
          Dato_Lcd(verde[2]);
          Dato_Lcd(verde[3]);

          Cmd_Lcd(0xC3);
          Dato_Lcd(0x42);
          Dato_Lcd(0x3A);
          Dato_Lcd(azul[1]);
          Dato_Lcd(azul[2]);
          Dato_Lcd(azul[3]);


          TA0CCR1=rojo[0];
          TA0CCR2=verde[0];
          TA1CCR1=azul[0];

      }
}
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
    //verificar si entra R,G, B

    if(UCA1RXBUF == 0x52 || UCA1RXBUF  == 0x72 ||
       UCA1RXBUF == 0x47 || UCA1RXBUF  == 0x67 ||
       UCA1RXBUF == 0x42 || UCA1RXBUF  == 0x62)
    {
       i=1;
    buff_Rx[0] = UCA1RXBUF;
    dato=0;
    }

    else if (i>0 && i<4) //recibe los digitos del brillo
    {
     if(UCA1RXBUF >= 0x30 && UCA1RXBUF <=0x39) //verificar que sea un num del 0 a 9
     {
         buff_Rx[i] = UCA1RXBUF;
         i++;

      if(i>=4)
      {
         dato = (buff_Rx[1] - 0x30)*100;
         dato += (buff_Rx[2] - 0x30)*10;
         dato += (buff_Rx[3] - 0x30)*1;

      if(dato > 256){
          dato=0;
      }

       if (buff_Rx[0] == 0x52 || buff_Rx[0] == 0x72)
           {
           rojo[0] = dato;
           rojo[1] = buff_Rx[1];
           rojo[2] = buff_Rx[2];
           rojo[3] = buff_Rx[3];
           }
        else if (buff_Rx[0] == 0x47 || buff_Rx[0] == 0x67)
           {
            verde[0] = dato;
            verde[1] = buff_Rx[1];
            verde[2] = buff_Rx[2];
            verde[3] = buff_Rx[3];
            }
        else if (buff_Rx[0] == 0x42 || buff_Rx[0] == 0x62)
            {
            azul[0] = dato;
            azul[1] = buff_Rx[1];
            azul[2] = buff_Rx[2];
            azul[3] = buff_Rx[3];
            }

           i=0; //se reinicia para esperar un nuevo dato
          }
     }
     else{
        i=0;  //si no es un dato correcto se reinicia
         }
    }
}


