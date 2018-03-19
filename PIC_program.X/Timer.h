/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

/*
 1MHz Fosc/4 = 250KHz INTCLK
 1MHz INTCLK/2 = 125kHz = Prescaler clock cycles per second.
               = time for one cycle 
 *              = 1/Prescaler Clock Frequency = 8us
 * 
 * Sys_tick_counter = 1s/8us = 125000 decimal value
 * when reached can be used to display a second has been counted
 * for one interrupt (125000/65536) = 1.907 counts
 *                                 = 1/1.907
 * clock will overflow every 524.38ms
 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TIMER_H
#define	TIMER_H

#include <xc.h> // include processor files - each processor file is guarded.

void Init_TMR0(void);
void Init_TMR1(void);
unsigned int Timer1(void);
//void interrupt INTERRUPT_InterruptManager (void);

#define _XTAL_FREQ          4000000  //Declare Oscillator value (1Mhz) for use in __delay_ms, etc.
#define TMR1_cnt            65536    // value to be subtracted away from timer0 register (max of 65535 allowed)
#define ONE_CLK_DURATION    0.52438
#define Sys_tick_counter    125000


//Variables
unsigned int time_16bit;  //for main



#endif	/* XC_HEADER_TEMPLATE_H */

