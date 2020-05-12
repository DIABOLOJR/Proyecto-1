/*La libreria implementada esta basada en el codigo 
 *de ejemplo del Catedratico y fue adaptada por el estudiante */

#include "I2C.h"
#include <xc.h>

void I2C_Master_Init(const unsigned long c){//Funcion de iniciacion del I2C del master y sus configuraciones
    SSPCON = 0b00101000;
    SSPCON2 = 0;
    SSPADD = (_XTAL_FREQ/(4*c))-1;
    SSPSTAT = 0;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
}

void I2C_Master_Wait(){//Tiempo de espera mientrar esl Stat y CON tiene el valor estable para hacer la siguiente funcion
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void I2C_Master_Start(){//Iniciacion de master que activa el bit SEN del SSPCON
    I2C_Master_Wait();      
    SSPCON2bits.SEN = 1;    
}

void I2C_Master_RepeatedStart(){
    I2C_Master_Wait();      
    SSPCON2bits.RSEN = 1;   
}

void I2C_Master_Stop(){//Apaagado de master que activa el PEN del SSPCON
    I2C_Master_Wait();     
    SSPCON2bits.PEN = 1;    
}

void I2C_Master_Write(unsigned d){//Guardado de valor variable en el buffer del pic para la transmision del I2C
    I2C_Master_Wait();     
    SSPBUF = d;             
}

unsigned short I2C_Master_Read(unsigned short a){//Lectura del I2C del master
    unsigned short temp;
    I2C_Master_Wait();      //tiempo de espera para tner condiciones estables
    SSPCON2bits.RCEN = 1;  //encendido del RCEN
    I2C_Master_Wait();      //tiempo de espera
    temp = SSPBUF;          //Guardado de valor en el buffer
    I2C_Master_Wait();      //Tiempo de espera
    if(a == 1){//Prueba de Clock para intervcambio de datos del pic
        SSPCON2bits.ACKDT = 0;
    }else{
        SSPCON2bits.ACKDT = 1;
    }
    SSPCON2bits.ACKEN = 1;          
    return temp;                    
}

void I2C_Slave_Init(uint8_t address){ //Inicializacion de Slave
    SSPADD = address; //valor donde se guarda la direccion de los pics
    SSPCON = 0x36;      // 0b00110110
    SSPSTAT = 0x80;     // 0b10000000
    SSPCON2 = 0x01;     // 0b00000001
    TRISC3 = 1;
    TRISC4 = 1;
    GIE = 1;
    PEIE = 1;
    SSPIF = 0;
    SSPIE = 1;
}


