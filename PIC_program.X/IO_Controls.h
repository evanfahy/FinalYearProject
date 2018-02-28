/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef IO_CONTROLS_H
#define	IO_CONTROLS_H

//Function Declarations
void InitialisePorts (void);    //Setup I/O ports
void LEDPin(char OnOff);        //direct Pin control
void PIN3(char OnOff);
void PIN6(char OnOff);

//  Port Variables
//  ************* PORT A *************
union {
    unsigned char portAByte;
    struct{
        unsigned TxPin:1;               //RA0 (PGM)UART TX (& Programming Data line)
        unsigned RxPin:1;               //RA1 (PGM)UART RX (& Programming Clock line)
        unsigned RCD_FaultPinDIN:1;     //RA2 (DI) INTERRUPT PIN - RCD Fault Input
        unsigned ResetButtonPinDin:1;   //RA3 (DI) Reset Button Input (shared with MCLR=>IN only)
        unsigned Pin3:1;                //RA4 (DO) Avail for TEST/DEBUG
        unsigned ShortTripSCRPinOut:1;  //RA5 (DO) Short Trip SCR
        unsigned NoPin_RA6:1;           //no such port pin, filling up Byte with extra bit
        unsigned NoPin_RA7:1;           //no such port pin, filling up Byte with extra bit
    } bits;
}portA;

//  ************* PORT C *************
union {
    unsigned char portCByte;
    struct{
        unsigned VccSensePinAIN:1;     //RC0 (A4) AI VCC Level Sense
        unsigned FaultLEDPinOut:1;     //RC1 (DO) Fault LED
        unsigned ClosePinOut:1;        //RC2 (DO) Output to close M7
        unsigned MainsSensePinAIN:1;   //RC3 (A7) Mains Level Sense
        unsigned Pin6:1;               //RC4 (DO) Avail for TEST/DEBUG
        unsigned Pin5:1;               //RC5 (DO) trip M7 Output
        unsigned NoPin_RC6:1;           //no such port pin, filling up Byte with extra bit
        unsigned NoPin_RC7:1;           //no such port pin, filling up Byte with extra bit
    } bits;
}portC;

//Function Definitions
void InitialisePorts(void){

    OPTION_REG = 0b10000000;
    //bit7:0=weak pull-up enable
    //bit6:0=falling edge interrupt
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
    TRISA  = 0b00011000;     // 0 = Digital Outputs; 1 = Digital IN.
                             // RA3 pin4 bit is always ?1? as RA3 is an input only
    ANSELA = 0b00000000;     // Disable all port A Analog inputs (1=Analog In)
    
    
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

void PIN5(char OnOff){

    portC.portCByte = PORTC;
    portC.bits.Pin5 = OnOff;
    PORTC=portC.portCByte;
    return; 
}

#endif	/* IO_CONTROLS_H */

