/*
*  ����ˮ�����
*  CPU:STC89C52TC
*  ����:11.0592MHZ
*
*  �տ�ʼ����ʾ������Һ����ʾһ�����⡣
*  �����°���֮�󣬿�ʼͳ��ˮ������
*  �ٴΰ��£�����ˮ�����ͼ۸���ʾ�ܼۺͱ��μ۸�
*  �����;��⵽��ˮ��������ĳ��ֵ���򱨾���
*/

#include "main.h"
#include "LCD1602.h"
#include "Flowmeter.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ˮ�����Ƶ�������
extern uint flowmeterCount;

// ��ʱ z ms
void delay_ms(uint z)
{
	uint x, y;
	for (x = z; x>0; x--)
        for (y = 110; y>0; y--);
}

// ������
void main()
{
    // ��ʼ��Һ��
    init_LCD();
    
    // ��ʼ�� �ⲿ�ж�1 ��ˮ������
    InitFlowmeter();
    // LCD��ʾ ��һ��
    write_com(0x80);
    write_date('A');
    
    while(1)
    {
        write_com(0x80);
        LCD_ShowInt(flowmeterCount);
    }
}
