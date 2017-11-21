/*
 * File:   newmain.c
 * Author: Evan
 *
 * Created on 25 October 2017, 09:46
 */

#define _XTAL_FREQ 1000000  //Declare Oscillator value (0.1Mhz) for use in __delay_ms, etc.

#include <xc.h>
#include <math.h>
#include "Constants.h"
#include "DeviceConfig.h"
#include "IO_Controls.h"


int main() {
    
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
 
    while(1){
        
        char temp = PORTA;
        
        //if(portA.bits.Pin3 == 0){
        
        if((PORTA & 0x10) != 0) {
            
            PIN6(Set);
            //portC.bits.Pin6 = 1;
            //PORTC=portC.portCByte;
            __delay_ms(200);   //#define __delay_ms(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000.0)))
                            // NOTE: To use the __delay_ms() function, YOU must have previously defined _XTAL_FREQ

            PIN6(Clear);
            //portC.bits.Pin6 = 0;
            //PORTC=portC.portCByte;
            __delay_ms(200);
        }
            
    //}
    }
   
   
    return (0);
}
