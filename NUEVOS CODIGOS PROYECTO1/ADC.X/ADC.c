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
#include "ADC.h"
#include "I2C.h"

//Este codigo se utilizo para los 4 sensores cambiando las direcciones de los pic slaves entre 0X50,0X52,0X54 y 0X56.

#define _XTAL_FERQ 8000000

//Variables de codigo
int Coversion;
int z;
int a;
//Codigo de dely explicado y creado por el profesor
void delay_ms (unsigned int dms){
    for (int i = 0; i<dms; i++){
        for(int j = 0; j<255; j++);
    }
}
//codigo de interrupciones
void __interrupt () isr(void){
    
    if (PIR1bits.ADIF == 1){//interrupcion de la conversion del ADC
        Coversion = ADRESH;
        ADCON0bits.GO_DONE = 1;//nit go done del ADC
        PIR1bits.ADIF = 0;
    }
    
    if (PIR1bits.SSPIF == 1){//La larguisima interrupcion del I2C
        SSPCONbits.CKP = 0;
        if ((SSPCONbits.SSPOV)||(SSPCONbits.WCOL)){//Revision de colision y de transimison de data del I2C para ver si no ocurrio ningun accidente
            z = SSPBUF;//Valor del BUFFER guardado en z
            SSPCONbits.SSPOV = 0;
            SSPCONbits.WCOL = 0;
            SSPCONbits.CKP = 1;
        }
        
        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {/// segunda revision + espera de condiciones indicadas para transmitir datos
            z = SSPBUF;                 
            PIR1bits.SSPIF = 0;         
            SSPCONbits.CKP = 1;         
            while(!SSPSTATbits.BF);     
            a = SSPBUF;             
            __delay_us(250);
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){//valor del Slave guadrdado en buffer para ser mandado al master
            z = SSPBUF;
            BF = 0;
            SSPBUF = Coversion;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
        
        PIR1bits.SSPIF = 0;
    }
}


void CONFIGS (void){//configuracion del PIC para el I2c y El ADC
    TRISA = 0b00000001;
    ANSEL = 0b00000001;
    TRISB = 0;
    
    ADC(0);
    delay_ms(50);
    
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;//Banderas del ADC
    
    I2C_Slave_Init(0x56);//direccion del Pic slave
    
    ADCON0bits.GO_DONE = 1;
    
}


void main(void) {//main
    CONFIGS();
    while(1){
    PORTB = Coversion;
    }
}
