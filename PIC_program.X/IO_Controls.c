/*
 * File:   IO_Controls.c
 * Author: Evan
 *
 * Created on 05 March 2018, 19:09
 */

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
    
    
    portA.portAByte = 0;     // initialise the A shadow register to zero
    LATA   = 0;              // Switch off all port A output pins before enabling
    
    TRISA  = 0b00011110;     /// 0 = Digital Outputs; 1 = Digital IN.
                             // RA5 is set to digital output for PASS LED
                             // RA4 is set to digital input for for INT_NEG (Interrupt-on-change)
                             // RA3 pin4 bit is always ?1? as RA3 is an input only
                             // RA2 is set to digital input for INT_POS
                             // RA1 (RXPIN) is set to digital input
                             // RA0 (TXPIN) is set to digital output
    
    ANSELA = 0b00000000;     // Disable all port A Analog inputs (1 = Analog In/Out, 0 = digital In/Out)
    
    IOCAPbits.IOCAP4 = 1;   //Enable IOCIE on RA4/Pin3

    
    portC.portCByte = 0;    // initialise the C shadow register to zero
    LATC = 0;               // Switch off all port C output pins before enabling
    TRISC  = 0b00001000;    // 0 = Digital Outputs; 1 = Digital IN.
                            //AN7 / RC3 and AN6 / RC2 set to Analog IN for ADC 
    ANSELC = 0b00001000;    // Disable all port C Analog inputs (1=Analog In)
                            // Enable PortC.3 as Analog: AN7 (pin7) (RC3:Positive cycle Sense)
  
    //************* PORT C end *************

    return;
}


void SCR(char OnOff){
    
    portC.portCByte = PORTC;
    portC.bits.SCR = OnOff;
    PORTC = portC.portCByte;
    return;

}

void ToggleSCR(void){

    portC.portCByte = PORTC;
    if(portC.bits.SCR == 1){
       
        portC.bits.SCR = 0;
    }
    else{
        portC.bits.SCR = 1;
    }
     PORTC=portC.portCByte; 
     return;
}

void ADC_OPTO_Relay(char OnOff){

    portC.portCByte = PORTC;
    portC.bits.ADC_OPTORelay = OnOff;
    PORTC=portC.portCByte;
    return; 
}

void ToggleADC_OPTO_Relay(void){

    portC.portCByte = PORTC;
    if(portC.bits.ADC_OPTORelay == 1){
       
        portC.bits.ADC_OPTORelay = 0;
    }
    else{
        portC.bits.ADC_OPTORelay = 1;
    }
     PORTC=portC.portCByte; 
     return;
}

void SCR_Relay(char OnOff){

    portC.portCByte = PORTC;
    portC.bits.SCR_Relay = OnOff;
    PORTC=portC.portCByte;
    return; 
}

void ToggleSCR_Relay(void){

    portC.portCByte = PORTC;
    if(portC.bits.SCR_Relay == 1){
       
        portC.bits.SCR_Relay = 0;
    }
    else{
        portC.bits.SCR_Relay = 1;
    }
     PORTC=portC.portCByte; 
     return;
}

void Fail_LED(char OnOff){

    portC.portCByte = PORTC;
    portC.bits.Fail_LED = OnOff;
    PORTC=portC.portCByte;
    return; 
}

void ToggleRLED(void){

    portC.portCByte = PORTC;
    if(portC.bits.Fail_LED == 1){
       
        portC.bits.Fail_LED = 0;
    }
    else{
        portC.bits.Fail_LED = 1;
    }
     PORTC=portC.portCByte; 
     return;
}

void PassLED(char OnOff){

    portA.portAByte = PORTA;
    portA.bits.PassLED = OnOff;
    PORTA=portA.portAByte;
    return; 
}

void ToggleGLED(void){

    portA.portAByte = PORTA;
    if(portA.bits.PassLED == 1){
       
        portA.bits.PassLED = 0;
    }
    else{
        portA.bits.PassLED = 1;
    }
     PORTA=portA.portAByte; 
     return;
}

