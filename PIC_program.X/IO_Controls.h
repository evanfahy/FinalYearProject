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
void TogglePIN6(void);
void TogglePIN5(void);

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

#endif	/* IO_CONTROLS_H */

