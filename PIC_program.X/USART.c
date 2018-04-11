/*
 * File:   USART.c
 * Author: Evan
 *
 * Created on 05 March 2018, 19:07
 */


#include <xc.h>
#include "USART.h"

void InitUSART(void)
{
    
    PIE1bits.TXIE = 0;
    RCREG = 0;
    
    RCSTAbits.RX9 = 0;  //8bit-reception
    RCSTAbits.CREN = 1; // enable continous receiving
    
    
    TXSTAbits.TX9 = 0;  //8bit-transmission   
    TXSTAbits.TXEN = 1; //Transmit enabled
    
    // at 4Mhz Baud rate set up at 10417 
    TXSTAbits.SYNC = 0;       //Asynch mode
    TXSTAbits.BRGH = 1;       //high speed
    BAUDCONbits.BRG16 = 0;    //8-bit Baud Rate Generator
    SPBRG = 23;
        
    TRISAbits.TRISA1 = 1;
    ANSELAbits.ANSA1 = 0;
    
    TRISAbits.TRISA0 = 0;
    ANSELAbits.ANSA1 = 0;
    
    APFCONbits.RXDTSEL = 1; // RX/DT function is on RA0
    APFCONbits.TXCKSEL = 1; // RX/DT function is on RA1
        
    return;
}

void putch(char c)
{
    while(!TRMT)    //Do nothing while Transmit shift reg is empty
        continue;
    TXREG = c;
}

void TxChar(char data){
    
    TXREG = data;
    __delay_us(5);
    while(!TXIF){  
    }
    return;
}

unsigned char getch(void)
{
    while(!RCIF)
        continue;
    return RCREG;
}


