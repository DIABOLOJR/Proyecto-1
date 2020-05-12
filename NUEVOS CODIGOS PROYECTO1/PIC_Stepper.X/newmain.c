/*************************** PALABRA DE CONFIGURACION *************************/
    #pragma config FOSC   = INTRC_NOCLKOUT           
    #pragma config WDTE   = OFF       
    #pragma config PWRTE  = OFF      
    #pragma config MCLRE  = OFF      
    #pragma config CP     = OFF         
    #pragma config CPD    = OFF         
    #pragma config BOREN  = OFF     
    #pragma config IESO   = OFF      
    #pragma config FCMEN  = OFF     
    #pragma config  LVP   = OFF       
    #pragma config BOR4V  = BOR21V   
    #pragma config WRT    = OFF        
    #include <xc.h>    
    #include <stdint.h>        
    #define _XTAL_FREQ 8000000
    #include <xc.h>
    #include <stdint.h>
    uint8_t ESTADO   = 0;
    uint8_t S_ABRIR  = 0;
    uint8_t S_CERRAR = 0;
    uint8_t z        = 0;
    uint8_t REC      = 0;
    uint8_t L_D      = 0;
    uint8_t L_I      = 0;
    uint8_t VALOR    = 0;
    uint16_t D       = 0;
    uint16_t delay   = 15;
    uint8_t LEDROJO  = 0;
    uint16_t LEDS    = 50;
    
/******************************************************************************/
    void setup (void);
/*--------------------------- RUTINA DE INTERRUPCIÓN -------------------------*/    
void __interrupt() isr (void){
    if (INTCONbits.RBIF == 1){
        if (PORTBbits.RB0 == 0){S_ABRIR = 1;}
        if (PORTBbits.RB1 == 0){S_CERRAR= 1;}
        INTCONbits.RBIF = 0;}

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
            SSPBUF = VALOR;             //Enviamos el valor del contador binario
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }PIR1bits.SSPIF = 0;}}
/*----------------------- RUTINA DE INTERRUPCIÓN FIN -------------------------*/  


/****************************** MAIN FUNCTION *********************************/
void main(void) {
    setup();
    //I2C_Slave_Init(0x50);
    
    while(1){// RB3 = Switch de abierto RB2 = switch de cerrado
        if (PORTBbits.RB2 == 0 && PORTBbits.RB3 == 1){PORTDbits.RD4 = 0; PORTDbits.RD5 = 1;} //ESTADO = 1 => CERRADO
        if (PORTBbits.RB2 == 1 && PORTBbits.RB3 == 0){PORTDbits.RD4 = 1; PORTDbits.RD5 = 0;} //ESTADO = 0 => OPEN
        PORTEbits.RE1 = (!PORTBbits.RB5 || !PORTBbits.RB4); // Izquierda
        PORTEbits.RE0 = (!PORTBbits.RB6 || !PORTBbits.RB4);// Derecha
        PORTEbits.RE2 = !PORTBbits.RB7;
        if (S_ABRIR == 1){
            if (PORTBbits.RB3 == 0 && PORTBbits.RB2 == 1){ // En el caso de que el capó esté ya abierto
                for (int a = 0; a <= 5; a++){ // titilará las luces indicando que está abierto
                    if (PORTDbits.RD7){ 
                        PORTDbits.RD7 = 0;
                        PORTDbits.RD6 = 1;}
                    else{
                        PORTDbits.RD7 = 1;
                        PORTDbits.RD6 = 0;}
                    __delay_ms(LEDS);}
                PORTDbits.RD7 = 0;
                PORTDbits.RD6 = 0;}
            if (PORTBbits.RB2 == 0 && PORTBbits.RB3 == 1){ // En el caso de que esté cerrado    
                PORTDbits.RD5 = 0;  
                PORTDbits.RD4 = 0;
                while(PORTBbits.RB3 == 1){
                    if (LEDROJO == 1){
                    LEDROJO = 0;
                    PORTDbits.RD6 = 1;
                    PORTDbits.RD7 = 0;}
                    else {LEDROJO = 1;
                    PORTDbits.RD6 = 0;
                    PORTDbits.RD7 = 1;}                
                    for (int i = 0; i < 4 ;i++){
                        if (i == 0){
                            PORTDbits.RD0 = 1;
                            PORTDbits.RD1 = 0;
                            PORTDbits.RD2 = 0;
                            PORTDbits.RD3 = 0;
                            __delay_ms(delay);}
                        else if (i == 1){
                            PORTDbits.RD0 = 0;
                            PORTDbits.RD1 = 1;
                            PORTDbits.RD2 = 0;
                            PORTDbits.RD3 = 0;
                            __delay_ms(delay);}
                        else if (i == 2){
                            PORTDbits.RD0 = 0;
                            PORTDbits.RD1 = 0;
                            PORTDbits.RD2 = 1;
                            PORTDbits.RD3 = 0;
                            __delay_ms(delay);}
                        else if (i == 3){
                            PORTDbits.RD0 = 0;
                            PORTDbits.RD1 = 0;
                            PORTDbits.RD2 = 0;
                            PORTDbits.RD3 = 1;
                            __delay_ms(delay);}}}
                PORTDbits.RD6 = 0;
                PORTDbits.RD7 = 0;}
            S_ABRIR = 0;//Volverá a esperar a que haya otra solicitud de apertura.
        }
        if (S_CERRAR == 1){
            if (PORTBbits.RB3 == 1 && PORTBbits.RB2 == 0){ // En el caso de que el capó esté ya cerrado
                for (int b = 0; b <= 5; b++){ // titilará las luces indicando que está cerrado
                    if (PORTDbits.RD7){ 
                        PORTDbits.RD7 = 0;
                        PORTDbits.RD6 = 1;}
                    else{
                        PORTDbits.RD7 = 1;
                        PORTDbits.RD6 = 0;}
                    __delay_ms(LEDS);}
                PORTDbits.RD7 = 0;
                PORTDbits.RD6 = 0;}
            if (PORTBbits.RB2 == 1 && PORTBbits.RB3 == 0){ // En el caso de que esté abierto   
                PORTDbits.RD5 = 0;  
                PORTDbits.RD4 = 0;
                while(PORTBbits.RB2 == 1){
                    if (LEDROJO == 1){
                    LEDROJO = 0;
                    PORTDbits.RD6 = 1;
                    PORTDbits.RD7 = 0;}
                    else {LEDROJO = 1;
                    PORTDbits.RD6 = 0;
                    PORTDbits.RD7 = 1;}                
                    for (int i = 4; i > 0 ;i--){
                        if (i == 1){
                            PORTDbits.RD0 = 1;
                            PORTDbits.RD1 = 0;
                            PORTDbits.RD2 = 0;
                            PORTDbits.RD3 = 0;
                            __delay_ms(delay);}
                        else if (i == 2){
                            PORTDbits.RD0 = 0;
                            PORTDbits.RD1 = 1;
                            PORTDbits.RD2 = 0;
                            PORTDbits.RD3 = 0;
                            __delay_ms(delay);}
                        else if (i == 3){
                            PORTDbits.RD0 = 0;
                            PORTDbits.RD1 = 0;
                            PORTDbits.RD2 = 1;
                            PORTDbits.RD3 = 0;
                            __delay_ms(delay);}
                        else if (i == 4){
                            PORTDbits.RD0 = 0;
                            PORTDbits.RD1 = 0;
                            PORTDbits.RD2 = 0;
                            PORTDbits.RD3 = 1;
                            __delay_ms(delay);}}}
                PORTDbits.RD6 = 0;
                PORTDbits.RD7 = 0;}
            S_CERRAR = 0;//Volverá a esperar a que haya otra solicitud de apertura.
        }
    }    
    return;}
/******************************************************************************/

/*************************** RUTINA DE CONFIGURACION  *************************/
void setup (void){
    /*CONFIGURANDO PUERTOS DE ENTRADA Y SALIDA ASÍ COMO VALORES INICIALES*/
    TRISA   = 0b00000000;
    ANSEL   = 0b00000000;
    ANSELH  = 0b00000000;
    TRISB   = 0b11111111;
    TRISC   = 0b00000000;
    TRISD   = 0b00000000;
    TRISE   = 0b0000;
    PORTA   = 0b00000000;
    PORTC   = 0b00000000;
    PORTD   = 0b00000000;
    PORTE   = 0b0000;
    /*************************************************************************/
    /*CONFIGURACIÓ DE INTERRUPCIONES POR IOCB*/
    IOCB    = 0b00000011;
    INTCONbits.RBIF = 0; /*LIMPIEZA DE LA BANDERA DEL IOCB*/
    INTCONbits.RBIE = 1; /*INTERRUPT ENABLE DEL IOCB*/
    INTCONbits.GIE = 1; /*ACTIVACIÓN PRINCIPAL DE INTERRUPCIOENS */  }



