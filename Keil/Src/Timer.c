#include "main.h"

void InitTimer1()
{
	TMOD &= 0x0F;   // ���ö�ʱ��ģʽ
	TL1 = 0x00;		// ���ö�ʱ��ֵ
	TH1 = 0x4C;		// ���ö�ʱ��ֵ
	TF1 = 0;		// ���TF1��־
}
