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

#define _XTAL_FREQ 8000000
#include <xc.h>
#include "8 bits.h"
#include "uart.h"
#include "SPI.h"

int a;
int POT;
int FOTO;
int CONT;
int OTRO;
int b;
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    TRISC2 = 0;
    
    PORTCbits.RC2 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);

}
void main()
{
setup();
   UART_Init(9600);
   b = 0;

   while (1){
      if(UART_Data_Ready()){
        a = UART_Read();
      
       __delay_ms(100);
      }
      if (b == 0){
          POT = a;
          b++;
      }
      else if (b == 1){
          FOTO = a;
          b++;
      }
      else if (b == 2){
          CONT = a;
          b++;
      }
      else if (b == 3){
          OTRO = a;
          b = 0;
      }
      
      PORTCbits.RC2 = 0;       //Slave Select
       __delay_ms(1);
       
      spiWrite(a);
       
       __delay_ms(1);
       PORTCbits.RC2 = 1;
       
 
   }
}