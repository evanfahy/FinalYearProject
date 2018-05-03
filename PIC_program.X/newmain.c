/*
 * File:   newmain.c
 * Author: Evan Fahy
 *
 * Created on 25 October 2017, 09:46
 */

#define _XTAL_FREQ          4000000  //Declare Oscillator value (0.1Mhz) for use in __delay_ms, etc.
#define _TMR1_cnt           65536    // value to be subtracted away from timer0 register (max of 65535 allowed)

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Constants.h"
#include "DeviceConfig.h"
#include "IO_Controls.h"
#include "ADC.h"
#include "USART.h"
#include "Timer.h"

void initPIC(void);
void pi3Connect(void);
void verifyACSignal(void);
void setupTestParameters(void);
void beginHighCurrentTest(void);
void interrupt ISR (void);

//Global Scope
//volatile keyword is a qualifier that is applied to a variable when it is declared. It tells the compiler that the value of the variable may change at any time
//without any action being taken by the code the compiler finds nearby.
volatile unsigned short sampleVal = 0, ADC_Test = False, tmr1StartFlag = False, tmr1Samples = 0, tmr1Start = False, 
                        mainTest = False, PosCycle = False, NegCycle = False, SCR_On = False;
unsigned short rxFlag = 0;
unsigned long PoW = 0;
unsigned short cycles = 0;

int main() 
{
    
    initPIC();
    
    for(int i = 0; i < 6; i++){
    
        ToggleGLED();
        __delay_ms(100);
    }
      
   pi3Connect();
    __delay_ms(10);
   verifyACSignal();
    
    
    return (0);
}

void initPIC(void)
{
    
    // Setup Oscillator for 4MHz
    OSCCON = 0b01101000;    // SPLLEN IRCF2 IRCF2 IRCF1 IRCF0 unused SCS1 SCS0
                            // D7:       SPLLEN=0 => 4xPLL disabled
                            // D6-D3:    IRCF3-IRCF0 selects frequency
                            // eg    1010 = 500KHz (MF-Power up default)
                            //      1011 = 1MHz HF
                            //      1101 = 4MHz HF
                            // D2:       unused
                            // D1-D0     SCS1-SCS0 = Sys Clock Select
                            //              (00= determined by configuration Word 1 FOSC<1:0>)
    
    // Wait for the HF oscillator to be ready
    while(HFIOFR==0){       //waits for high frequency oscillator to begin
    }
    
    // Ensure interrupt disabled while starting up
    INTCON = 0b00000000;            
    INTCONbits.GIE = Disable;       // Disable Global Interrupt
    
    InitialisePorts();              // routine to setup ports
    Init_TMR1();
    InitialiseADC();
    InitUSART();

    INTCONbits.GIE = Enable;        //Enable Global Interrupt
    
    return;
}


void pi3Connect(void)
{
    PIE1bits.RCIE = Enable;                                 //enable USART receive interrupt
    INTCONbits.PEIE = Enable;                               //enables all active peripheral interrupts
    
    RCSTAbits.SPEN = Enable;                                //enable USART
    RCSTAbits.CREN = Disable; RCSTAbits.CREN = Enable;      // resets continuous receiver
    
    unsigned char handshake = '0';
   
    //Waits for a message from the Pi3
    while(handshake != '1'){
    
        handshake = rxArray[0];
        Fail_LED(Set);
    }
    rxFlag = 0;
            
    //PIE1bits.RCIE = Disable;                                 //enable USART receive interrupt
    empty_RX_BUFF();
    Fail_LED(Clear);
    
    //Sends message back to alert Pi3 is connected
    __delay_ms(10);
    UART_TX(handshake);
    
    return;
}

void verifyACSignal(void)
{   
    unsigned char handshake2 = '0';
    ADC_Test = True;
        
    PIE1bits.RCIE = Enable;                                 //enable USART receive interrupt
    
    //Waits for a message from the Pi3
    while(handshake2 != '2'){
    
        handshake2 = rxArray[0];
    }
    rxFlag = 0;
    empty_RX_BUFF();

    //Sends message back to alert Pi3 is connected
    __delay_ms(10);
    UART_TX(handshake2);
    PIE1bits.RCIE = Disable;                                 //enable USART receive interrupt
    ADON=Enable;       //ADC enable
    
    while(!ADON){} 
    aveTmr1Val = 0;
     
     //Begin ADC Test
     // 100 is arbitrary
    while(tmr1Samples < 100 ){
         
        adcSample = ReadADC();
        
        if((adcSample > 512) && (tmr1Start == False)) {
                    
            if(tmr1StartFlag == 0) {
            
                T1CONbits.TMR1ON = Enable;   //Timer on
                tmr1StartFlag = Set;
            }
            else {
                
                tmr1Val = Timer1();           //Function call to calculate time
                aveTmr1Val = aveTmr1Val + tmr1Val;
                
                tmr1Samples = tmr1Samples + 1 ;
                tmr1StartFlag = Clear;
            }
            
            //Timer has been started
            tmr1Start = True;
        }
        else if((adcSample < 128) && (tmr1Start == True)) {
            
            //Timer will stop on when ADC_sample > 512
            tmr1Start = False;
        }
        else{}
    }
     
    ADON=Disable;
    tmr1Samples = 0;
    tmr1Val = 0;
    aveTmr1Val = fabs((double)(aveTmr1Val/100));
    
    //Tmr1Val_Ms = fabs((double)(aveTmr1Val*2)/1000); // to display in ms
    
    if((aveTmr1Val >= 4750) && (aveTmr1Val <= 5250)){
    
        txByte = '1';
        //swap mains to OPTOCOUPLER circuit
        ADC_OPTO_Relay(Set);

    }
    else{
        
        txByte = '0';
    }
    
    PIE1bits.RCIE = Enable;                                 //enable USART receive interrupt     

    
    //Send to Pi
     UART_TX(txByte);
     __delay_ms(1800);
     //PIE1bits.RCIE = Disable;                                 //enable USART receive interrupt
     printf("%d\n", aveTmr1Val);
     //printf("Half cycle: %d ms\n", Tmr1Val_Ms); 
     empty_RX_BUFF();
     
    if((aveTmr1Val >= 4750) && (aveTmr1Val <= 5250)){
        
        //ADC flaged over
        ADC_Test = False; 
    
        for(int i = 0; i < 6; i++){
    
            ToggleGLED();
            __delay_ms(100);
        }
        //Switch AC to main test
        ADC_OPTO_Relay(Set);
        //Calls next function
        setupTestParameters();  
    }
    else{}
     //}

    return;
}

void setupTestParameters(void)
{
    //PIE1bits.RCIE = Enable;                                 //enable USART receive interrupt
    INTCONbits.PEIE = Enable;                               //enables all active peripheral interrupts
    
    //RCSTAbits.SPEN = Enable;                                //enable USART
    RCSTAbits.CREN = Disable; RCSTAbits.CREN = Enable;      // resets continuous receiver
    
    unsigned char handshake3 = '0';
    
    PassLED(1);
    
    while(handshake3 != '3'){
    
        handshake3 = rxArray[0];
    }
    rxFlag = 0;
    PassLED(0);
   
    __delay_ms(10);
    UART_TX(rxArray[0]);
    empty_RX_BUFF();
    
    //wait for PoW value
    
    while(rxArray[0] == '\0'){}
    rxFlag = 0;
    
    __delay_ms(10);
    UART_TX_String(rxArray);

    PoW = atoi(rxArray);
    //printf("%c", rxArray);
    empty_RX_BUFF();
    
    //wait for number of cycles
    while(rxArray[0] == '\0'){}
    rxFlag = 0;
    
    __delay_ms(10);
    UART_TX_String(rxArray);
    cycles = atoi(rxArray);
    //printf("%c", rxArray);
    empty_RX_BUFF();

        
    PoW = ((PoW*100)/360);    //PoW is now a percentage of 360 degrees
    PoW = ((10000/100)*PoW);   //PoW is now the percentage of full 20ms cycle (10000 clock cycles)
    
    //begin test
    beginHighCurrentTest();
   
    return;
}


void beginHighCurrentTest(void)
{
    mainTest = True;
    PIE1bits.RCIE = Disable;               //Disables RX 
        
    //When POS_INT occurs start timer
    //Constantly check when timer == PoW
    //When timer == PoW drive transistor to Thyristor
    //Switching Thyristor relay during cycles for numOfCycles
    //Finish test
    //Alert Pi
    //Send time taken to Pi3
    //Examine scope
    //Done
    
    //Polarity of Transformer is flipped
    //SCR will only let negative cycle flow
    if(PoW > 5000)
    {
    
        SCR_Relay(Set);
        NegCycle = True;
        
    }
    else if (PoW < 4999){
        
        SCR_Relay(Clear);
        PosCycle = True;
//        for(int i = 0; i < 6; i++){
//            
//            ToggleGLED();
//            __delay_ms(250);
//         }
    }
    
    
    for(int i = 0; i < cycles; i++){
    
        //SCR(Clear);
   
        INTCONbits.INTE = Enable;             //Enable External interrupt
        INTCONbits.IOCIE = Enable;            // Enable interrupt-on-change
    
        while(SCR_On <= PoW)
        {
            SCR_On = Timer1Read();
        }
        INTCONbits.INTE = Disable;             //Enable External interrupt
        INTCONbits.IOCIE = Disable;            // Enable interrupt-on-change
        SCR(Set);
        SCR_On = 0;
        TMR1Reset();
    }
    
    NegCycle = False;
    PosCycle = False;
    SCR(Clear);
    SCR_Relay(Clear);
    ADC_OPTO_Relay(Clear);
   

    return;
}

void interrupt ISR (void)
{
    //External Interrupt
    //Positive Cycle Zero Cross
    if((INTCONbits.INTF == 1) && (INTCONbits.INTE == 1)){
       
         if((PORTAbits.RA2 == 1)){
                            
             if((ADC_Test == True) && (tmr1StartFlag == 0)){
                 
                TMR1ON = Enable;
                tmr1StartFlag++;
                Fail_LED(Clear);
                PassLED(Set);
             }
             else if(PosCycle == True){
                 
               TMR1ON = Enable; 
            }    
         }
        INTCONbits.INTF = 0;
    }
    
    //Interrupt on Change Interrupt
    //Positive Cycle Zero Cross
    else if(( IOCAFbits.IOCAF4 == 1 ) && (INTCONbits.IOCIE == 1)){
    
        if((PORTAbits.RA4 == 1)){
            
            if((ADC_Test == True) && (tmr1StartFlag == 1)){
                tmr1Val = Timer1();
                Fail_LED(Set);
                PassLED(Clear);
                tmr1StartFlag = 0;
            }
            else if(NegCycle == True){
                
               TMR1ON = Enable; 
            }
           
        }
        
        IOCAFbits.IOCAF4 = 0;   //clears flag
    }
    
    //UART RX Interrupt routine
    else if (PIR1bits.RCIF){
        rxFlag = 1;
        if((RCSTAbits.OERR == 1) || (RCSTAbits.FERR == 1)){
            
            //OERR is the error
            RCSTAbits.CREN = Disable; RCSTAbits.CREN = Enable;      // resets continuous receiver
            Fail_LED(Set);
        }
        else{

            UART_RX_String();
        }

    }
    
    return;
}