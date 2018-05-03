#ifndef ADC_H
#define	ADC_H

#include <xc.h> // include processor files - each processor file is guarded.
#include "Constants.h"

//Function Declarations
void InitialiseADC(void);
unsigned short ReadADC(void);

//Variables
unsigned short adcData = 0; //combined value of ADC result registers
unsigned short adcSample;  //16bit
unsigned short previousADCsample;  //16bit


#endif	/* ADC_H */

