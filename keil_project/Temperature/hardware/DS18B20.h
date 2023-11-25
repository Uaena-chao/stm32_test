#ifndef __DS18B20_H
#define __DS18B20_H



u8 DS18B20_Init(void);
void DS18B20_Output_Input(u8 cmd);
void DS18B20_Reset(void);
u8 DS18B20_Check(void);
void DS18B20_Write(u8 Data);
u8 DS18B20_Read(void);

int16_t BS1820_ReadTemp(void);




#endif
