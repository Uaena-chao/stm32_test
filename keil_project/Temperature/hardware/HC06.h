#ifndef __HC06_H
#define __HC06_H

#include <stdio.h>

void MyUSART1_Init(void);
void USART1_SendByte(uint8_t Byte);
uint8_t USART1_GetRxFlag(void);
uint8_t USART1_GetRxData(void);
void USART1_SendArray(uint8_t array[], uint16_t len);
void USART1_SendString(uint8_t string[]);

#endif
