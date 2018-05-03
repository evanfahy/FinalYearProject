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

#include <xc.h> // include processor files - each processor file is guarded.

//Function Declarations
void InitialisePorts (void);    //Setup I/O ports


void SCR(char );
void ToggleSCR(void );
void SCR_Relay(char );
void ToggleSCR_Relay(void);
void ADC_OPTO_Relay(char );
void ToggleADC_OPTO_Relay(void);
void Fail_LED(char );
void PassLED(char );
void ToggleGLED(void);
void ToggleRLED(void);



//  Port Variables
//  ************* PORT A *************
union {
    unsigned char portAByte;
    struct{
        unsigned TxPin:1;               //RA0 ART TX (& Programming Data line)
        unsigned RxPin:1;               //RA1 ART RX (& Programming Clock line)
        unsigned PosCycleINT:1;         //RA2 INTERRUPT PIN
        unsigned ResetButtonPin:1;      //RA3 Reset Button Input
        unsigned NegCycleINT:1;         //RA4 INT on change 
        unsigned PassLED:1;             //RA5 
        unsigned NoPin_RA6:1;           //no such port pin, filling up Byte with extra bit
        unsigned NoPin_RA7:1;           //no such port pin, filling up Byte with extra bit
    } bits;
}portA;

//  ************* PORT C *************
union {
    unsigned char portCByte;
    struct{
        unsigned TestTrigger:1;     
        unsigned SCR_Relay:1;     
        unsigned SCR:1;               
        unsigned MainsSenseADC:1;   //RC3 (A7) Mains Level Sense
        unsigned ADC_OPTORelay:1;               
        unsigned Fail_LED:1;               
        unsigned NoPin_RC6:1;           //no such port pin, filling up Byte with extra bit
        unsigned NoPin_RC7:1;           //no such port pin, filling up Byte with extra bit
    } bits;
}portC;

#endif	/* IO_CONTROLS_H */

