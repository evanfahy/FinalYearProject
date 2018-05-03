/* 
 * File:    DeviceConfig.h 
 * Author:  Evan Fahy
 * Comments:
 * Revision history:    v1.0 - First draft
 * 
 * Source lines that should be handled by the preprocessor, 
 * such as #define and #include are referred to as preprocessor directives. 
 * Another C construct, the #pragma directive, 
 * is used to instruct the compiler to use pragmatic or implementation-dependent features.
 * 
 * #pragma is for compiler directives that are machine-specific or operating-system-specific, 
 * i.e. it tells the compiler to do something, set some option, take some action, override some default, 
 * etc. that may or may not apply to all machines and operating systems.
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef DEVICECONFIG_H
#define	DEVICECONFIG_H

#include <xc.h> // include processor files - each processor file is guarded.

//Configuration Settings    (CONFIG1 & CONFIG2))
//Clock related
#pragma config FOSC = INTOSC    //Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config CLKOUTEN = OFF   //Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       //Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config PLLEN = OFF      //PLL Enable (4x PLL enabled)
#pragma config FCMEN = OFF      //Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

//Power Up/Reset Settings
#pragma config PWRTE = ON       //Power-up timer Enable (PWRT enabled)
#pragma config MCLRE = OFF      //MCLR Pin Function Select INTERNAL RESET (MCLR/VPP pin function is MCLR)
#pragma config BOREN = ON       //DEBUG OFF: Brown-out Reset Enabled
#pragma config BORV = LO        //Brown-out Reset Voltage Selection (Vbor HI = 2.5V, LO1 = 0.9V)
#pragma config STVREN = ON      //Stack Overflow/Underflow Reset Enable (Stack Overflow or UnderFlow will cause a reset)
#pragma config WDTE = OFF        //DEBUG OFF: Watchdog Timer Enable (WDT Enabled)

//Programming Settings
#pragma config WRT = OFF        //Flash Memory Self-Write Protection (Write protection off)
#pragma config CP = OFF         //DEBUG OFF: Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        //Data Memory Code Protection (Data memory code protection is disabled)
#pragma config LVP = OFF        //Low-Voltage Programming Enable (Low-Voltage programming enabled)

#endif	/* DEVICECONFIG_H */

