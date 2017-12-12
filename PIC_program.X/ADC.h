// This is a guard condition so that contents of this file are not included
// more than once. 

#include "Constants.h"

#ifndef ADC_H
#define	ADC_H

//Function Declarations
void InitialiseADC(void);
unsigned short ReadADC(void);

//Function Definitions
void InitialiseADC(void){

    ADCON0bits.CHS = 0b0111;    //selects Pin7 (AN7)
    ADFM = Set;                 //Right Justified results
    ADCON1bits.ADCS = 0b000;    //selects clock = Fosc/2
    //ADCON1bits.ADNREF = 0;    //connects -Ref to Vss
    ADCON1bits.ADPREF = 0b011;  //connects +Ref Voltage to... VDD
                                //00=VDD, 01=n/a, 10=Pin, 11=FVR
    FVRCON = 0b11000010;        //ADFVR configured to 2.048V
    
    __delay_us(10);
    ADIE=Disable;               //disable ADC interrupts, Poll instead
    ADON=Disable;               //Switch off ADC. Started by readADC function
    GO_nDONE=Stop;              //don't start conversation yet
    return;
}

unsigned short ReadADC(){

    unsigned short adcData = 0; //combined value of ADC result registers
    
    GO_nDONE = Start;           //Start conversation
    while (GO_nDONE){           //While conversation is happening do nothing
    }
    
    //mask off unused hi byte bits and keep bottom 2
    adcData = (ADRESH & 0x03);  //Store high byte into short integer variable
    adcData<<=8;                //Shift high byte data into higher nibble
    adcData+=ADRESL;          //Store low byte into variable
    
    GO_nDONE = Stop;
    
    return adcData;
    
}

#endif	/* ADC_H */

