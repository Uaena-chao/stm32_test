#include "stm32f10x.h"                  // Device header
#include "DELAY.h"
#include "DS18B20.h"
#include "OLED.h"



#define DS18B20_RCC  RCC_APB2Periph_GPIOB
#define DS18B20_GPIO GPIO_Pin_1
#define DS18B20_PORT GPIOB


#define DS18B20_LOW    GPIO_ResetBits(DS18B20_PORT,DS18B20_GPIO)
#define DS18B20_HIGH   GPIO_SetBits(DS18B20_PORT,DS18B20_GPIO)


u8 DS18B20_Init(){
	
	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(DS18B20_RCC, ENABLE);
	
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = DS18B20_GPIO;
 	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
	
	DS18B20_HIGH;
	DS18B20_Reset();
	
	return DS18B20_Check();
}

void DS18B20_Output_Input(u8 cmd){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = DS18B20_GPIO;
	
	if(cmd){
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	}
	else{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	}
	
	GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}

void DS18B20_Reset(){
	DS18B20_Output_Input(1);
	DS18B20_LOW;
	Delay_us(750);
	DS18B20_HIGH;
	Delay_us(15);
}

//0存在1不在
u8 DS18B20_Check(){
	u8 retry=0;
	DS18B20_Output_Input(0);	//SET PG11 INPUT	 
    while (GPIO_ReadInputDataBit(DS18B20_PORT,DS18B20_GPIO)&&retry<200)
	{
		retry++;
		Delay_us(1);
	};	 
	if(retry>=200)
		return 1;
	else 
		retry=0;
    while (!GPIO_ReadInputDataBit(DS18B20_PORT,DS18B20_GPIO)&&retry<240)
	{
		retry++;
		Delay_us(1);
	};
	if(retry>=240)
		return 1;	    
	return 0;
}


void DS18B20_Write(u8 Data){
	u8 i,DataBit;
	
	DS18B20_Output_Input(1);
	for(i=0;i<8;i++){
		
		DataBit = Data & 0x01;
		Data >>=1;
		
		if(DataBit){
			DS18B20_LOW;
			Delay_us(2);
			DS18B20_HIGH;
			Delay_us(60);
		}
		else{
			DS18B20_LOW;
			Delay_us(60);
			DS18B20_HIGH;
			Delay_us(2);
		}
			
	}
}


u8 DS18B20_Read_Bit(){
	u8 Data;
	
	DS18B20_Output_Input(1);
	DS18B20_LOW;
	Delay_us(2);
	DS18B20_HIGH;
	DS18B20_Output_Input(0);
	Delay_us(12);
	if(GPIO_ReadInputDataBit(DS18B20_PORT,DS18B20_GPIO) == SET){
		Data = 1;
	}
	else
		Data = 0;
	Delay_us(50);
	return Data;
}

u8 DS18B20_Read(){
	u8 i,t;
	u8 Data=0;
	for(i=0;i<8;i++){
		t = DS18B20_Read_Bit();
		Data = (t<<7) | (Data>>1);
		
	}
	return Data;
}


//开始温度转换
void DS18B20_Start(void) 
{   						               
    DS18B20_Reset();	   
	DS18B20_Check();	 
    DS18B20_Write(0xcc);
    DS18B20_Write(0x44);
} 



int16_t BS1820_ReadTemp(){
	u8 temp;
    u8 TL,TH;
	int16_t tem;
    DS18B20_Start ();  			// ds1820 start convert
    DS18B20_Reset();
    DS18B20_Check();	 
    DS18B20_Write(0xcc);	// skip rom
    DS18B20_Write(0xbe);	// convert	    
    TL=DS18B20_Read(); 	    // LSB   
    TH=DS18B20_Read();    	// MSB  
	    	  
    if(TH>7){
        TH=~TH;
        TL=~TL; 
        temp=1;					//温度为负  
    }
	else{
		temp=0;				//温度为正	 
	}		
    tem = TH; 					//获得高八位
    tem <<= 8;    
    tem += TL;					//获得底八位
	tem += temp;
	
	if(!temp)
		return tem; 		//返回温度值
	else 
		return -tem;    
}








