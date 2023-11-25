#include "stm32f10x.h"                  // Device header
#include "HC06.h"
#include "OLED.h"

uint8_t USART1_RxData,RxFlag=0;
//uint8_t RxPacket[4];
//uint8_t TxPacket[4];

void MyUSART1_Init(){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
	
}

void HC06_Init(){
	
}

void USART1_SendByte(uint8_t Byte){
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

void USART1_SendArray(uint8_t array[], uint16_t len){
	uint16_t i;
	for(i=0;i<len;i++){
		USART1_SendByte(array[i]);
	}
}

void USART1_SendString(uint8_t string[]){
	uint16_t i;
	for(i=0;string[i] != '\0';i++){
		USART1_SendByte(string[i]);
	}
}


uint8_t USART1_GetRxFlag(){
	if(RxFlag == 1){
		RxFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t USART1_GetRxData(){
	return USART1_RxData;
}

void USART1_IRQHandler(void){
//	static uint8_t RxState = 0;
//	static uint8_t rxplace = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET ){
		RxFlag = 1;
		USART1_RxData = USART_ReceiveData(USART1);
		
//		if(RxState == 0){
//			if(RxData == 0xFF){
//				RxState = 1;
//				rxplace = 0;
//			}
//		}
//		else if(RxState == 1){
//			RxPacket[rxplace++] = RxData;
//			if(rxplace >= 4 ){
//				RxState = 2;
//			}
//		}
//		else if(RxState == 2){
//			if ( RxData == 0xFE){
//				RxState = 0;
//				RxFlag = 1;
//			}
//		}
		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}
