#ifndef __USART_H
#define __USART_H

#include <stdio.h>

void MyUSART2_Init(void);
void USART2_SendByte(uint8_t Byte);
void USART2_SendArray(uint8_t array[], uint16_t len);
void USART2_SendString(uint8_t string[]);
uint8_t USART2_GetRxFlag(void);
uint8_t USART2_GetRxData(void);
void USART2_SetDoneFlag(uint8_t flag);


#endif
