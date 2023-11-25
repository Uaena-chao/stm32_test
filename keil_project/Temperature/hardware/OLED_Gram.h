#ifndef __OLED_GRAM_H
#define __OLED_GRAM_H

typedef enum {false = 0,true = 1} bool;


unsigned int gram_y=0,gram_x=0;
uint8_t gram_flag = 0;

uint8_t OLED_GRAM[8][128] = {0};

uint8_t OLED_GRAM_Ready[8][128] = {0};

bool OLED_Dot_Matrix[64][128] = {false};


#endif
