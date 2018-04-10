#ifndef USART_H
#define USART_H

#define _XTAL_FREQ          4000000

void putch(char );
void TxChar(char );
void InitUSART(void);

unsigned char mydata;

#endif /* USART_H  */
