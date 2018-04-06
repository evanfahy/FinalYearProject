/*
 * File:   USART.c
 * Author: Evan
 *
 * Created on 05 March 2018, 19:07
 */


#include <xc.h>
#include "USART.h"

#define _XTAL_FREQ          4000000


void putch(char c)
{
    while(!TRMT);
    TXREG=c;
}

unsigned char getch(void)
{
    while(!RCIF)
        continue;
    return RCREG;
}

void InitUSART(void)
{
    
    PIE1bits.TXIE = 0;
    RCREG = 0;
    RCSTAbits.CREN = 1; // enable continous receiving
    TXSTAbits.TXEN = 1;
    
    // at 4Mhz Baud rate set up at 10412 //HERE: 10417 
    TXSTAbits.SYNC = 0;       //Asynch mode
    TXSTAbits.BRGH = 1;       //high speed
    BAUDCONbits.BRG16 = 0;
    SPBRG = 23;
        
    TRISAbits.TRISA1 = 1;
    ANSELAbits.ANSA1 = 0;
    APFCONbits.RXDTSEL = 1; // RX/DT function is on RA0
    APFCONbits.TXCKSEL = 1; // RX/DT function is on RA1
        
    return;
}

void TxChar(char data){
    
    TXREG = data;
    __delay_us(10);
    while(!TXIF){  
    }
    return;
}