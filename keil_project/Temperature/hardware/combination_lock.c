#include "stm32f10x.h"
#include "OLED.h"
#include "KeyBoard.h"
#include "Delay.h"
#include "COMBINATION_LOCK.h"

unsigned int ERR_Count=0;
extern char keynum;
extern unsigned int Password,TempPass,Ad_Password;
extern u8 MainFlag;
extern u8 ChangeMimaFlag;

void PassEnter(void)
{
	OLED_ShowNum(3,1,TempPass,4);
	KEY_Scan(&keynum);
	if(keynum>='0'&&keynum<='9')	//如果S1~S10按键按下，输入密码
	{
		TempPass*=10;				//密码左移一位
		TempPass+=(keynum-'0');		//获取一位密码
		keynum = 0;
		OLED_ShowNum(3,1,TempPass,4);	//更新显示
	}
	if(keynum=='*')	//如果S11按键按下，退位
	{
		TempPass/=10;
		keynum=0;
		OLED_ShowNum(3,1,TempPass,4);	//更新显示
	}
	if(keynum=='#')
	{
		keynum=0;
		if(TempPass==Password)	//如果密码等于正确密码
		{
			ERR_Count=0;
			OLED_ShowString(3,5,"PASS");	//显示OK
			TempPass=0;		//密码清零
			Delay_ms(500);
			OLED_ShowString(3,1,"    ");
			MainFlag=1;
		}
		else				//否则
		{
			OLED_ShowString(3,5,"ERR ");	//显示ERR
			ERR_Count++;
			OLED_ShowNum(4,16,ERR_Count,1);
			TempPass=0;		//密码清零
			OLED_ShowNum(3,1,TempPass,4);	//更新显示
			Delay_ms(500);
			OLED_ShowString(3,5,"    ");
			if(ERR_Count ==5)
			{
				ERR_Count=0;
				OLED_Clear();
				OLED_ShowString(1,1," ERROR TOO MUCH ");
				OLED_ShowString(2,1,"Ad_Password:");
				while(!(TempPass == Ad_Password&&keynum =='#') )
				{
					KEY_Scan(&keynum);
					OLED_ShowNum(2,13,TempPass,4);	//更新显示
					if(keynum>='0'&&keynum<='9')	//如果S1~S10按键按下，输入密码
					{
						TempPass*=10;				//密码左移一位
						TempPass+=(keynum-'0');		//获取一位密码
						keynum = 0;
					}
					if(keynum=='*')	//如果S11按键按下，退位
					{
						TempPass/=10;
						keynum=0;
					}
					if(keynum == '#'&& TempPass != Ad_Password)
					{
						OLED_ShowString(3,1,"PASSWORD_ERR");	//显示ERR
						Delay_ms(500);
						OLED_ShowString(3,1,"            ");
						keynum=0;
						TempPass=0;
					}
				
				}
				TempPass=0;
			}
		}
	}
	if(keynum=='A')
	{
		keynum=0;
	}
}

void PassSet(void)
{
	keynum=0;
	KEY_Scan(&keynum);
	if(keynum>='0'&&keynum<='9')	//如果S1~S10按键按下，输入密码
	{
		TempPass*=10;				//密码左移一位
		TempPass+=(keynum-'0');		//获取一位密码
		keynum = 0;
	}
	if(keynum=='*')	//如果S11按键按下，退位
	{
		TempPass/=10;
		keynum=0;
	}
	if(keynum=='#')
	{
		ChangeMimaFlag = 1;
	}
}
