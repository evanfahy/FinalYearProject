/*
 * File:   ADC.c
 * Author: Evan
 *
 * Created on 05 March 2018, 19:06
 */


#include <xc.h>
#include "ADC.h"

#define _XTAL_FREQ          4000000  //Declare Oscillator value (0.1Mhz) for use in __delay_ms, etc.

//Function Definitions
void InitialiseADC(void){

    ADCON0bits.CHS = 0b0111;    //selects Pin7(AN7)
    //ADCON0bits.CHS = 0b0110;    //selects Pin8(AN6)
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

    adcData = 0;
    
    GO_nDONE = Start;           //Start conversation
    while (GO_nDONE){           //While conversation is happening do nothing// should take 2us
    }
    
    //mask off unused hi byte bits and keep bottom 2
    adcData = (ADRESH & 0x03);  //Store high byte into short integer variable
    adcData<<=8;                //Shift high byte data into higher nibble
    adcData+=ADRESL;          //Store low byte into variable
    
    //ADRESH = 0x00;
    //ADRESL = 0x00;
    
    GO_nDONE = Stop;
    
    return adcData;
    
}
