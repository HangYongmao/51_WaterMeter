/*
*  智能水表设计
*  CPU:STC89C52RC
*  晶振:11.0592MHZ
*
*  刚开始不显示，或者液晶显示一个标题。
*  当按下按键之后，开始统计水流量。
*  再次按下，计算水流量和价格，显示总价和本次价格。
*  如果中途检测到总水流量超过某个值。则报警。
*/

#include "main.h"
#include "Timer.h"
#include "LCD1602.h"
#include "Flowmeter.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Key.h"

// 水流量计的脉冲数
extern uint flowmeterCount;

// 用于计算流速
uint clock=0;
uint waterL=0;

// 当前所在页面
enum MenuPage page=HomePage;
uint currentWater;
uint PriceSumH=0;   // 累计用水的价格 整数部分
uint PriceSumL=0;   // 累计用水的价格 小数部分
uint WaterSum=0;    // 累计用水量

// 延时 z ms
void delay_ms(uint z)
{
	uint x, y;
	for (x = z; x>0; x--)
        for (y = 114; y>0; y--);
}

unsigned int calcWaterPriceH(unsigned int flowmeterCount)
{
    // 3L以内 1元/L
    // 3L到5L 2元/L
    // 5L以上 3元/L
    unsigned long priceH=0; // 整数部分
    if (flowmeterCount/PULSE >= 5)
    {
        priceH = 3*1+2*2+(flowmeterCount/PULSE-5)*3;
    }
    else if (flowmeterCount/PULSE >= 3)
    {
        priceH = 3*1+(flowmeterCount/PULSE-3)*2;
    }
    else
        priceH = flowmeterCount/PULSE;
    return priceH;
}

unsigned int calcWaterPriceL(unsigned int flowmeterCount)
{
    // 3L以内 1元/L
    // 3L到5L 2元/L
    // 5L以上 3元/L
    unsigned long priceL=0; // 小数部分
    if (flowmeterCount/PULSE >= 5)
    {
        priceL = (flowmeterCount%PULSE)*100/PULSE*3;
    }
    else if (flowmeterCount/PULSE >= 3)
    {
        priceL = (flowmeterCount%PULSE)*2*100/PULSE;
    }
    else
        priceL = flowmeterCount%PULSE*100/PULSE;
    return priceL;
}

// 主函数
void main()
{
    P1=0;
    // 初始化液晶
    init_LCD();

    // 初始化 外部中断1 接水流量计
    InitFlowmeter();
    
    // 初始化定时器 测速
    InitTimer1();
    
    // LCD显示 第一行
    write_com(0x80+0x01);
    LCD_ShowStr("Welcome To Use");
    
    while(1)
    {
        switch(keyscan())
        {
        case NoKey:
            break;
        case keyIC:
        {
            // 显示下一个界面
            // 如果当前在主页面
            // 显示 正在用水
            if ((page == HomePage) || (page == SettlePage))
            {
                // 清空当前用水量
                flowmeterCount = 0;
                write_com(0x01);	// 显示清0，数据指针清0
                page = UsePage;
                clock = 0;
                TR1 = 1;		// 定时器1开始计时
            }
            // 如果 正在用水
            // 显示 结算界面
            else
            {
                write_com(0x01);	// 显示清0，数据指针清0
                page = SettlePage;
                
                // 累计用水的价格 整数部分
                PriceSumH += calcWaterPriceH(flowmeterCount)+calcWaterPriceL(flowmeterCount)/100;
                // 累计用水的价格 小数部分
                PriceSumL += calcWaterPriceL(flowmeterCount)%100;
                // 累计用水量
                WaterSum += flowmeterCount;
                
                // 当前用水量
                currentWater = flowmeterCount;
                flowmeterCount = 0;
            }
            break;
        }
        case keyClear:
        {
            // 清空所有数据
            flowmeterCount = 0;
            WaterSum = 0;
            PriceSumH = 0;
            PriceSumL = 0;
            write_com(0x01);	// 显示清0，数据指针清0
            write_com(0x80+0x01);
            LCD_ShowStr("Clear Complete");
            delay_ms(1000);
            
            page = HomePage;
            break;
        }
        }
        switch(page)
        {
        case HomePage:
            // LCD显示 第一行
            write_com(0x80+0x01);
            LCD_ShowStr("Welcome To Use");
            break;

        case UsePage:
            // 水量超过10L, 报警
            if (flowmeterCount > PULSE*10)
            {
                Beep = 0;
                delay_ms(500);
                Beep = 1;
            }
            // LCD显示 第一行
            write_com(0x80);
            write_date('W');
            write_date('a');
            write_date('t');
            write_date(':');
            write_com(0x80+0x04);
            LCD_ShowInt(flowmeterCount/PULSE);  // 整数部分
            write_date('.');
            write_date((flowmeterCount%PULSE*10/PULSE)+'0');   // 小数部分
            write_date((flowmeterCount%PULSE*100/PULSE)%10+'0');
            write_date('L');
            
            waterL = flowmeterCount/PULSE*100+flowmeterCount%PULSE*10/PULSE*10+flowmeterCount%PULSE*100/PULSE%10;
            
            // 显示流速
            write_com(0x80+0x0A);
            LCD_ShowInt(waterL/(clock/3));  // 整数部分
            write_date('.');
            write_date(waterL*10/(clock/3)%10+'0');
            write_date(waterL*100/(clock/3)%10+'0');

            // LCD显示 第二行
            write_com(0x80+0x40);
            write_date('P');
            write_date('r');
            write_date('i');
            write_date(':');
            write_com(0x80+0x40+0x04);
            LCD_ShowInt(calcWaterPriceH(flowmeterCount)+calcWaterPriceL(flowmeterCount)/100);
            write_date('.');
            write_date(calcWaterPriceL(flowmeterCount)%100/10+'0');
            write_date(calcWaterPriceL(flowmeterCount)%10+'0');
            write_date('$');
            
            // 流速的单位
            write_date(' ');
            write_date('L');
            write_date('/');
            write_date('M');
            write_date('i');
            write_date('n');
            break;

        case SettlePage:
            // LCD显示 第一行
            write_com(0x80);
            write_date('W');
            write_date(':');
            write_com(0x80+0x02);
            LCD_ShowInt(currentWater/PULSE);  // 整数部分
            write_date('.');
            write_date((currentWater%PULSE*10/PULSE)+'0');   // 小数部分
            write_date((currentWater%PULSE*100/PULSE)%10+'0');
            write_date('L');
        
            // LCD显示 第一行
            write_com(0x80+0x8);
            write_date('$');
            write_date(':');
            write_com(0x80+0x8+0x02);
            LCD_ShowInt(calcWaterPriceH(currentWater)+calcWaterPriceL(currentWater)/100);
            write_date('.');
            write_date(calcWaterPriceL(currentWater)%100/10+'0');
            write_date(calcWaterPriceL(currentWater)%10+'0');
            
            // LCD显示 第二行
            write_com(0x80+0x40);
            write_date('W');
            write_date(':');
            write_com(0x80+0x02+0x40);
            LCD_ShowInt(WaterSum/PULSE);  // 整数部分
            write_date('.');
            write_date((WaterSum%PULSE*10/PULSE)+'0');   // 小数部分
            write_date((WaterSum%PULSE*100/PULSE)%10+'0');
            write_date('L');
        
            // LCD显示 第二行
            write_com(0x80+0x08+0x40);
            write_date('$');
            write_date(':');
            write_com(0x80+0x08+0x40+0x02);
            LCD_ShowInt(PriceSumH);
            write_date('.');
            write_date(PriceSumL%100/10+'0');
            write_date(PriceSumL%10+'0');
            break;
        }
        if (TF1 == 1)
        {
            TF1 = 0;
            TL1 = 0x00;		//设置定时初值
            TH1 = 0x4C;		//设置定时初值
            clock++;
        }
    }
}
