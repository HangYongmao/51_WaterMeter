#include "main.h"

// 按键检测
uchar keyscan()
{
	// 检测按键 IC卡
	if (keyICPort == 0)
	{
		delay_ms(10);   // 消抖动
		if (keyICPort == 0)
		{
			while (!keyICPort);	//等待按键释放
            return keyIC;
		}
	}
    
    // 检测按键 清空
    if (keyClearPort == 0)
	{
		delay_ms(10);   // 消抖动
		if (keyClearPort == 0)
		{
			while (!keyClearPort);	//等待按键释放
            return keyClear;
		}
	}
    return NoKey;
}
