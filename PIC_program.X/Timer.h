/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

/*
 * 4MHz Fosc/4 = 1MHz INTCLK
 * 1MHz INTCLK/2 = 500kHz = Prescaler clock cycles per second.
 *               = time for one cycle               
 *               = 1/Prescaler Clock Frequency 
 *               = 2us (timer1 increments every clock cycle)
 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TIMER_H
#define	TIMER_H

#include <xc.h> // include processor files - each processor file is guarded.

void Init_TMR1(void);
void TMR1Reset(void);
unsigned int Timer1(void);
unsigned int Timer1Read(void);

#define _XTAL_FREQ          4000000  //Declare Oscillator value (1Mhz) for use in __delay_ms, etc.
#define TMR1_cnt            65536    // value to be subtracted away from timer0 register (max of 65535 allowed)
#define ONE_CLK_DURATION    0.52438
#define Sys_tick_counter    125000


//Variables
unsigned long tmr1Val;  //for main
unsigned long aveTmr1Val, Tmr1Val_Ms;





#endif	/* XC_HEADER_TEMPLATE_H */

