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
void beginHighCurrentTest(void);
void interrupt ISR (void);

//Global Scope
volatile int sampleVal = 0, count = False, startFlag = False, posFlag = 1, negFlag = 0, zeroCount = 0, printCount = 0;
int tmr1Count = 0;

int main() 
{
    
    initPIC();
    
    for(int i = 0; i < 6; i++){
    
        TogglePIN6();
        __delay_ms(250);
    }
    
    while(1) 
    {
                
        //verifyACSignal();
        pi3Connect();
        
    }
    
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

    __delay_ms(500);
    
    return;
}


void pi3Connect(void)
{
    PIE1bits.RCIE = Enable;                                 //enable USART receive interrupt
    INTCONbits.PEIE = Enable;                               //enables all active peripheral interrupts
    
    RCSTAbits.SPEN = Enable;                                //enable USART
    RCSTAbits.CREN = Disable; RCSTAbits.CREN = Enable;      // resets continuous receiver
    
    while(1){
    
        
//        printf("Hey");
//        __delay_ms(5000);
    }
    
    return;
}


void verifyACSignal(void)
{
     ADON=Enable;       //ADC enable
     
     aveTmr1Val = 0;
     
         PIN5(Clear);
         // 100 is arbitrary
     while(tmr1Count < 100 ){
         
         adcSample = ReadADC();
        
        if((adcSample > 512) && (startFlag == False)) {
        
            //sampleVal = adcSample;
            
            if(count == 0) {
            
                T1CONbits.TMR1ON = Enable;   //Timer on
                PIN6(Set);
                count = 1;
            }
            else {
                
                PIN6(Clear);
                tmr1Val = Timer1();           //Function call to calculate time
                aveTmr1Val = aveTmr1Val + tmr1Val;
                
                tmr1Count = tmr1Count + 1 ;
                count = 0;
            }
            
            //Timer has been started
            startFlag = True;
        }
        else if((adcSample < 128) && (startFlag == True)) {
            
            //Timer will stop on when ADC_sample > 512
            startFlag = False;
        }
        else{}
     }
     
     ADON=Disable;
     tmr1Count = 0;
     aveTmr1Val = fabs((double)(aveTmr1Val/100));
    PIN5(Set);
    
    //Send to Pi
    
    return;
}


void beginHighCurrentTest(void)
{
    INTCONbits.INTE = Enable;             //Enable External interrupt
    INTCONbits.IOCIE = Enable;                 // Enable interrupt-on-change

    return;
}


void interrupt ISR (void)
{
    //Positive Cycle Zero Cross
    if((INTCONbits.INTF == 1) && (INTCONbits.INTE == 1)){
       

         if((PORTAbits.AN2 == 1)){
            
            //if((T1CONbits.TMR1ON == Disable) /* && (zeroCount == 0)*/)
                
             if(count == 0){
                 
                T1CONbits.TMR1ON = Enable;
                count++;
             }
             PIN6(Set);
             PIN5(Clear);
         }
        INTCONbits.INTF = 0;
    }
    
    //Positive Cycle Zero Cross
    else if(( IOCAFbits.IOCAF4 == 1 ) && (INTCONbits.IOCIE == 1)){
    
        if((PORTAbits.AN3 == 1)){
            
            //if((T1CONbits.TMR1ON == Disable) /* && (zeroCount == 0)*/){
            
                //T1CONbits.TMR1ON = Enable;
            if(count == 1){
                tmr1Val = Timer1();
                printCount++;
                count = 0;
                if(printCount == 200)
                {        
                    //printf("Time:%d", time_16bit);
                    printCount = 0;
                }
            }
            
            PIN5(Set);
            PIN6(Clear);
           
        }
        
        IOCAFbits.IOCAF4 = 0;   //clears flag
    }
    
    else if (PIR1bits.RCIF){
        
        TogglePIN5();
            
        charData = getch(); // read received character to buffer getch
//        printf("%c\0", myCharData);
//        TXREG = 0;

        //myIntData = (unsigned int)(myCharData);
        
        printf("%c\0", charData);

        RCREG = 0;
        PIR1bits.RCIF = 0; // reset receive interrupt flag
    }
    
    return;
}