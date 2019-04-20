#include "main.h"

// �������
uchar keyscan()
{
	// ��ⰴ�� IC��
	if (keyICPort == 0)
	{
		delay_ms(10);   // ������
		if (keyICPort == 0)
		{
			while (!keyICPort);	//�ȴ������ͷ�
            return keyIC;
		}
	}
    
    // ��ⰴ�� ���
    if (keyClearPort == 0)
	{
		delay_ms(10);   // ������
		if (keyClearPort == 0)
		{
			while (!keyClearPort);	//�ȴ������ͷ�
            return keyClear;
		}
	}
    return NoKey;
}
