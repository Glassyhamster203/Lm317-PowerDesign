#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"

uint8_t OLED_GRAM[144][8];

// 反显函数
void OLED_ColorTurn(uint8_t i)
{
	if (i == 0)
	{
		OLED_WR_Byte(0xA6, OLED_CMD); // 正常显示
	}
	if (i == 1)
	{
		OLED_WR_Byte(0xA7, OLED_CMD); // 反色显示
	}
}

// 屏幕旋转180度
void OLED_DisplayTurn(uint8_t i)
{
	if (i == 0)
	{
		OLED_WR_Byte(0xC8, OLED_CMD); // 正常显示
		OLED_WR_Byte(0xA1, OLED_CMD);
	}
	if (i == 1)
	{
		OLED_WR_Byte(0xC0, OLED_CMD); // 反转显示
		OLED_WR_Byte(0xA0, OLED_CMD);
	}
}

// 发送一个字节
// 向SSD1306写入一个字节。
// mode:数据/命令标志 0,表示命令;1,表示数据;

#ifdef OLED_I2C

void OLED_WR_Byte(uint8_t dat, uint8_t cmd)
{
	uint8_t Data[2];
	if (cmd)
	{
		Data[0] = 0x00;
		Data[1] = dat;
	}
	else
	{
		Data[0] = 0x40;
		Data[1] = dat;
	}

	HAL_I2C_Master_Transmit(OLED_I2C, 0x78, Data, 2, 0xfff);
}

#endif

// 开启OLED显示
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D, OLED_CMD); // 电荷泵使能
	OLED_WR_Byte(0x14, OLED_CMD); // 开启电荷泵
	OLED_WR_Byte(0xAF, OLED_CMD); // 点亮屏幕
}

// 关闭OLED显示
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D, OLED_CMD); // 电荷泵使能
	OLED_WR_Byte(0x10, OLED_CMD); // 关闭电荷泵
	OLED_WR_Byte(0xAF, OLED_CMD); // 关闭屏幕
}

// 更新显存到OLED
void OLED_Refresh(void)
{
	uint8_t i, n;
	for (i = 0; i < 8; i++)
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD); // 设置行起始地址
		OLED_WR_Byte(0x00, OLED_CMD);	  // 设置低列起始地址
		OLED_WR_Byte(0x10, OLED_CMD);	  // 设置高列起始地址
		for (n = 0; n < 128; n++)
			OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
	}
}
// 清屏函数
void OLED_Clear(void)
{
	uint8_t i, n;
	for (i = 0; i < 8; i++)
	{
		for (n = 0; n < 128; n++)
		{
			OLED_GRAM[n][i] = 0; // 清除所有数据
		}
	}
	OLED_Refresh(); // 更新显示
}

// 画点
// x:0~127
// y:0~63
void OLED_DrawPoint(uint8_t x, uint8_t y)
{
	uint8_t i, m, n;
	i = y / 8;
	m = y % 8;
	n = 1 << m;
	OLED_GRAM[x][i] |= n;
}

// 清除一个点
// x:0~127
// y:0~63
void OLED_ClearPoint(uint8_t x, uint8_t y)
{
	uint8_t i, m, n;
	i = y / 8;
	m = y % 8;
	n = 1 << m;
	OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
	OLED_GRAM[x][i] |= n;
	OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
}

void OLED_Clear_adjust_cal(void)
{
	uint8_t i, n;
	for (i = 6; i < 8; i++)
	{
		for (n = 48; n < 128; n++)
		{
			OLED_GRAM[n][i] = 0; // 清除所有数据
		}
	}
}

void Draw_Line_Buffer(int x1, int y1, int x2, int y2)
{
	unsigned short us;
	unsigned short usX_Current, usY_Current;

	int lError_X = 0, lError_Y = 0, lDelta_X, lDelta_Y, lDistance;
	int lIncrease_X, lIncrease_Y;

	lDelta_X = x2 - x1; // 计算坐标增量
	lDelta_Y = y2 - y1;

	usX_Current = x1;
	usY_Current = y1;

	if (lDelta_X > 0)
		lIncrease_X = 1; // 设置单步方向

	else if (lDelta_X == 0)
		lIncrease_X = 0; // 垂直线

	else
	{
		lIncrease_X = -1;
		lDelta_X = -lDelta_X;
	}

	if (lDelta_Y > 0)
		lIncrease_Y = 1;

	else if (lDelta_Y == 0)
		lIncrease_Y = 0; // 水平线

	else
	{
		lIncrease_Y = -1;
		lDelta_Y = -lDelta_Y;
	}

	if (lDelta_X > lDelta_Y)
		lDistance = lDelta_X; // 选取基本增量坐标轴

	else
		lDistance = lDelta_Y;

	for (us = 0; us <= lDistance + 1; us++) // 画线输出
	{
		OLED_DrawPoint(usX_Current, usY_Current); // 画点
		lError_X += lDelta_X;
		lError_Y += lDelta_Y;

		if (lError_X > lDistance)
		{
			lError_X -= lDistance;
			usX_Current += lIncrease_X;
		}

		if (lError_Y > lDistance)
		{
			lError_Y -= lDistance;
			usY_Current += lIncrease_Y;
		}
	}
}

// num_shift为了满足数据不同的对齐方式,数据左对齐那么num_shift=4,数据右对齐那么num_shift=0
void Draw_Wave_Buffer(uint16_t *wave_data, uint16_t num_shift)
{
	uint16_t i;
	OLED_Clear();
	for (i = 0; i < 127; i++)
	{
		Draw_Line_Buffer(i, (64 - (wave_data[i] >> num_shift) / 100.0) - 1, i + 1, (64 - (wave_data[i + 1] >> num_shift) / 100.0) - 1);
	}
}

// x,y:圆心坐标
// r:圆的半径
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r)
{
	int a, b, num;
	a = 0;
	b = r;
	while (2 * b * b >= r * r)
	{
		OLED_DrawPoint(x + a, y - b);
		OLED_DrawPoint(x - a, y - b);
		OLED_DrawPoint(x - a, y + b);
		OLED_DrawPoint(x + a, y + b);

		OLED_DrawPoint(x + b, y + a);
		OLED_DrawPoint(x + b, y - a);
		OLED_DrawPoint(x - b, y - a);
		OLED_DrawPoint(x - b, y + a);

		a++;
		num = (a * a + b * b) - r * r; // 计算画的点离圆心的距离
		if (num > 0)
		{
			b--;
			a--;
		}
	}
}

// 在指定位置显示一个字符,包括部分字符
// x:0~127
// y:0~63
// size:选择字体 12/16/24
// 取模方式 逐列式
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size1)
{
	uint8_t i, m, temp, size2, chr1;
	uint8_t y0 = y;
	size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2); // 得到字体一个字符对应点阵集所占的字节数
	chr1 = chr - ' ';										   // 计算偏移后的值
	for (i = 0; i < size2; i++)
	{
		if (size1 == 12)
		{
			temp = asc2_1206[chr1][i];
		} // 调用1206字体
		else if (size1 == 16)
		{
			temp = asc2_1608[chr1][i];
		} // 调用1608字体
		else if (size1 == 24)
		{
			temp = asc2_2412[chr1][i];
		} // 调用2412字体
		else
			return;
		for (m = 0; m < 8; m++) // 写入数据
		{
			if (temp & 0x80)
				OLED_DrawPoint(x, y);
			else
				OLED_ClearPoint(x, y);
			temp <<= 1;
			y++;
			if ((y - y0) == size1)
			{
				y = y0;
				x++;
				break;
			}
		}
	}
}

// 显示字符串
// x,y:起点坐标
// size1:字体大小
//*chr:字符串起始地址
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t size1)
{
	while ((*chr >= ' ') && (*chr <= '~')) // 判断是不是非法字符!
	{
		OLED_ShowChar(x, y, *chr, size1);
		x += size1 / 2;
		if (x > 128 - size1) // 换行
		{
			x = 0;
			y += 2;
		}
		chr++;
	}
}

// m^n
uint32_t OLED_Pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;
	while (n--)
	{
		result *= m;
	}
	return result;
}

////显示2个数字
////x,y :起点坐标
////len :数字的位数
////size:字体大小
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num,uint8_t  len, uint8_t size1)
{
	uint8_t t, temp;
	for (t = 0; t < len; t++)
	{
		temp = (num / OLED_Pow(10, len - t - 1)) % 10;
		if (temp == 0)
		{
			OLED_ShowChar(x + (size1 / 2) * t, y, '0', size1);
		}
		else
		{
			OLED_ShowChar(x + (size1 / 2) * t, y, temp + '0', size1);
		}
	}
}

// 显示汉字
// x,y:起点坐标
// num:汉字对应的序号
// 取模方式 列行式
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t num, uint8_t size1)
{
	uint8_t i, m, n = 0, temp, chr1;
	uint8_t x0 = x, y0 = y;
	uint8_t size3 = size1 / 8;
	while (size3--)
	{
		chr1 = num * size1 / 8 + n;
		n++;
		for (i = 0; i < size1; i++)
		{
			if (size1 == 16)
			{
				temp = Hzk1[chr1][i];
			} // 调用16*16字体
			//				else if(size1==24)
			//						{temp=Hzk2[chr1][i];}//调用24*24字体
			//				else if(size1==32)
			//						{temp=Hzk3[chr1][i];}//调用32*32字体
			//				else if(size1==64)
			//						{temp=Hzk4[chr1][i];}//调用64*64字体
			else
				return;

			for (m = 0; m < 8; m++)
			{
				if (temp & 0x01)
					OLED_DrawPoint(x, y);
				else
					OLED_ClearPoint(x, y);
				temp >>= 1;
				y++;
			}
			x++;
			if ((x - x0) == size1)
			{
				x = x0;
				y0 = y0 + 8;
			}
			y = y0;
		}
	}
}

// num 显示汉字的个数
// space 每一遍显示的间隔
void OLED_ScrollDisplay(uint8_t num, uint8_t space)
{
	uint8_t i, n, t = 0, m = 0, r;
	while (1)
	{
		if (m == 0)
		{
			OLED_ShowChinese(128, 24, t, 16); // 写入一个汉字保存在OLED_GRAM[][]数组中
			t++;
		}
		if (t == num)
		{
			for (r = 0; r < 16 * space; r++) // 显示间隔
			{
				for (i = 0; i < 144; i++)
				{
					for (n = 0; n < 8; n++)
					{
						OLED_GRAM[i - 1][n] = OLED_GRAM[i][n];
					}
				}
				OLED_Refresh();
			}
			t = 0;
		}
		m++;
		if (m == 16)
		{
			m = 0;
		}
		for (i = 0; i < 144; i++) // 实现左移
		{
			for (n = 0; n < 8; n++)
			{
				OLED_GRAM[i - 1][n] = OLED_GRAM[i][n];
			}
		}
		OLED_Refresh();
	}
}

// 配置写入数据的起始位置
void OLED_WR_BP(uint8_t x, uint8_t y)
{
	OLED_WR_Byte(0xb0 + y, OLED_CMD); // 设置行起始地址
	OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
	OLED_WR_Byte((x & 0x0f), OLED_CMD);
}

// x0,y0：起点坐标
// x1,y1：终点坐标
// BMP[]：要写入的图片数组
void OLED_ShowPicture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t BMP[])
{
	uint32_t j = 0;
	uint8_t x = 0, y = 0;
	if (y % 8 == 0)
		y = 0;
	else
		y += 1;
	for (y = y0; y < y1; y++)
	{
		OLED_WR_BP(x0, y);
		for (x = x0; x < x1; x++)
		{
			OLED_WR_Byte(BMP[j], OLED_DATA);
			j++;
		}
	}
}

// OLED的初始化
void OLED_Init(void)
{
	//
	OLED_WR_Byte(0xAE, OLED_CMD); //--turn off oled panel
	OLED_WR_Byte(0x00, OLED_CMD); //---set low column address
	OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
	OLED_WR_Byte(0x40, OLED_CMD); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81, OLED_CMD); //--set contrast control register
	OLED_WR_Byte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1, OLED_CMD); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8, OLED_CMD); // Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6, OLED_CMD); //--set normal display
	OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f, OLED_CMD); //--1/64 duty
	OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00, OLED_CMD); //-not offset
	OLED_WR_Byte(0xd5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80, OLED_CMD); //--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9, OLED_CMD); //--set pre-charge period
	OLED_WR_Byte(0xF1, OLED_CMD); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA, OLED_CMD); //--set com pins hardware configuration
	OLED_WR_Byte(0x12, OLED_CMD);
	OLED_WR_Byte(0xDB, OLED_CMD); //--set vcomh
	OLED_WR_Byte(0x40, OLED_CMD); // Set VCOM Deselect Level
	OLED_WR_Byte(0x20, OLED_CMD); //-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02, OLED_CMD); //
	OLED_WR_Byte(0x8D, OLED_CMD); //--set Charge Pump enable/disable
	OLED_WR_Byte(0x14, OLED_CMD); //--set(0x10) disable
	OLED_WR_Byte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7)
	OLED_WR_Byte(0xAF, OLED_CMD);
	OLED_Clear();
}
