#include "stm32f10x.h"
#include "DELAY.h"
#include "OLED.h"
#include "OLED_Font.h"
#include "OLED_Gram.h"

/*引脚配置*/

#define OLED_GPIOX            GPIOB
#define RCC_APB2Periph_GPIOX  RCC_APB2Periph_GPIOB
#define OLED_SCL              GPIO_Pin_5
#define OLED_SDA        	  GPIO_Pin_6


#define OLED_W_SCL(x)	      GPIO_WriteBit(OLED_GPIOX, OLED_SCL, (BitAction)(x))
#define OLED_W_SDA(x)		  GPIO_WriteBit(OLED_GPIOX, OLED_SDA, (BitAction)(x))


/*引脚初始化*/
void OLED_I2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOX, ENABLE);
	
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = OLED_SCL;
 	GPIO_Init(OLED_GPIOX, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = OLED_SDA;
 	GPIO_Init(OLED_GPIOX, &GPIO_InitStructure);
	
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
  * @brief  I2C开始
  * @param  无
  * @retval 无
  */
void OLED_I2C_Start(void)
{
	OLED_W_SDA(1);
	OLED_W_SCL(1);
	OLED_W_SDA(0);
	OLED_W_SCL(0);
}

/**
  * @brief  I2C停止
  * @param  无
  * @retval 无
  */
void OLED_I2C_Stop(void)
{
	OLED_W_SDA(0);
	OLED_W_SCL(1);
	OLED_W_SDA(1);
}

/**
  * @brief  I2C发送一个字节
  * @param  Byte 要发送的一个字节
  * @retval 无
  */
void OLED_I2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA(Byte & (0x80 >> i));
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}
	OLED_W_SCL(1);	//额外的一个时钟，不处理应答信号
	OLED_W_SCL(0);
}

/**
  * @brief  OLED写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void OLED_WriteCommand(uint8_t Command)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x00);		//写命令
	OLED_I2C_SendByte(Command); 
	OLED_I2C_Stop();
}

/**
  * @brief  OLED写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void OLED_WriteData(uint8_t Data)
{
	OLED_I2C_Start();
	OLED_I2C_SendByte(0x78);		//从机地址
	OLED_I2C_SendByte(0x40);		//写数据
	OLED_I2C_SendByte(Data);
	OLED_I2C_Stop();
}

/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand(0xB0 | Y);					//设置Y位置
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置低4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置高4位
}
 
/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
  */
void OLED_Clear(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
			OLED_GRAM[j][i] = 0x00;
		}
	}
	GramtoDot();
}

/**
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	gram_y = (Line - 1) * 2, gram_x = (Column - 1) * 8;
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
		OLED_GRAM[(gram_y + gram_x/128)][((gram_x++)%128)] = (OLED_F8x16[Char - ' '][i]);
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	gram_y = (Line - 1) * 2 + 1, gram_x = (Column - 1) * 8;
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
		OLED_GRAM[(gram_y + gram_x/128)][((gram_x++)%128)] = (OLED_F8x16[Char - ' '][i + 8]);
	}
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
	}
}

/**
  * @brief  OLED初始化
  * @param  无
  * @retval 无
  */
void OLED_Init(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_Init();			//端口初始化
	
	OLED_WriteCommand(0xAE);	//关闭显示
	
	OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//设置多路复用率
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//设置显示偏移
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//设置显示开始行
	
	OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//设置对比度控制
	OLED_WriteCommand(0xCF);

	OLED_WriteCommand(0xD9);	//设置预充电周期
	OLED_WriteCommand(0xF1);

	OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
	OLED_WriteCommand(0x30);

	OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭

	OLED_WriteCommand(0xA6);	//设置正常/倒转显示 A6normal A7 invert

	OLED_WriteCommand(0x8D);	//设置充电泵
	OLED_WriteCommand(0x14);

	OLED_WriteCommand(0xAF);	//开启显示
		
	OLED_Clear();				//OLED清屏
}



/**
* @brief  OLED显示汉字字模库中含有的汉字
  * @param  无
  * @retval 无
  */
void OLED_ShowChinese(uint8_t Line, uint8_t Column, int Num)
{      	
	uint8_t i;
	OLED_SetCursor((Line - 1) * 2, (Column - 1) * 16);		//设置光标位置在上半部分
	gram_y = (Line - 1) * 2, gram_x = (Column - 1) * 16;
	for (i = 0; i < 16; i++)
	{
		OLED_WriteData(OLED_Chinese[Num][i]);			//显示上半部分内容
		OLED_GRAM[(gram_y + gram_x/128)][((gram_x++)%128)] = (OLED_Chinese[Num][i]);
	}
	OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 16);	//设置光标位置在下半部分
	gram_y = (Line - 1) * 2 + 1, gram_x = (Column - 1) *16;
	for (i = 0; i < 16; i++)
	{
		OLED_WriteData(OLED_Chinese[Num][i + 16]);		//显示下半部分内容
		OLED_GRAM[(gram_y + gram_x/128)][((gram_x++)%128)] = (OLED_Chinese[Num][i + 16]);
	}
}

void OLED_ShowBMP(uint8_t x){
	uint8_t i,j;
	for(i=0;i<8;i++){
		for(j=0;j<128;j++){
			OLED_GRAM[i][j] = OLED_BMP[x][i][j];
		}
	}
	OLED_Refresh();
}


/*************************************************************************************************/
/**
* @brief  控制OLED显示关闭与否，不清除屏幕上内容
* @param  0为灭，非0为亮
  * @retval 无
  */
void OLED_Display_Turn(uint8_t turn){
	if(turn){
		OLED_WriteCommand(0XAF);
	}
	else{
		OLED_WriteCommand(0XAE);
	}
}

/**
* @brief  控制OLED显示模式，正常与反色
* @param  0为反色显示，1为正常显示
  * @retval 无
  */
void OLED_Display_Mode(uint8_t mode){
	if(mode){
		OLED_WriteCommand(0XA6); //正常显示
	}
	else{
		OLED_WriteCommand(0XA7); //反色显示
	}
}

/**
* @brief  控制OLED单独X轴与Y轴是否镜像显示
* @param  1为镜像，0为正常
  * @retval 无
  */
void OLED_Display_Dir(uint8_t x, uint8_t y){
	if(x)
		OLED_WriteCommand(0XA0);
	else
		OLED_WriteCommand(0XA1);
	if(y)
		OLED_WriteCommand(0XC0);
	else
		OLED_WriteCommand(0XC8);
}

/**
  * @brief  OLED全亮
  * @param  无
  * @retval 无
  */
void OLED_On(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0xFF);
		}
	}
}

void OLED_Refresh(){
	unsigned int i,j;
	for(i=0;i<8;i++){
		OLED_SetCursor(i,0);
		for(j=0;j<128;j++){
			OLED_WriteData(OLED_GRAM[i][j]);
		}
	}
}
/*显示一个点
y:行数1-8;   x:列数1-128
*/
void OLED_ShowPoint(uint8_t y,uint8_t x){
	if(y>8 || x>128)
		return;
	uint8_t col,line;
	col = (y-1)/8;
	line = x;
	OLED_SetCursor(col,line);
	OLED_GRAM[col][line] |= 0x01 << ((y-1)%8);
	OLED_Refresh();
}

/*熄灭一个点
y:行数1-8;   x:列数1-128
*/
void OLED_ClearPoint(uint8_t y,uint8_t x){
	uint8_t col,line;
	col = (y-1)/8;
	line = x;
	OLED_SetCursor(col,line);
	OLED_GRAM[col][line] &= ~(0x01 << ((y-1)%8));
}


void OLED_Mask_ShowBMP(uint8_t X,uint16_t time){
	FourPoint(0,0,X);
	Delay_ms(time);
	FourPoint(0,1,X);
	Delay_ms(time);
	FourPoint(1,0,X);
	Delay_ms(time);
	FourPoint(1,1,X);
	Delay_ms(time);
}

void FourPoint(uint8_t ystart,uint8_t xstart,uint8_t X){
	uint8_t i,j;
	for(i=ystart;i<64;i+=2){
		for(j=xstart;j<128;j+=2){
			if( OLED_BMP[X][i/8][j] & (0x01 << (i%8)) ){
				OLED_Dot_Matrix[i][j] = true;
			}
		}
	}
	DottoGram();
	OLED_Refresh();
}



/*************************************************************************************************/

void DottoGram(void){
	unsigned int i,j;
	for(i=0;i<64;i++){
		for(j=0;j<128;j++){
			if( OLED_Dot_Matrix[i][j] == true ){
				OLED_GRAM[i/8][j] |= (0x01 << (i%8) );
			}
			else{
				OLED_GRAM[i/8][j] &= (~(0x01 << (i%8) ));
			}
		}
	}
}

void GramtoDot(void){
	unsigned int i,j;
	for(i=0;i<64;i++){
		for(j=0;j<128;j++){
			if( (OLED_GRAM[i/8][j] & (0x01 << (i%8))) ){
				OLED_Dot_Matrix[i][j] = true;
			}
			else{
				OLED_Dot_Matrix[i][j] = false;
			}
		}
	}
}


/*************************************************************************************************/

void OLED_Scroll(){
	bool temp[128];
	unsigned int i,j;
	for(i=0;i<128;i++){
		temp[i] = OLED_Dot_Matrix[0][i];
	}
	for(i=0;i<63;i++){
		for(j=0;j<128;j++){
			OLED_Dot_Matrix[i][j] = OLED_Dot_Matrix[i+1][j];
		}
	}
	for(i=0;i<128;i++){
		OLED_Dot_Matrix[63][i] = temp[i];
	}
}

void OLED_Scroll_Horizontal(){
	bool temp[64];
	unsigned int i,j;
	for(i=0;i<64;i++){
		temp[i] = OLED_Dot_Matrix[i][0];
	}
	for(i=0;i<64;i++){
		for(j=0;j<127;j++){
			OLED_Dot_Matrix[i][j] = OLED_Dot_Matrix[i][j+1];
		}
	}
	for(i=0;i<64;i++){
		OLED_Dot_Matrix[i][127] = temp[i];
	}
}


/*************************************************************************************************/


void OLED_BarChart(int16_t Temp[]){
	GramtoDot();
	uint8_t i,j;
	for(i=0;i<128;i++){
		OLED_Dot_Matrix[16][i] = true;
		OLED_Dot_Matrix[60][i] = true;
	}
	
	for(i=0,j=7;i<10;i++,j+=12){
		uint8_t k;
		uint8_t  num = ((uint8_t)(Temp[i]+0.5) - 15) * 2;
		for(k=17; k<60;k++){
			OLED_Dot_Matrix[k][j]   = false;
			OLED_Dot_Matrix[k][j+1] = false;
			OLED_Dot_Matrix[k][j+2] = false;
			OLED_Dot_Matrix[k][j+3] = false;
			OLED_Dot_Matrix[k][j+4] = false;
			OLED_Dot_Matrix[k][j+5] = false;
		
		}
	}
	
	for(i=0,j=7;i<10;i++,j+=12){
		uint8_t k;
		uint8_t  num = ((uint8_t)(Temp[i]+0.5) - 15) * 2;
		for(k=60-num; k<60;k++){
			OLED_Dot_Matrix[k][j]   = true;
			OLED_Dot_Matrix[k][j+1] = true;
			OLED_Dot_Matrix[k][j+2] = true;
			OLED_Dot_Matrix[k][j+3] = true;
			OLED_Dot_Matrix[k][j+4] = true;
			OLED_Dot_Matrix[k][j+5] = true;
		
		}
	}
	
	DottoGram();
	OLED_Refresh();
}
















/***************************************************************************************************/
