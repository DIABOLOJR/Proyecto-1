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
#include "8 bits.h"
#include "UART.h"
#define _XTAL_FREQ 8000000
//Variables
int POT;
int CONT;
int FOTO;
int OTRO;
int a = 0;
int J;



//Configuracion del pic
void Properties (void){
    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0;
    I2C_Master_Init(100000);
    J = 0;
    POT = 0;
    FOTO = 0;
    CONT = 0;
    OTRO =0;
    UART_Init(9600);

}

void main(void) {
    Properties();
    
    while(1){
        I2C_Master_Start();//Inicializacion del master de a libreria
        I2C_Master_Write(0x51);//revision de la direccion del pic al que se le esta comunicando
        POT = I2C_Master_Read(0);//lectura del buffer del I2C
        I2C_Master_Stop();//parar la comunicacion del I2C con todos lo pics

        delay_ms(20);

        I2C_Master_Start();
        I2C_Master_Write(0x53);
        CONT = I2C_Master_Read(0);
        I2C_Master_Stop();

        delay_ms(20);

        I2C_Master_Start();
        I2C_Master_Write(0x55);
        FOTO = I2C_Master_Read(0);
        I2C_Master_Stop();
        delay_ms(20);
        
        I2C_Master_Start();
        I2C_Master_Write(0x57);
        OTRO = I2C_Master_Read(0);
        I2C_Master_Stop();
        delay_ms(20);
        

        
        if (a == 0){
        UART_Write(POT);
        a++;    
        }
        else if (a == 1){
        UART_Write(FOTO);
        a++;
        }
        else if (a == 2){
        UART_Write(OTRO);
        a++;
        }
        else if (a == 3){
        UART_Write(CONT);
        a = 0;
        }
        else{
        a = 0;
        }

//Funciones de LCD para mappear los valor del I2C
    }
}