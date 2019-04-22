/*
*  ����ˮ�����
*  CPU:STC89C52RC
*  ����:11.0592MHZ
*
*  �տ�ʼ����ʾ������Һ����ʾһ�����⡣
*  �����°���֮�󣬿�ʼͳ��ˮ������
*  �ٴΰ��£�����ˮ�����ͼ۸���ʾ�ܼۺͱ��μ۸�
*  �����;��⵽��ˮ��������ĳ��ֵ���򱨾���
*/

#include "main.h"
#include "Timer.h"
#include "LCD1602.h"
#include "Flowmeter.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Key.h"

// ˮ�����Ƶ�������
extern uint flowmeterCount;

// ���ڼ�������
uint clock=0;
uint waterL=0;

// ��ǰ����ҳ��
enum MenuPage page=HomePage;
uint currentWater;
uint PriceSumH=0;   // �ۼ���ˮ�ļ۸� ��������
uint PriceSumL=0;   // �ۼ���ˮ�ļ۸� С������
uint WaterSum=0;    // �ۼ���ˮ��

// ��ʱ z ms
void delay_ms(uint z)
{
	uint x, y;
	for (x = z; x>0; x--)
        for (y = 114; y>0; y--);
}

unsigned int calcWaterPriceH(unsigned int flowmeterCount)
{
    // 3L���� 1Ԫ/L
    // 3L��5L 2Ԫ/L
    // 5L���� 3Ԫ/L
    unsigned long priceH=0; // ��������
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
    // 3L���� 1Ԫ/L
    // 3L��5L 2Ԫ/L
    // 5L���� 3Ԫ/L
    unsigned long priceL=0; // С������
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

// ������
void main()
{
    P1=0;
    // ��ʼ��Һ��
    init_LCD();

    // ��ʼ�� �ⲿ�ж�1 ��ˮ������
    InitFlowmeter();
    
    // ��ʼ����ʱ�� ����
    InitTimer1();
    
    // LCD��ʾ ��һ��
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
            // ��ʾ��һ������
            // �����ǰ����ҳ��
            // ��ʾ ������ˮ
            if ((page == HomePage) || (page == SettlePage))
            {
                // ��յ�ǰ��ˮ��
                flowmeterCount = 0;
                write_com(0x01);	// ��ʾ��0������ָ����0
                page = UsePage;
                clock = 0;
                TR1 = 1;		// ��ʱ��1��ʼ��ʱ
            }
            // ��� ������ˮ
            // ��ʾ �������
            else
            {
                write_com(0x01);	// ��ʾ��0������ָ����0
                page = SettlePage;
                
                // �ۼ���ˮ�ļ۸� ��������
                PriceSumH += calcWaterPriceH(flowmeterCount)+calcWaterPriceL(flowmeterCount)/100;
                // �ۼ���ˮ�ļ۸� С������
                PriceSumL += calcWaterPriceL(flowmeterCount)%100;
                // �ۼ���ˮ��
                WaterSum += flowmeterCount;
                
                // ��ǰ��ˮ��
                currentWater = flowmeterCount;
                flowmeterCount = 0;
            }
            break;
        }
        case keyClear:
        {
            // �����������
            flowmeterCount = 0;
            WaterSum = 0;
            PriceSumH = 0;
            PriceSumL = 0;
            write_com(0x01);	// ��ʾ��0������ָ����0
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
            // LCD��ʾ ��һ��
            write_com(0x80+0x01);
            LCD_ShowStr("Welcome To Use");
            break;

        case UsePage:
            // ˮ������10L, ����
            if (flowmeterCount > PULSE*10)
            {
                Beep = 0;
                delay_ms(500);
                Beep = 1;
            }
            // LCD��ʾ ��һ��
            write_com(0x80);
            write_date('W');
            write_date('a');
            write_date('t');
            write_date(':');
            write_com(0x80+0x04);
            LCD_ShowInt(flowmeterCount/PULSE);  // ��������
            write_date('.');
            write_date((flowmeterCount%PULSE*10/PULSE)+'0');   // С������
            write_date((flowmeterCount%PULSE*100/PULSE)%10+'0');
            write_date('L');
            
            waterL = flowmeterCount/PULSE*100+flowmeterCount%PULSE*10/PULSE*10+flowmeterCount%PULSE*100/PULSE%10;
            
            // ��ʾ����
            write_com(0x80+0x0A);
            LCD_ShowInt(waterL/(clock/3));  // ��������
            write_date('.');
            write_date(waterL*10/(clock/3)%10+'0');
            write_date(waterL*100/(clock/3)%10+'0');

            // LCD��ʾ �ڶ���
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
            
            // ���ٵĵ�λ
            write_date(' ');
            write_date('L');
            write_date('/');
            write_date('M');
            write_date('i');
            write_date('n');
            break;

        case SettlePage:
            // LCD��ʾ ��һ��
            write_com(0x80);
            write_date('W');
            write_date(':');
            write_com(0x80+0x02);
            LCD_ShowInt(currentWater/PULSE);  // ��������
            write_date('.');
            write_date((currentWater%PULSE*10/PULSE)+'0');   // С������
            write_date((currentWater%PULSE*100/PULSE)%10+'0');
            write_date('L');
        
            // LCD��ʾ ��һ��
            write_com(0x80+0x8);
            write_date('$');
            write_date(':');
            write_com(0x80+0x8+0x02);
            LCD_ShowInt(calcWaterPriceH(currentWater)+calcWaterPriceL(currentWater)/100);
            write_date('.');
            write_date(calcWaterPriceL(currentWater)%100/10+'0');
            write_date(calcWaterPriceL(currentWater)%10+'0');
            
            // LCD��ʾ �ڶ���
            write_com(0x80+0x40);
            write_date('W');
            write_date(':');
            write_com(0x80+0x02+0x40);
            LCD_ShowInt(WaterSum/PULSE);  // ��������
            write_date('.');
            write_date((WaterSum%PULSE*10/PULSE)+'0');   // С������
            write_date((WaterSum%PULSE*100/PULSE)%10+'0');
            write_date('L');
        
            // LCD��ʾ �ڶ���
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
            TL1 = 0x00;		//���ö�ʱ��ֵ
            TH1 = 0x4C;		//���ö�ʱ��ֵ
            clock++;
        }
    }
}
