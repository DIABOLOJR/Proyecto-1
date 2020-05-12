/******************************************************************************/
/* AUTOR: LUIS JAVIER NIJ CULAJAY                                             */
/* FECHA: ENERO DE 2020                                                       */
/*                                                                            */                                   
/* ESTA LIBRERÍA, CONSTITUYE UN CONGLOMERADO DE FUNCIONES ESPECIALES PARA EL  */
/* USO E IMPLEMENTACIÓN DEL MÓDULO DE CONVERSIÓN ADC DEL MICROCONTROLADOR PIC */
/* MODELO 16F887 Y SUS VARIANTES APLICABLES.                                  */
/* TODA COMERCIALIZACIÓN NO AUTORIZADA DE LA SIGUIENTE LIBRERÍA LOS METERÁ EN */
/* BRONCAS CON EL AUTOR, Y ES BRAVO ASI QUE AGUAS.                            */
/* POR UNA GUATEMALA EN PAZ, REPORTÓ GUATEVISIÓN                              */

#include <xc.h>
#include <stdint.h>
#include <math.h>
 #define _XTAL_FREQ 8000000
void CONFIG_ADC(void) {
    ADCON1bits.ADFM  = 0;   /*   JUST. A LA IZQ.DEL REGISTRO ADRESH Y ADRESL  */
    ADCON1bits.VCFG1 = 0;   /*  ESTABLECMOS LA REFERENCIA NEGATIVA DEL ADC    */
    ADCON1bits.VCFG0 = 0;   /*  ESTABLECMOS LA REFERENCIA POSITIVA DEL ADC    */
    ADCON0bits.ADCS1 = 1;   /*A/D CONVERSION CLOCK SELECT-> ADCS1   ADCS0     */
    ADCON0bits.ADCS0 = 0;   /* FOSC/8 A 8 MHz -> 40us         1       0       */
    ADCON0bits.CHS3  = 0;    /* BITS SELECTORES DE CANAL DE CONVERSIÓN ADC     */
    ADCON0bits.CHS2  = 0;   /*  0     0     0     0     0      0    0     1   */
    ADCON0bits.CHS1  = 0;   /* CHS3  CHS2  CHS1  CHS0  CHS3  CHS2  CHS1  CHS0 */    
    ADCON0bits.CHS0  = 0;   /* CHANNEL 0000           Y CHANNEL 0001          */
    ADCON0bits.ADON  = 1;   /*    ENCENDEMOS EL MÓDULO DE CONVERSIÓN ADC      */
    //__delay_us(25);
    return;
}
    
    
  
