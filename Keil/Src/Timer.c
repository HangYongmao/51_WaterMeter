#include "main.h"

void InitTimer1()
{
	TMOD &= 0x0F;   // 设置定时器模式
	TL1 = 0x00;		// 设置定时初值
	TH1 = 0x4C;		// 设置定时初值
	TF1 = 0;		// 清除TF1标志
}
