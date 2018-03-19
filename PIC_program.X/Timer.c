/*
 * File:   Timer.c
 * Author: Evan
 *
 * Created on 05 March 2018, 19:10
 */


#include <xc.h>
#include "Timer.h"
#include "Constants.h"
#include "Variables.h"
#include "USART.h"

unsigned int temp_time;   //16 bit number
unsigned int time;

void Init_TMR1(void)
{
    
    T1CON = 0b00000000;
    T1CONbits.TMR1CS = 0b00;    //Timer1 clock source is instruction clock (Fosc/4)
    T1CONbits.T1CKPS = 0b01;    //Prescale value 1:2
    
    T1GCON = 0b01000000;
    
    INTCON = 0b10000000;
    
    return;
}

void Init_TMR0(void)
{
    TMR0CS = 0;  //Internal instruction cycle (Fosc/4))
    PSA = 1;     //Prescaler enabled
    PS0 = 0;    //Prescaler 1:64
    PS1 = 0;    //Prescaler 1:64
    PS2 = 0;    //Prescaler 1:64
    
    TMR0IE = 1;
    TMR0IF = 0;
    TMR0 = 0;
    
    return;
}

 unsigned int Timer1(void)
{
        T1CONbits.TMR1ON = Disable;                 // Stop the timer
        
        temp_time = TMR1H;
        temp_time <<= 8;
        temp_time += TMR1L;
        
        time = temp_time;
        
        TMR1L = 0x00;
        TMR1H = 0x00;
        temp_time = 0;
        
        return time;
}
 