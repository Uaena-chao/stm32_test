#include "stm32f10x.h"                  // Device header

extern uint16_t num;

void Timer2_Init(void){
	
	TIM_TimeBaseInitTypeDef TIM2_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM2_TimeBaseInitStructure.TIM_ClockDivision     = TIM_CKD_DIV1 ;
	TIM2_TimeBaseInitStructure.TIM_CounterMode       = TIM_CounterMode_Up ;
	TIM2_TimeBaseInitStructure.TIM_Period            = 9777 -1;    // ARR(0~65525)
	TIM2_TimeBaseInitStructure.TIM_Prescaler         = 7200  -1;     // PSC(0~65525)
	TIM2_TimeBaseInitStructure.TIM_RepetitionCounter = 0 ;   //重复计数寄存器（高级计数器特有）
	
	TIM_TimeBaseInit(TIM2,&TIM2_TimeBaseInitStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);
	
}



/*中断函数模板
void TIM2_IRQHandler(void){
	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET ){
		

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
	
}
*/
