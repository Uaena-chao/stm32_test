#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "DS18B20.h"
#include "HC06.h"
#include "TIMER.h"
#include "USART.h"
#include "Buzzer.h"

extern uint8_t Temperature_down;
extern uint8_t Temperature_high;

extern int16_t Temperature;

extern int16_t Temperature_arr[10];


void MENU_1(){
	
	OLED_ShowChinese(1,1,3);
	OLED_ShowChinese(1,2,4);
	OLED_ShowChinese(1,3,5);
	OLED_ShowChinese(1,4,6);
	OLED_ShowChar(1,9,':');
	
	if(Temperature>0)
		OLED_ShowString(1,10,"+");
	else
		OLED_ShowString(1,10,"-");
	OLED_ShowNum(1,11,Temperature/16,2);
	OLED_ShowString(1,13,".");
	OLED_ShowNum(1,14,((Temperature%16)/1.6+ 0.5),1);
	OLED_ShowChar(1,15,'~'+1);
	OLED_ShowChar(1,16,'C');
	
	OLED_ShowChinese(3,1,7);
	OLED_ShowChinese(3,2,9);
	OLED_ShowChinese(3,3,5);
	OLED_ShowChinese(3,4,6);
	OLED_ShowChar(3,9,':');
	OLED_ShowNum(3,10,Temperature_high,2);
	OLED_ShowChar(3,12,'~'+1);
	OLED_ShowChar(3,13,'C');
		
	OLED_ShowChinese(4,1,8);
	OLED_ShowChinese(4,2,9);
	OLED_ShowChinese(4,3,5);
	OLED_ShowChinese(4,4,6);
	OLED_ShowChar(4,9,':');
	OLED_ShowNum(4,10,Temperature_down,2);
	OLED_ShowChar(4,12,'~'+1);
	OLED_ShowChar(4,13,'C');
}


void MENU_2(){
	OLED_ShowChinese(1,1,10);
	OLED_ShowChinese(1,2,11);
	OLED_ShowChinese(1,3,5);
	OLED_ShowChinese(1,4,6);
	OLED_ShowChar(1,9,':');
	
	if(Temperature>0)
		OLED_ShowString(1,10,"+");
	else
		OLED_ShowString(1,10,"-");
	OLED_ShowNum(1,11,Temperature/16,2);
	OLED_ShowString(1,13,".");
	OLED_ShowNum(1,14,((Temperature%16)/1.6+ 0.5),1);
	OLED_ShowChar(1,15,'~'+1);
	OLED_ShowChar(1,16,'C');
	
	OLED_BarChart(Temperature_arr);

}

void MENU_3(){
	
	OLED_ShowString(2,7,"3.NULL");

}


void MENU_4(){
	
	OLED_ShowString(2,7,"4.NULL");

}














