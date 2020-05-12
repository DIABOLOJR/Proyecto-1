// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "I2C.h"
//variables

#define _XTAL_FREQ 8000000

int VALOR;

int z;
int a;

int Aumento;
int Decremento;

//interrupción
void __interrupt () isr(void){
    if (INTCONbits.RBIF == 1){//Interrupcion de push button del puerto b
        if(PORTBbits.RB7 == 0){
            Aumento = 1;//variables para el aumento o decremento del puerto
        }
        else if (PORTBbits.RB6 == 0){
            Decremento = 1;
        }
        INTCONbits.RBIF = 0;
    }
    else if (PIR1bits.SSPIF == 1){
        SSPCONbits.CKP = 0;
        if ((SSPCONbits.SSPOV)||(SSPCONbits.WCOL)){
            z = SSPBUF;
            SSPCONbits.SSPOV = 0;
            SSPCONbits.WCOL = 0;
            SSPCONbits.CKP = 1;
        }
        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            z = SSPBUF;                 
            PIR1bits.SSPIF = 0;         
            SSPCONbits.CKP = 1;         
            while(!SSPSTATbits.BF);     
            a = SSPBUF;             
            __delay_us(250);
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = VALOR;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
        PIR1bits.SSPIF = 0;
    }
}

void SET(void){
    TRISA = 0x00;
    ANSEL = 0x00;
    TRISB = 0b11111111;
    IOCB = 0b11111111;
    ANSELH = 0;
  
    VALOR = 0;
    Aumento = 0;
    Decremento = 0;
    
    INTCONbits.PEIE = 1;
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    INTCONbits.GIE =1;
    
    I2C_Slave_Init(0x32);
}

void contador (void){//contaador de 4 bits
    if (PORTBbits.RB7 == 1 && Aumento == 1){//revisa si el push 1 esta en 1 y si su variable esta encendida
        if (VALOR < 15){//si el valor es menor a 15
            VALOR++;//suma 1 al valor
        }
        else {
            VALOR = 0;//reinicia el valor si ya es mayor a 15
        }
        Aumento = 0;//limpia la variable
    }//lo mismo para el decremento solo que reinicia si es 15 y le resta 1 al prgrama
    else if (PORTBbits.RB6 == 1 && Decremento == 1){
        if (VALOR > 0){
            VALOR--;
        }
        else {
            VALOR = 15;
        }
        Decremento = 0;
    }
}

void main(void) {
    SET();
    while(1){
    contador();
    PORTA = VALOR;//Coloca el valor en el PORTA y en el buffer en la interrupcion
    }
}
