/*
 * File:   newmain.c
 * Author: Evan Fahy
 *
 * Created on 25 October 2017, 09:46
 */

#define _XTAL_FREQ          4000000  //Declare Oscillator value (0.1Mhz) for use in __delay_ms, etc.
#define _TMR1_cnt           65536    // value to be subtracted away from timer0 register (max of 65535 allowed)
#define START               1
#define FINISH              0
#define RUNNING             2

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "Constants.h"
#include "DeviceConfig.h"
#include "IO_Controls.h"
#include "ADC.h"
#include "USART.h"
#include "Timer.h"
#include "Variables.h"

void interrupt ISR (void);

//Global Scope
volatile int sampleVal = 0, count = False, startFlag = False, zeroCount = 0;


int main() {
    
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
    INTCON = 0b00000000;    // bit7= Global Int Enable; bit4= Int Pin ENable (RA2, Pin11)
    INTCONbits.GIE = Disable;          // Disable Global Interrupt
    
    InitUSART();
    InitialisePorts();      // routine to setup ports
    InitialiseADC();
    ADON=Enable;            //ADC enable
    //Init_TMR0();
    Init_TMR1();
    
    INTCONbits.INTE = Enable;           //Enable External interrupt
    INTCONbits.GIE = Enable;            //Enable Global Interrupt
    INTCONbits.INTF = 0;                //make interrupt flag clear.
    
    unsigned short ADC_sample;  //16bit
    
    while(1) {
                
        //ADC_sample = ReadADC();
        
        if((ADC_sample > 768) && (startFlag == False)) {
        
            sampleVal = ADC_sample;
            
            if(count == 0) {
            
                T1CONbits.TMR1ON = Enable;   //Timer on
                PIN6(Set);
                count = 1;
            }
            else {
                
                PIN6(Clear);
                time_16bit = Timer1();           //Function call to calculate time
                //printf("Time: %d\r", time_16bit);
                count = 0;
            }
            
            //Timer has been started
            startFlag = True;
        }
        else if((ADC_sample < 128) && (startFlag == True)) {
            
            //Timer will stop on when ADC_sample > 768
            startFlag = False;
        }
        else{}
        
    }
    
    return (0);
}

void interrupt ISR (void)
{
    if( (INTCONbits.INTF==1) && (INTCONbits.INTE==1) ){
        
        if((PORTAbits.AN2 == 1)){
        
            zeroCount++;
            //Toggles at every period
            if(zeroCount == 2){
                
                zeroCount = 0;
                TogglePIN6();
            }
        }
    }
    INTCONbits.INTF = 0;
    
    return;
}