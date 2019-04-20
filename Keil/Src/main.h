#ifndef __MAIN_H__
#define __MAIN_H__

#include <reg52.h>

#define uchar unsigned char
#define uint  unsigned int

// 1L水=PULSE个脉冲
#define PULSE 553 // 553

// 页面
enum MenuPage{
    HomePage,   // 0-主菜单
    UsePage,    // 1-正在用水
    SettlePage  // 2-结算界面
};

enum KeyValue{
    NoKey,      // 0-没有按键被按下
    keyIC,  // 1-按下 IC卡
    keyClear// 2-按下 清空
};

// LCD1602
sbit LCD_RS = P2 ^ 0;	// 液晶数据命令选择端
sbit LCD_RW = P2 ^ 1;   // 读/写选择端(H/L)
sbit LCD_EN = P2 ^ 2;   // 液晶使能端
sfr LCD_Port= 0x80;

// 水流量计
sbit FlowmeterPort = P3 ^ 2;

// 按键
sbit keyICPort = P3 ^ 5;
sbit keyClearPort = P3 ^ 6;

// 蜂鸣器
sbit Beep = P3 ^ 3;

// 延时 z ms
void delay_ms(uint z);

//value: 要转换的整数，string: 转换后的字符串,radix: 转换进制数，如2,8,10,16 进制
char* itoa(int value, char* string, int radix);

#endif
