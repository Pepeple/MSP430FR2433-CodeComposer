#include <msp430.h>
#include <LCD16x2fr16MHz.h>

unsigned int retardo = 0, disparo=0, nadc = 0, angulo = 0, mVin = 0;
char digitos[7] = {0, 0, 0, 0, 0, 0, 0};
char i = 0, imprimir = 1;

void Cifra(unsigned int num);

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    DCO_16MHz_REFO();

    TA1CTL = TASSEL_2 + ID_3 + MC_2;
    TA1EX0 = TAIDEX_1;

    // timer
    TA1CCTL1 = CM_3 + CAP + CCIE;
    TA1CCTL2 = CCIE;

    P1DIR &= ~BIT5;             // Entrada de captura
    P1SEL0 &= ~BIT5;
    P1SEL1 |= BIT5;

    P1DIR |= BIT4;       // Configurar salidas
    P1SEL0 &= ~BIT4;
    P1SEL1 &= ~BIT4;

    // ADC
    SYSCFG2 |= ADCPCTL1;
    ADCCTL0 |= ADCSHT_2 | ADCON;
    ADCCTL1 |= ADCSHP+ADCCONSEQ_0;
    ADCCTL2 |= ADCRES;
    ADCIE |= ADCIE0;
    ADCMCTL0 |= ADCINCH_1 | ADCSREF_1;

    // 1.5V
    PMMCTL0_H = PMMPW_H;
    PMMCTL2 |= INTREFEN;
    __delay_cycles(400);

    PM5CTL0 &= ~LOCKLPM5;

    Ini_Lcd();
    __bis_SR_register(GIE);


    while (1)
    {
        ADCCTL0 |= ADCENC | ADCSC;      // Sampling and conversion start
        __delay_cycles(4000000);

        if (imprimir == 1)
        {
            mVin = ((unsigned long)nadc * 1500) / 1023;

            retardo = ((unsigned long)nadc * 8300) / 1023;

            angulo = ((unsigned long)nadc * 180) / 1023;



            Cifra(mVin);
            Cmd_Lcd(0x80);
            Dato_Lcd('V');
            Dato_Lcd(':');
            Cmd_Lcd(0x82);
            i = 0;
            while (i < 4)
            {
                if (i == 1) Dato_Lcd(0x2E);
                Dato_Lcd(digitos[i]);
                i++;
            }

            Cifra(angulo);
            Cmd_Lcd(0xC0);
            Dato_Lcd('A');
            Dato_Lcd(':');
            Cmd_Lcd(0xC2);
            i = 1;
            while (i < 4)
            {
                Dato_Lcd(digitos[i]);
                i++;
            }
            Dato_Lcd(0xDF);




            imprimir = 0;
        }

    }
}

#pragma vector = TIMER1_A1_VECTOR
__interrupt void Contador(void)
{
    switch (TA1IV)
    {
        case 2:
            TA1CCR2  = TA1CCR1 + retardo;
            break;

        case 4:

                P1OUT |= BIT4;           // Activar el disparo
                __delay_cycles(320);
                P1OUT &= ~BIT4;          // Desactivar el disparo

            break;
    }
}

#pragma vector = ADC_VECTOR
__interrupt void ADC_ISR(void)
{
    nadc = ADCMEM0;
    imprimir = 1;

}

void Cifra(unsigned int num)
{
    i = 0;
    while (i < 4)
    {
        digitos[i] = 0x30;
        i++;
    }

    char residuo = 0;
    while (num >= 1000)
    {
        num -= 1000;
        residuo++;
    }
    digitos[0] += residuo;

    residuo = 0;
    while (num >= 100)
    {
        num -= 100;
        residuo++;
    }
    digitos[1] += residuo;

    residuo = 0;
    while (num >= 10)
    {
        num -= 10;
        residuo++;
    }
    digitos[2] += residuo;

    residuo = 0;
    while (num >= 1)
    {
        num -= 1;
        residuo++;
    }
    digitos[3] += residuo;
}
