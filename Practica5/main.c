#include <msp430.h> 
#include <LCD16x2fr16MHz.h>

unsigned int distancia=0,save=0,valor=0, i=0;

char DMillar = 0x30 , UMillar = 0x30 , Centena = 0x30, Decena = 0x30, Unidad = 0x30;
char  imprimir = 1, modo = 0, fuera = 0;
const char mensaje[16] = {" Fuera de rango"};

/**
 * main.c
 */
int main(void)
{



    WDTCTL = WDTPW | WDTHOLD;
    DCO_16MHz_REFO();

    PM5CTL0 &= ~LOCKLPM5;

    TA1CTL = TASSEL_2+ID_3+MC_2;
    TA1EX0 = TAIDEX_1;

    TA1CCTL2 = CM_1 + CAP + CCIE;
    TA1CCTL1 = CM_2 + CAP + CCIE;
    P1DIR &= ~(BIT4|BIT5);
    P1SEL0 &= ~(BIT4|BIT5);
    P1SEL1 |= (BIT4|BIT5);

    P1DIR |= BIT1;
    P1OUT &= ~BIT1;


	P2DIR &= ~(BIT2|BIT1|BIT0);
	P2REN |= (BIT2|BIT1|BIT0);

    P2OUT &= ~(BIT2|BIT1|BIT0);


    P2IES |= (BIT2|BIT1|BIT0);
    P2IE |= (BIT2|BIT1|BIT0);
    P2IFG &= ~(BIT2|BIT1|BIT0);




        __bis_SR_register(GIE);
        Ini_Lcd();









    while(1)
    {










        if(imprimir==1)
            {
                Cmd_Lcd(0x80);


                if(modo==0)
                {
                    Dato_Lcd('D');
                    valor = distancia;
                }
                else
                {
                    Dato_Lcd('A');
                    valor = save - distancia;
                }
               DMillar = 0x30;
               UMillar = 0x30;
               Centena = 0x30;
               Decena = 0x30;
               Unidad = 0x30;
               while(valor >= 10000)
               {
                   valor -= 10000;
                  DMillar++;
               }
               while(valor >= 1000)
               {
                   valor -= 1000;
                  UMillar++;
               }
               while(valor >= 100)
               {
                   valor -= 100;
                  Centena++;
               }
               while(valor >= 10)
               {
                   valor -= 10;
                  Decena++;
               }
               Unidad += valor;

               Dato_Lcd(':');
               Dato_Lcd(DMillar);
               Dato_Lcd(UMillar);
               Dato_Lcd(Centena);
               Dato_Lcd(Decena);
               Dato_Lcd(Unidad);


               Cmd_Lcd(0xc0);
               if(fuera == 1)
               {
                   for (i = 0; i <= 16; i++)
                   {
                       Dato_Lcd(mensaje[i]);
                   }
                   fuera = 0;
               }
               else
               {
                   valor = save;
                   DMillar = 0x30;
                   UMillar = 0x30;
                   Centena = 0x30;
                   Decena = 0x30;
                   Unidad = 0x30;
                   while(valor >= 10000)
                   {
                       valor -= 10000;
                       DMillar++;
                   }
                   while(valor >= 1000)
                   {
                       valor -= 1000;
                       UMillar++;
                   }
                   while(valor >= 100)
                   {
                       valor -= 100;
                       Centena++;
                   }
                   while(valor >= 10)
                   {
                       valor -= 10;
                       Decena++;
                   }
                   Unidad += valor;
                   Dato_Lcd('R');
                   Dato_Lcd(':');
                   Dato_Lcd(DMillar);
                   Dato_Lcd(UMillar);
                   Dato_Lcd(Centena);
                   Dato_Lcd(Decena);
                   Dato_Lcd(Unidad);

               }


               imprimir = 0;
            }

    }

}




#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer_A(void) {
    switch(TA1IV)
    {
    case 2:
        distancia = TA1CCR1/58;
    imprimir = 1;

    if(distancia>900)
        fuera = 1;

        break; //TACCR1

    case 4:TA1R = 0x0000;
        break; //TACCR2

    case 14: break; //TAIFG

    }
}






#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void) {
    if (P2IFG & BIT2)
    {
        P2IFG &= ~BIT2;
        P1OUT  |= BIT1;
        __delay_cycles(160);
        P1OUT &= ~BIT1;
    }

    else if (P2IFG & BIT1)
    {
        P2IFG &= ~BIT1;
        save = distancia;
        imprimir = 1;

    }

    else if (P2IFG & BIT0)
    {
        P2IFG &= ~BIT0;
        if(modo==0)
            modo = 1;
        else
            modo = 0;

    }

}
