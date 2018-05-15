/*
 * File:   newmain.c
 * Author: Evan Fahy
 *
 * Created on 25 October 2017, 09:46
 */

#define _XTAL_FREQ          4000000  //Declare Oscillator value (0.4Mhz) for use in __delay_ms, etc.

//Standard libs
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "newmain.h"

int main() 
{
    
    initPIC();
   
    for(int i = 0; i < 6; i++){
    
        ToggleGLED();
        __delay_ms(250);
    }
    
   pi3Connect();
    __delay_ms(10);
   verifyACSignal();
    
    
    return (0);
}

void initPIC(void)
{
    
    // Setup Oscillator for 4MHz
    OSCCON = 0b01101000;    // SPLLEN IRCF2 IRCF2 IRCF1 IRCF0 unused SCS1 SCS0
                            // D7:       SPLLEN=0 => 4xPLL disabled
                            // D6-D3:    IRCF3-IRCF0 selects frequency
                            // eg    1010 = 500KHz (MF-Power up default)
                            //      1011 = 1MHz HF
                            //      1101 = 4MHz HF
                            // D2:       unused
                            // D1-D0     SCS1-SCS0 = Sys Clock Select
                            //              (00= determined by configuration Word 1 FOSC<1:0>)
    
    // Wait for the HF oscillator to be ready
    while(HFIOFR==0){       //waits for high frequency oscillator to begin
    }
    
    // Ensure interrupt disabled while starting up
    INTCON = 0b00000000;            
    INTCONbits.GIE = Disable;       // Disable Global Interrupt
    
    InitialisePorts();              // routine to setup ports
    Init_TMR1();
    InitialiseADC();
    InitUSART();

    INTCONbits.GIE = Enable;        //Enable Global Interrupt
    
    return;
}


void pi3Connect(void)
{
    PIE1bits.RCIE = Enable;                                 //enable USART receive interrupt
    INTCONbits.PEIE = Enable;                               //enables all active peripheral interrupts
    
    RCSTAbits.SPEN = Enable;                                //enable USART
    RCSTAbits.CREN = Disable; RCSTAbits.CREN = Enable;      // resets continuous receiver
    
    unsigned char handshake = '0';
   
    //Waits for a message from the Pi3
    while(handshake != '1'){
    
        handshake = rxArray[0];
        Fail_LED(Set);
    }
    rxFlag = 0;
            
    //PIE1bits.RCIE = Disable;                                 //enable USART receive interrupt
    empty_RX_BUFF();
    Fail_LED(Clear);
    
    //Sends message back to alert Pi3 is connected
    __delay_ms(10);
    UART_TX(handshake);
    
    return;
}

void verifyACSignal(void)
{   
    unsigned char handshake2 = '0';
    ADC_Test = True;
        
    PIE1bits.RCIE = Enable;                                 //enable USART receive interrupt
    
    //Waits for a message from the Pi3
    while(handshake2 != '2'){
    
        handshake2 = rxArray[0];
    }
    rxFlag = 0;
    empty_RX_BUFF();

    //Sends message back to alert Pi3 is connected
    __delay_ms(10);
    UART_TX(handshake2);
    PIE1bits.RCIE = Disable;                                 //enable USART receive interrupt
    ADON=Enable;       //ADC enable
    
    while(!ADON){} 
    aveTmr1Val = 0;
     
     //Begin ADC Test
     // 300 is arbitrary
    while(tmr1Samples < 300 ){
         
        adcSample = ReadADC();
        
        if((adcSample > 512) && (tmr1Start == False)) {
                    
            if(tmr1StartFlag == 0) {
            
                T1CONbits.TMR1ON = Enable;                  //Timer on
                tmr1StartFlag = Set;
            }
            else {
                
                tmr1Val = Timer1StopRead();                         //Function call to calculate time
                aveTmr1Val = aveTmr1Val + tmr1Val;
                
                tmr1Samples = tmr1Samples + 1;
                tmr1StartFlag = Clear;
            }
            
            //Timer has been started
            tmr1Start = True;
        }
        else if((adcSample < 128) && (tmr1Start == True)) {
            
            //Timer will stop on when ADC_sample > 512
            tmr1Start = False;
        }
        else{}
    }
     
    ADON=Disable;
    tmr1Samples = 0;
    tmr1Val = 0;
    aveTmr1Val = fabs((double)(aveTmr1Val/300));
    
    //Tmr1Val_Ms = fabs((double)(aveTmr1Val*2)/1000); // to display in ms
    
    if((aveTmr1Val >= 4750) && (aveTmr1Val <= 5250)){
    
        txByte = '1';
        //swap mains to OPTOCOUPLER circuit
        ADC_OPTO_Relay(Set);

    }
    else{
        
        txByte = '0';
    }
    
    PIE1bits.RCIE = Enable;                                 //enable USART receive interrupt     

    
    //Send to Pi
     UART_TX(txByte);
     __delay_ms(1800);
     printf("%d\n", aveTmr1Val);
     //printf("Half cycle: %d ms\n", Tmr1Val_Ms); 
     empty_RX_BUFF();
     
    if((aveTmr1Val >= 4750) && (aveTmr1Val <= 5250)){
        
        //ADC flaged over
        ADC_Test = False; 
    
        for(int i = 0; i < 6; i++){
    
            ToggleGLED();
            __delay_ms(100);
        }
        //Switch AC to main test
        ADC_OPTO_Relay(Set);
        //Calls next function
        setupTestParameters();  
    }
    else{}
     //}

    return;
}

void setupTestParameters(void)
{
    //PIE1bits.RCIE = Enable;                                   //enable USART receive interrupt
    INTCONbits.PEIE = Enable;                                   //enables all active peripheral interrupts
    
    //RCSTAbits.SPEN = Enable;                                  //enable USART
    RCSTAbits.CREN = Disable; RCSTAbits.CREN = Enable;          // resets continuous receiver
    
    unsigned char handshake3 = '0';
    
    PassLED(1);
    
    while(handshake3 != '3'){
    
        handshake3 = rxArray[0];
    }
    rxFlag = 0;
    PassLED(0);
   
    __delay_ms(10);
    UART_TX(rxArray[0]);
    empty_RX_BUFF();
    
    //wait for PoW value
    while(rxArray[0] == '\0'){}
    rxFlag = 0;
    
    __delay_ms(10);
    UART_TX_String(rxArray);

    PoW = atoi(rxArray);
    //printf("%c", rxArray);
    empty_RX_BUFF();
    
    //wait for number of cycles
    while(rxArray[0] == '\0'){}
    rxFlag = 0;
    
    __delay_ms(10);
    UART_TX_String(rxArray);
    cycles = atoi(rxArray);
    //printf("%c", rxArray);
    empty_RX_BUFF();

        
    PoW = ((PoW*100)/360);    //PoW is now a percentage of 360 degrees
    PoW = ((10000/100)*PoW);   //PoW is now the percentage of full 20ms cycle (10000 clock cycles)
    
    //begin test
    beginHighCurrentTest();
   
    return;
}


void beginHighCurrentTest(void)
{
    mainTest = True;
    PIE1bits.RCIE = Disable;               //Disables RX 
        
    //When POS_INT or NEG_INT occurs start timer
    //Constantly check when timer == PoW
    //When timer == PoW drive Thyristor
    //Finish test
    //Examine scope

    
    //Polarity of Transformer is flipped
    //SCR will only let negative cycle flow
    if(PoW > 5000)
    {
    
        SCR_Relay(Set);
        NegCycle = True;
        
    }
    else if (PoW < 4999){
        
        SCR_Relay(Clear);
        PosCycle = True;

    }
    
    //Repeats cycle pulse for i number of times
    for(int i = 0; i < cycles; i++){
    
        //Test is about to begin
        for(int j = 0; j < 6; j++){
            
            ToggleGLED();
            __delay_ms(250);
         }
        
        //Arbitrary delay to mimic trigger
        __delay_ms(3000);
        Fail_LED(1);
        __delay_ms(2000);
        Fail_LED(0);
        
        PassLED(1);        
        //Enable interrupt to turn on timer at Pos/Neg zero crossing
        if(PoW > 5000){
    
            INTCONbits.IOCIE = Enable;
        
        }
        else if (PoW < 4999){
        
            INTCONbits.INTE = Enable;
        }
        else{}
        
        //While Timer1 has not reached the time of PoW
         while(SCR_On < PoW){
             
             SCR_On = Timer1Read();
         }       
        SCR(Set);   //Drive SCR
        TMR1Reset();
        SCR_On = 0;
        PassLED(0);
        SCR(Clear);
    }
    
    NegCycle = False;
    PosCycle = False;
    SCR(Clear);
    SCR_Relay(Clear);
    ADC_OPTO_Relay(Clear);
    
    return;
}