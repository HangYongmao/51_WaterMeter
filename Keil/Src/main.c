/*
*  智能水表设计
*  CPU:STC89C52TC
*  晶振:11.0592MHZ
*
*  刚开始不显示，或者液晶显示一个标题。
*  当按下按键之后，开始统计水流量。
*  再次按下，计算水流量和价格，显示总价和本次价格。
*  如果中途检测到总水流量超过某个值。则报警。
*/

#include "main.h"
#include "LCD1602.h"
#include "Flowmeter.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// 水流量计的脉冲数
extern uint flowmeterCount;

// 延时 z ms
void delay_ms(uint z)
{
	uint x, y;
	for (x = z; x>0; x--)
        for (y = 110; y>0; y--);
}

// 主函数
void main()
{
    // 初始化液晶
    init_LCD();
    
    // 初始化 外部中断1 接水流量计
    InitFlowmeter();
    // LCD显示 第一行
    write_com(0x80);
    write_date('A');
    
    while(1)
    {
        write_com(0x80);
        LCD_ShowInt(flowmeterCount);
    }
}
