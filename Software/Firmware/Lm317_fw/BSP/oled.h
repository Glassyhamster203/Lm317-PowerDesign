#ifndef __OLED_H
#define __OLED_H
#include "stdlib.h"
#include "main.h"
#include "i2c.h"
//-----------------OLED端口定义----------------

#define OLED_I2C &hi2c1

#define OLED_CMD 0  // 写命令
#define OLED_DATA 1 // 写数据

#define u8 unsigned char
#define u32 unsigned int

struct type_OLED_Show
{
    int oled_flat_1;
    int oled_flat_2;
    int oled_flat_3;
};

void OLED_Init(void);                                           // OLED初始化
void OLED_Clear(void);                                          // OLED清屏
void OLED_Refresh(void);                                        // OLED刷新
void OLED_DrawCircle(u8 x, u8 y, u8 r);                         // OLED画圆
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size1);               // OLED显示字符
void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 size1);            // OLED显示字符串
void OLED_ShowChinese(u8 x, u8 y, u8 num, u8 size1);            // OLED显示汉字
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size1);       // OLED显示数字
void Draw_Line_Buffer(int x1, int y1, int x2, int y2);          // OLED画线
void OLED_ShowPicture(u8 x0, u8 y0, u8 x1, u8 y1, u8 BMP[]);    // OLED显示图片
void Draw_Wave_Buffer(uint16_t *wave_data, uint16_t num_shift); // OLED画波形，显示ADC采样波形

void OLED_DrawPoint(u8 x, u8 y);  // OLED画点
void OLED_ClearPoint(u8 x, u8 y); // OLED清点
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_WaitAck(void);
void Send_Byte(u8 dat);
void OLED_WR_Byte(u8 dat, u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_WR_BP(u8 x, u8 y);

void OLED_Clear_adjust_cal(void);
#endif
