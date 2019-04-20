#include "Flowmeter.h"

// 水流量计的脉冲数
uint flowmeterCount=0;

// 当前所在页面
extern enum MenuPage page;

// 初始化 外部中断1 接水流量计
void InitFlowmeter()
{
	EX0=1;  // 外部中断0开
	IT0=1;  // 下降沿触发方式
    EA=1;   // 全局中断开
}

void Flowmeter_INT() interrupt 2
{
    // 处于正在用水界面, 统计用水量
    if (page == UsePage)
        flowmeterCount++;
    //while(!FlowmeterPort); // 释放检测
}
