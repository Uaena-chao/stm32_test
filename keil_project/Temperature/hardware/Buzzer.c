#include "stm32f10x.h"                  // Device header
#include "Delay.h"   


extern uint8_t USART1_RxData;
extern int16_t Temperature; 

uint8_t Buzzer_On_OFF = 0;    // 1:开蜂鸣器 0:关蜂鸣器

void Timer3_Init(void){
	
	TIM_TimeBaseInitTypeDef TIM3_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_InternalClockConfig(TIM3);
	
	TIM3_TimeBaseInitStructure.TIM_ClockDivision     = TIM_CKD_DIV1 ;
	TIM3_TimeBaseInitStructure.TIM_CounterMode       = TIM_CounterMode_Up ;
	TIM3_TimeBaseInitStructure.TIM_Period            = 50 -1;    // ARR(0~65525)
	TIM3_TimeBaseInitStructure.TIM_Prescaler         = 720  -1;     // PSC(0~65525)
	TIM3_TimeBaseInitStructure.TIM_RepetitionCounter = 0 ;   //重复计数寄存器（高级计数器特有）
	
	TIM_TimeBaseInit(TIM3,&TIM3_TimeBaseInitStructure);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3,ENABLE);
	
}


void Buzzer_Init(){
	
	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	Timer3_Init();
	
}



void TIM3_IRQHandler(void){
	static uint8_t loop=0;
	
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET ){
		
		loop += 1;
		loop =(loop)%2;
		
		if(Buzzer_On_OFF && loop){
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
		}
		else{
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		}
		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
	
}














