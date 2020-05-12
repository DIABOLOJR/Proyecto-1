/******************************************************************************/
/* AUTOR: LUIS JAVIER NIJ CULAJAY                                             */
/* FECHA: ENERO DE 2020                                                       */
/*                                                                            */                                   
/* ESTA LIBRERÍA, CONSTITUYE UN CONGLOMERADO DE FUNCIONES ESPECIALES PARA EL  */
/* USO E IMPLEMENTACIÓN DE DELAYS PATITO EN EL CHIP MICROCONTROLADOR PIC      */
/* MODELO 16F887 Y SUS VARIANTES APLICABLES.                                  */
/* TODA COMERCIALIZACIÓN NO AUTORIZADA DE LA SIGUIENTE LIBRERÍA LOS METERÁ EN */
/* BRONCAS CON EL AUTOR, Y ES BRAVO ASI QUE AGUAS.                            */
/* POR UNA GUATEMALA EN PAZ, REPORTÓ GUATEVISIÓN                              */


#include <xc.h>
#include <stdint.h>
/******************************************************************************/
/* FUNCIÓNES DELAY*/
void delay_ms(unsigned char dms){
    for (int i = 0; i<dms;i++)
    {for (int j = 0; j<255 ;j++){}}}
/******************************************************************************/

void delay_us (unsigned char ams){
    for (int j = 0; j<ams;j++){
        asm("NOP");}}
/******************************************************************************/