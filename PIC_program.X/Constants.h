/* 
 * File:  Constants.h 
 * Author:  Evan Fahy
 * Comments:
 * Revision history: v1.0 - First draft
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef CONSTANTS_H
#define	CONSTANTS_H

//misc instructions/abbreviations
#define Pin6_OFF LATCbits.LATC4=Clear
#define Pin6_ON LATCbits.LATC4=Set

//Constant Definitions
#define Yes 1
#define No 0
#define longDelay 1
#define shortDelay 0
#define Set 1
#define Clear 0
#define Start 1
#define Stop 0
#define Enable 1
#define Disable 0

#define TurnOn 1 
#define TurnOff 0



#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */



#endif	/* CONSTANTS_H */

