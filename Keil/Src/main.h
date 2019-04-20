#ifndef __MAIN_H__
#define __MAIN_H__

#include <reg52.h>

#define uchar unsigned char
#define uint  unsigned int

// 1Lˮ=PULSE������
#define PULSE 553 // 553

// ҳ��
enum MenuPage{
    HomePage,   // 0-���˵�
    UsePage,    // 1-������ˮ
    SettlePage  // 2-�������
};

enum KeyValue{
    NoKey,      // 0-û�а���������
    keyIC,  // 1-���� IC��
    keyClear// 2-���� ���
};

// LCD1602
sbit LCD_RS = P2 ^ 0;	// Һ����������ѡ���
sbit LCD_RW = P2 ^ 1;   // ��/дѡ���(H/L)
sbit LCD_EN = P2 ^ 2;   // Һ��ʹ�ܶ�
sfr LCD_Port= 0x80;

// ˮ������
sbit FlowmeterPort = P3 ^ 2;

// ����
sbit keyICPort = P3 ^ 5;
sbit keyClearPort = P3 ^ 6;

// ������
sbit Beep = P3 ^ 3;

// ��ʱ z ms
void delay_ms(uint z);

//value: Ҫת����������string: ת������ַ���,radix: ת������������2,8,10,16 ����
char* itoa(int value, char* string, int radix);

#endif
