#include "ADC.h"
#include <xc.h>
int a;// Seleccion de canal de ADC.
void ADC (int a){
    switch (a){
        case 0:
            ADCON1 = 0;
            ADCON0 = 0b01000001;
            break;
        case 1:
            ADCON1 = 0;
            ADCON0 = 0b01000101;
            break;
        case 2:
            ADCON1 = 0;
            ADCON0 = 0b01001001;
            break;
        case 3:
            ADCON1 = 0;
            ADCON0 = 0b01001101;
            break;
        case 4:
            ADCON1 = 0;
            ADCON0 = 0b01010001;
            break;
        case 5:
            ADCON1 = 0;
            ADCON0 = 0b01010101;
            break;
        case 6:
            ADCON1 = 0;
            ADCON0 = 0b01011001;
            break;
        case 7:
            ADCON1 = 0;
            ADCON0 = 0b01011101;
            break;
        case 8:
            ADCON1 = 0;
            ADCON0 = 0b01100001;
            break;
        case 9:
            ADCON1 = 0;
            ADCON0 = 0b01100101;
            break;
        case 10:
            ADCON1 = 0;
            ADCON0 = 0b01101001;
            break;
        case 11:
            ADCON1 = 0;
            ADCON0 = 0b01101101;
            break;
        case 12:
            ADCON1 = 0;
            ADCON0 = 0b01110001;
            break;
        case 13:
            ADCON1 = 0;
            ADCON0 = 0b01110101;
            break;
    }
}
