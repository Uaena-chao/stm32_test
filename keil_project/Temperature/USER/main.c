#include "stm32f10x.h"    // Device header
#include "Delay.h"
#include "OLED.h"
#include "DS18B20.h"
#include "HC06.h"
#include "TIMER.h"
#include "USART.h"
#include "Buzzer.h"
#include "MENU.h"

#include <string.h>

uint8_t Temperature_down=20;
uint8_t Temperature_high=30;

uint8_t USART_Print_Flag=0;

extern uint8_t Buzzer_On_OFF;

extern int16_t Temperature;

extern uint8_t USART1_RxData;
extern char USART2_RxData[100];
uint8_t MENU_Mode=0;

uint8_t Buzzer_flag_first = 0;

int16_t Temperature=25;

int16_t Temperature_arr[10] = {0};

int main()
{
	OLED_Init();
	
	Buzzer_Init();
	Timer2_Init();
	MyUSART1_Init();
	MyUSART2_Init();
	DS18B20_Init();
	
	OLED_Mask_ShowBMP(0,400);
	Delay_s(1);
	
	USART2_SendString("Start:");
	OLED_Clear();
	
	while(DS18B20_Init()){
		OLED_ShowString(2,3,"DS18B20  ERR");
	}

	OLED_ShowString(2,3,"DS18B20  SUC");
	Delay_s(1);
	OLED_ShowString(2,3,"            ");
	while(1)
	{	
		if(USART1_GetRxFlag() == 1){
			OLED_Clear();
			if(USART1_RxData == 0x01){
				MENU_Mode=0;
				OLED_Clear();
			}
			else if(USART1_RxData == 0x02){
				MENU_Mode=1;
				OLED_Clear();
			}
			else if(USART1_RxData == 0x03){
				Buzzer_flag_first = 1 ;
				Buzzer_On_OFF=1;
			}
			else if(USART1_RxData == 0x04){
				Temperature_down--;
			}
			else if(USART1_RxData == 0x05){
				Temperature_down++;
			}
			else if(USART1_RxData == 0x06){
				Temperature_high--;
			}
			else if(USART1_RxData == 0x07){
				Temperature_high++;
			}
			else if(USART1_RxData == 0XFF){
				Buzzer_flag_first = 0;
				Buzzer_On_OFF=0;
			}
		}
		
		
		if(USART2_GetRxFlag() == 1){
			if( USART2_RxData[0]=='h' && USART2_RxData[1]=='i' && USART2_RxData[2]=='g' && USART2_RxData[3]=='h'){
				Temperature_high = (USART2_RxData[4]-'0')*10 + (USART2_RxData[5]-'0');
				USART2_SendString("Edit Success!\n");
			}
			else if( USART2_RxData[0]=='d' && USART2_RxData[1]=='o' && USART2_RxData[2]=='w' && USART2_RxData[3]=='n' ){
				Temperature_down = (USART2_RxData[4]-'0')*10 + (USART2_RxData[5]-'0');
				USART2_SendString("Edit Success!\n");
			}
			else if( USART2_RxData[0]=='m' && USART2_RxData[1]=='e' && USART2_RxData[2]=='n' && USART2_RxData[3]=='u' ){
				MENU_Mode++;
				MENU_Mode %=2;
				OLED_Clear();
			}
			else{
				USART_Print_Flag = 2;
				printf("ERROR_COMMAND");
			}
			
			
			USART2_SetDoneFlag(1);
		}
		
		
		if(Buzzer_flag_first == 0){
			if( ((Temperature*0.0625 > Temperature_high) || (Temperature*0.0625 < Temperature_down)) ){
				USART_Print_Flag = 1;
				printf("!!!温度异常,请注意!!!");
				USART_Print_Flag = 2;
				printf("!!!温度异常,请注意!!!");
				Buzzer_On_OFF=1;
			}
			else{
				Buzzer_On_OFF=0;
			}
		}
		
		Temperature = BS1820_ReadTemp();
		if(MENU_Mode == 0){
			MENU_1();
		}
		else if(MENU_Mode == 1){
			MENU_2();
		}
		
		
		
		
	}
}


int fputc(int ch,FILE *f){
	if(USART_Print_Flag == 1)
		USART1_SendByte(ch);
	if(USART_Print_Flag == 2)
		USART2_SendByte(ch);
	return ch;
}

/*将数组前移一位，最后一位不变*/
void Arr_Forward1bit(int16_t arr[],uint8_t len){
	uint8_t i;
	for(i=0;i<len-1;i++){
		arr[i] = arr[i+1];
	}
}


void TIM2_IRQHandler(void){
	static uint8_t temp_arr_place = 0;
	static uint8_t num = 0,flag = 0;
	
	num++;
	if(num == 2){
		if(flag == 1){
			Arr_Forward1bit(Temperature_arr,10);
		}
		Temperature_arr[temp_arr_place++] = Temperature*0.0625;
		if( temp_arr_place == 10){
			flag = 1; 
			temp_arr_place = 9;
		}
		num = 0;
	}
	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET ){
		
		USART_Print_Flag = 1;
		printf("实时温度:%.2f℃\n",Temperature*0.0625);
		
		USART_Print_Flag = 2;
		printf("%.4f\n",Temperature*0.0625);
	
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}



