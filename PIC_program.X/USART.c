/*
 * File:   USART.c
 * Author: Evan
 *
 * Created on 05 March 2018, 19:07
 */


#include <xc.h>
#include "IO_Controls.h"
#include <stdio.h>
#include <string.h>
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
    
    APFCON0bits.RXDTSEL = 1; // RX/DT function is on RA0
    APFCON0bits.TXCKSEL = 1; // RX/DT function is on RA1
    
    //ptr = &rxArray;
        
    return;
}

void UART_TX(unsigned char *data){
    
    while(!TRMT)           //Do nothing while Transmit shift reg is full/is not empty (is not set)
                            //TRMT flag in TXSTA it is set when the shift register is empty 
        continue;
    if(TXIF){                //If the Transmit interrupt flag is set
                             //The flag marks that the transmitter is ready to receive new information for transmission.
                             // It is set when data is transferred to TSR. It is cleared on writing to TXREG.
        TXREG = data;
    }

    return;
}

void UART_TX_String(unsigned char *string)
{
  int i;
  for(i=0;string[i]!='\0';i++)
    UART_TX(string[i]);
}




unsigned char UART_RX_Byte(void)
{

    return RCREG;
}

//
void UART_RX_String()
{
/*character is received until end of the string received */

    *(rxArray+ptr)=RCREG;
    ptr++;
}

void empty_RX_BUFF(){
    for(int i = 0; i < 20; i++)
    {
        
        rxArray[i] = '\0';
    }
    ptr = 0;
}
//used with printf
void putch(char c)
{
    while(!TRMT)    //Do nothing while Transmit shift reg is full/is not empty (is not set)
                    //TRMT flag in TXSTA it is set when the shift register is empty 
        continue;
    TXREG = c;
}



