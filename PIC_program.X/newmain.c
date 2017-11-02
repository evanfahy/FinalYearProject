/*
 * File:   newmain.c
 * Author: Evan
 *
 * Created on 25 October 2017, 09:46
 */

#define _XTAL_FREQ 100000  //Declare Oscillator value (0.1Mhz) for use in __delay_ms, etc.

#include <xc.h>
#include <math.h>
#include "Constants.h"
#include "DeviceConfig.h"
#include "IO_Controls.h"


void main(void) {
    
    // Setup Oscillator for 1MHz
    OSCCON = 0b01011000;    // SPLLEN IRCF2 IRCF2 IRCF1 IRCF0 unused SCS1 SCS0
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
    GIE = Disable;          // Disable Global Interrupt
    
    InitialisePorts();      // routine to setup ports
    
    //Pin6_OFF;
    
    while (1){
        
        PIN6(1);
        __delay_ms(5000);
        PIN6(0);
        __delay_ms(5000);
    }
   
    //return (0);
}
