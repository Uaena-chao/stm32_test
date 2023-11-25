#ifndef __OLED_H
#define __OLED_H

/*****************/

void OLED_WriteCommand(uint8_t Command);
void OLED_WriteData(uint8_t Data);
void OLED_SetCursor(uint8_t Y, uint8_t X);

/*****************/

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

void OLED_ShowChinese(uint8_t Line, uint8_t Column, int Num);
void OLED_ShowBMP(uint8_t x);

/**************************/

void OLED_Display_Turn(uint8_t turn);
void OLED_Display_Mode(uint8_t mode);
void OLED_Display_Dir(uint8_t x, uint8_t y);
void OLED_On(void);
void OLED_Refresh(void);
void OLED_ShowPoint(uint8_t y,uint8_t x);
void OLED_ClearPoint(uint8_t y,uint8_t x);
void OLED_Mask_ShowBMP(uint8_t X,uint16_t time);
void FourPoint(uint8_t ystart,uint8_t xstart,uint8_t X);

void DottoGram(void);
void GramtoDot(void);
void OLED_Scroll(void);
void OLED_Scroll_Horizontal(void);

/**************************/

void OLED_BarChart(int16_t Temp[]);

#endif
