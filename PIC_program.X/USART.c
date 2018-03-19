/*
 * File:   USART.c
 * Author: Evan
 *
 * Created on 05 March 2018, 19:07
 */


#include <xc.h>
#include "USART.h"

void putch(char c)
{
    while(!TRMT);
    TXREG=c;
}
void InitUSART(void)
{
    
    TXIE=0;
    TRISA0 =0;
    TXCKSEL =1;
    TXSTAbits.TXEN=1;
    RCSTAbits.SPEN =1;
    
    // at 1Mhz Baud rate set up at 10412 //HERE: 10417 
    TXSTAbits.SYNC=0;       //Asynch mode
    TXSTAbits.BRGH=1;       //high speed
    BAUDCONbits.BRG16=0;
    SPBRG = 23;
    
    return;
}

void TxChar(char data){
    
    TXREG = data;
    __delay_us(10);
    while(!TXIF){  
    }
    return;
}