#include "stm32f10x.h"
#include "KeyBoard.h"
#include "Delay.h"
//	    

//********************************************************************************
u8 FLAG = 0;

void KEY_4x4_Init(void) //IO初始化
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = KEY_HANG;  //行  0123
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(KEY_PROT, &GPIO_InitStructure);
	GPIO_SetBits(KEY_PROT,KEY_HANG);
	
	//init GPIOA	  上拉输入
	GPIO_InitStructure.GPIO_Pin  = KEY1|KEY2|KEY3|KEY4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(KEY_PROT, &GPIO_InitStructure);
	GPIO_SetBits(KEY_PROT,KEY1|KEY2|KEY3|KEY4);
		 

}


void KEY_Scan(char *key) 
{	 
	
	GPIO_Write(KEY_PROT,0x00fe);//第一行
	if((KEY1_Input==RESET)||(KEY2_Input==RESET)||(KEY3_Input==RESET)||(KEY4_Input==RESET))   
	{
		Delay_ms(5);//去抖动 
		
		if(KEY1_Input==RESET)
		{
				FLAG = 1;
				*key = '1';
				while(!GPIO_ReadInputDataBit(KEY_PROT,KEY1));
		}
		else if(KEY2_Input==RESET)
		{
	 	 
	     	    FLAG = 1;
				*key = '2';
				while(!GPIO_ReadInputDataBit(KEY_PROT,KEY2));
		}
		else if(KEY3_Input==RESET)
		{
	 
				FLAG = 1;
				*key = '3';
				while(!GPIO_ReadInputDataBit(KEY_PROT,KEY3));
		}
		else if(KEY4_Input==RESET)
		{
	 
				FLAG = 1;
				*key = 'A';
				while(!GPIO_ReadInputDataBit(KEY_PROT,KEY4));
		}else 
					{
						FLAG = 0;
						GPIO_Write(KEY_PROT,0x00ff);
					}
		}
	GPIO_Write(KEY_PROT,0x00fd);//第二行
	if((KEY1_Input==RESET)||(KEY2_Input==RESET)||(KEY3_Input==RESET)||(KEY4_Input==RESET))
	{
		Delay_ms(5);//去抖动 
		
		if(KEY1_Input==RESET)
		{
				FLAG = 1;
				*key = '4';
				while(!GPIO_ReadInputDataBit(KEY_PROT,KEY1));
		}
		else if(KEY2_Input==RESET)
		{
	 	 
	     	FLAG = 1;
				*key = '5';
				while(!GPIO_ReadInputDataBit(KEY_PROT,KEY2));
		}
		else if(KEY3_Input==RESET)
		{
	 
	    	FLAG = 1;
				*key = '6';
				while(!GPIO_ReadInputDataBit(KEY_PROT,KEY3));
		}
		else if(KEY4_Input==RESET)
		{
	 
	    	FLAG = 1;
				*key = 'B';
				while(!GPIO_ReadInputDataBit(KEY_PROT,KEY4));
		}else 
					{
						FLAG = 0;
						GPIO_Write(KEY_PROT,0x00ff);
					}
		}
	GPIO_Write(KEY_PROT,0x00fb);//第三行
	if((KEY1_Input==RESET)||(KEY2_Input==RESET)||(KEY3_Input==RESET)||(KEY4_Input==RESET))
	{
		Delay_ms(5);//去抖动 
		
		if(KEY1_Input==RESET)
		{
				FLAG = 1;
				*key = '7';
				while(!GPIO_ReadInputDataBit(KEY_PROT,KEY1));
		}
		else if(KEY2_Input==RESET)
		{
	 	 
	     	FLAG = 1;
				*key = '8';
				while(!GPIO_ReadInputDataBit(KEY_PROT,KEY2));
		}
		else if(KEY3_Input==RESET)
		{
	 
	    	FLAG = 1;
				*key = '9';
				while(!GPIO_ReadInputDataBit(KEY_PROT,KEY3));
		}
		else if(KEY4_Input==RESET)
		{
	 
	    	FLAG = 1;
				*key = 'C';
				while(!GPIO_ReadInputDataBit(KEY_PROT,KEY4));
		}else 
					{
						FLAG = 0;
						GPIO_Write(KEY_PROT,0x00ff);
					}
		}
	GPIO_Write(KEY_PROT,0x00f7);//第四行
	if((KEY1_Input==RESET)||(KEY2_Input==RESET)||(KEY3_Input==RESET)||(KEY4_Input==RESET))
	{
		Delay_ms(5);//去抖动 
		
		if(KEY1_Input==RESET)
		{
				FLAG = 1;
				*key = '*';
				while(!GPIO_ReadInputDataBit(KEY_PROT,KEY1));
		}
		else if(KEY2_Input==RESET)
		{
	 	 
				FLAG = 1;
				*key = '0';
				while(!GPIO_ReadInputDataBit(KEY_PROT,KEY2));
		}
		else if(KEY3_Input==RESET)
		{
	 
				FLAG = 1;
				*key = '#';
				while(!GPIO_ReadInputDataBit(KEY_PROT,KEY3));
		}
		else if(KEY4_Input==RESET)
		{
	 
				FLAG = 1;
				*key = 'D';
				while(!GPIO_ReadInputDataBit(KEY_PROT,KEY4));
		}else 
					{
						FLAG = 0;
						GPIO_Write(KEY_PROT,0x00ff);
					}
		}
	if(FLAG == 0)
		*key = 0;
	
}




