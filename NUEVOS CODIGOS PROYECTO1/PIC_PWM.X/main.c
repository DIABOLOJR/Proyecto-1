/*************************** PALABRA DE CONFIGURACION *************************/
    #pragma config FOSC = INTRC_NOCLKOUT           
    #pragma config WDTE = OFF       
    #pragma config PWRTE = OFF      
    #pragma config MCLRE = OFF      
    #pragma config CP = OFF         
    #pragma config CPD = OFF         
    #pragma config BOREN = OFF     
    #pragma config IESO = OFF      
    #pragma config FCMEN = OFF     
    #pragma config  LVP = OFF       
    #pragma config BOR4V = BOR40V   
    #pragma config WRT = OFF        
    #include <xc.h>
    #include <stdint.h>        /* INCLUSIÓN DE LIBRERÍA DE ANCHO VARS ESTANDAR*/
    #include <math.h>      /* INLUSIÓN DE LA LIBRERÍA DE FUNCIONES MATEMÁTICAS*/
    #include "ADC.h"           /* INCLUSIÓN DE MI LIBRERÍA DEL MANEJOD DE ADC */
    #include "DELAYS.h"        /*  INCLUSIÓN DE MI LIBRERÍA DE DELAYS PATITO  */
    #include "I2C.h"
     #define _XTAL_FREQ 8000000 
/******************************************************************************/
/************ PROTOCOLO DE FUNCIONES, VARIABLES Y DEMÁS DEFINICIONES **********/
void setup (void);
uint8_t cambiante = 0;      /*     VAR DE CONTROL DE CAMBIO DE CANAL ADC      */
uint8_t eADC      = 0;      /* VAR DE CONTROL DENTRO DEL LOOP PRINCIPAL       */
uint8_t ADCAN0    = 0;      /* VAR PARA OBTENER EL VALOR DEL 1ER POTENCIÓMETRO*/
uint8_t ADCAN1    = 0;      /* VAR PARA OBTENER EL VALOR DEL 2O POTENCIÓMETRO */
uint8_t UP_TIME   = 0;
uint16_t NETO_X   = 0;
uint16_t S = 0;
uint8_t TC = 0;
uint8_t TIME_UP = 0;
uint8_t z = 0;
uint8_t REC = 0;
uint8_t POSICION = 0;
uint16_t AGREGADO = 0;
/******************************************************************************/
/************************* RUTINA DE INTERRUPCIÓN *****************************/
void __interrupt() isr (void){
    if (INTCONbits.T0IF == 1){ /*SI EL TMR0 HA SUFRIDO DESBORDE, T - T0=0.1mS*/        
        TIME_UP++;
        TC++;
        INTCONbits.T0IF = 0;
        TMR0 = 231;}
    
    if(PIR1bits.SSPIF == 1){
       SSPCONbits.CKP = 0;
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1; }        // Enables SCL (Clock)
        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            REC = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            __delay_us(250);
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = POSICION;             //Enviamos el valor del contador binario
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }       
        PIR1bits.SSPIF = 0;}}

/******************************************************************************/
void main(void){
    setup();        /************* FUNCIÓN DE CONFIGURACIÓN INICIAL ***********/ 
    NETO_X = 3;
    CONFIG_ADC();
    PORTDbits.RD0 = 1;              /*PIN PARA PRIMER PWM*/
    while(1){       /*************** MAIN GLORIUS SPARTAN PROGRAM *************/
        if (TIME_UP == 90){
            TIME_UP = 0;
            PORTDbits.RD0 = 1;
            S++;
            if (cambiante == 0){ADCON0bits.CHS0 = 1;} /*ESTE ARREGLO DE IFS, DEJA */
            if (cambiante == 1){ADCON0bits.CHS0 = 0;} /*CAMBIAR DE CANAL CADA 1ms */
            cambiante++;                              /*LA VAR CONTROL QUE CAMBIA */
            if (cambiante > 1){cambiante = 0;}
            if (ADCON0bits.GO_DONE == 0){eADC = 1;}
            
            if (S >= 15){
                S = 0;
            if (ADCAN0 < 42){NETO_X = 3;}
            else if (ADCAN0 > 42 && ADCAN0 < 84){NETO_X = 4;}
            else if (ADCAN0 > 84 && ADCAN0 < 126){NETO_X = 5;}
            else if (ADCAN0 > 126 && ADCAN0 <168){NETO_X = 6;}
            else if (ADCAN0 > 168 && ADCAN0 <215){NETO_X = 7;}
            else {NETO_X = 8;}
                
           
            }}
        if (TIME_UP > NETO_X){
            PORTDbits.RD0 = 0;}
        if (eADC == 1){
            eADC = 0;
            if (cambiante == 0){ADCAN0 = ADRESH;} /*MEDICIÓN DE EJE X*/
            if (cambiante == 1){ADCAN1 = ADRESH;} /*MEDICIÓN DE EJE Y*/
            ADCON0bits.GO_DONE = 1;}                  
        }return;}
/******************************************************************************/
/*************************** RUTINA DE CONFIGURACION  *************************/
void setup (void){
/*****CONFIGURANDO PUERTOS DE ENTRADA Y SALIDA ASÍ COMO VALORES INICIALES *****/
    TRISA  = 0b00000011;/*Todos los pines del puerto exepto RA0 y RA1 como OUT*/
    ANSEL  = 0b00000011;           /*Utilizaré el canal AN[0,1] del modulo ADC*/ 
    ANSELH = 0b00000000;           /*     Uso nulo del puerto B como ANIN     */
    TRISB  = 0b00000000;           /*    Todo el PORTB declarado como OUTPUT  */
    TRISC  = 0b00000000;           /*    Todo el PORTC declarado como OUTPUT  */
    TRISD  = 0b00000000;           /*    Todo el PORTC declarado como OUTPUT  */
    TRISE  = 0b0000;               /*    Todo el PORTE declarado como OUTPUT  */
    PORTB  = 0b00000000;           /*    Todo el PORTB colocado a 0v          */
    PORTC  = 0b00000000;           /*    Todo el PORTC colocado a 0v          */
    PORTD  = 0b00000000;           /*    Todo el PORTD colocado a 0v          */
    PORTE  = 0b0000;               /*    Todo el PORTE colocado a 0v          */
/******************************************************************************/
/************** CONFIGURACIÓN DE TIMER 0 Y DE SUS INTERRUPCIONES **************/
    OPTION_REGbits.T0CS = 0; /* TMR0 Clock Selector-> 0 = FOSC/4, 1 = EXTERNAl*/
    OPTION_REGbits.PSA  = 0; /* Prescaler selector-> 0 = TMR0 Module, 1 = WDT */
    OPTION_REGbits.PS0  = 0; /* PS[2:0] definen el preescalador del TMR0      */
    OPTION_REGbits.PS1  = 1; /* 0      1     0     -> 1:8                     */ 
    OPTION_REGbits.PS2  = 0; /* PS2   PS1   PS0                               */
    INTCONbits.T0IE     = 1; /* TMR0 INTERRUPT ENABLE-> 0 = nope, 1 = yes     */
    INTCONbits.T0IF     = 0; /* TMR0 INTERRUPT FLAG                           */
    INTCONbits.GIE      = 1; /* INTERRUPCIONES GLOBALES ACTIVADAS             */
    TMR0                = 231;/* Valor precargado para obtener 0.1ms de TMR0  */
/******************************************************************************/}