#include "stm32f10x.h"                  // Device header
#include "USART.h"

char USART2_RxData[100];
uint8_t USART2_RxFlag=0;
uint8_t RxData;
uint8_t DoneFlag=1;

void MyUSART2_Init(){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&USART_InitStructure);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART2,ENABLE);
}



void USART2_SendByte(uint8_t Byte){
	USART_SendData(USART2,Byte);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
}

void USART2_SendArray(uint8_t array[], uint16_t len){
	uint16_t i;
	for(i=0;i<len;i++){
		USART2_SendByte(array[i]);
	}
}

void USART2_SendString(uint8_t string[]){
	uint16_t i;
	for(i=0;string[i] != '\0';i++){
		USART2_SendByte(string[i]);
	}
}


uint8_t USART2_GetRxFlag(){
	if(USART2_RxFlag == 1){
		USART2_RxFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t USART2_GetRxData(){
	return RxData;
}

void USART2_SetDoneFlag(uint8_t flag){
	DoneFlag = flag;
}


void USART2_IRQHandler(void){
	static uint8_t USART2_RxPlace=0;
	static uint8_t USART2_RxState=0;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET ){
		uint8_t RxData = USART_ReceiveData(USART2);
		if(USART2_RxState == 0){
			if(RxData == '@' && DoneFlag){
				USART2_RxPlace = 0;
				USART2_RxState = 1;
			}
		}
		else if(USART2_RxState == 1){
			if (RxData == '\r'){
				USART2_RxState = 2;
			}
			else{
				USART2_RxData[USART2_RxPlace] = RxData;
				USART2_RxPlace++;
			}
			
		}
		else if(USART2_RxState == 2){
			if (RxData == '\n'){
				USART2_RxState = 0;
				USART2_RxData[USART2_RxPlace] = '\0';
				DoneFlag=0;
				USART2_RxFlag  = 1;
			}
		}
		
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
}
