/*
 * File:   IO_Controls.c
 * Author: Evan
 *
 * Created on 05 March 2018, 19:09
 */


#include <xc.h>
#include "IO_Controls.h"

//Function Definitions
void InitialisePorts(void){

    OPTION_REG = 0b11000000;
    //bit7:0 = weak pull-up enable
    //bit6:1 = Rising edge interrupt
    //others for timers
    
    //ensure weak-pull-ups are disabled if not needed to avoid random glitches
    //The ports automatically start up as tri-state inputs and are enabled as analog inputs
    //Ensure IO ports are configured to output zeros before enabling as outputs
    
    //************* PORT A *************
    // A0 TxPin (PGM)        Digital In
    // A1 RxPin (PGM)        Digital In
    // A2 RCD_FaultPinDIN    Digital In/ INTERRUPT PIN
    // A3 ResetButtonPinDIN  Digital In
    // A4 TestPin1Out        Digital Out
    // A5 ShortTripSCRPinOut Digital Out
    
    portA.portAByte = 0;     // initialise the A shadow register to zero
    LATA   = 0;              // Switch off all port A output pins before enabling
    TRISA  = 0b00011100;     // 0 = Digital Outputs; 1 = Digital IN.
                             // RA3 pin4 bit is always ?1? as RA3 is an input only
                             // RA2 (INTPIN) is set to input
    ANSELA = 0b00000000;     // Disable all port A Analog inputs (1=Analog In)
                             // RA2 Digital IN
    
    
    //INLVLA2 =1;             // select Schmitt Trigger levels for IOC A2 pin
    //************* PORT A end *************
    
        //************* PORT C *************
    // C0 VccSensePinAIN    Analog In -AN4
    // C1 FaultLEDPinOut    Digital Out
    // C2 ClosePinOut       Digital Out
    // C3 MainsSensePinAIN  Analog In -AN7
    // C4 TestPin2Out        Digital Out
    // C5 Trip M7 Digital Out
    
    portC.portCByte = 0;    // initialise the C shadow register to zero
    LATC = 0;               // Switch off all port C output pins before enabling
    TRISC  = 0b00001000;    // 0 = Digital Outputs; 1 = Digital IN.
                            //AN7 / RC3 and AN6 / RC2 set to Analog IN 
    ANSELC = 0b00001000;    // Disable all port C Analog inputs (1=Analog In)
                            // Enable PortC.3 as Analog: AN7 (pin7) (RC3:Positive cycle Sense)
                            // Enable PortC.2 as Analog: AN6 (pin8) (RC2:Negative cycle Sense)
  
    //************* PORT C end *************

    return;
}


void PIN3(char OnOff){

    //read, write, modify
    portA.portAByte = PORTA;//reads port A val into mask
    portA.bits.Pin3 = OnOff;//change bit in mask
    PORTA=portA.portAByte;//write to mask
    return;
}

void PIN6(char OnOff){

    portC.portCByte = PORTC;
    portC.bits.Pin6 = OnOff;
    PORTC=portC.portCByte;
    return; 
}

void TogglePIN6(void){

    portC.portCByte = PORTC;
    if(portC.bits.Pin6 == 1){
       
        portC.bits.Pin6 = 0;
    }
    else{
        portC.bits.Pin6 = 1;
    }
     PORTC=portC.portCByte; 
     return;
}

void TogglePIN5(void){

    portC.portCByte = PORTC;
    if(portC.bits.Pin5 == 1){
       
        portC.bits.Pin5 = 0;
    }
    else{
        portC.bits.Pin5 = 1;
    }
     PORTC=portC.portCByte; 
     return;
}