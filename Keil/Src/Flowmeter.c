#include "Flowmeter.h"

// ˮ�����Ƶ�������
uint flowmeterCount=0;

// ��ʼ�� �ⲿ�ж�1 ��ˮ������
void InitFlowmeter()
{
	EX0=1;  // �ⲿ�ж�0��
	IT0=1;  // �½��ش�����ʽ
    EA=1;   // ȫ���жϿ�
}

void Flowmeter_INT() interrupt 2
{
    flowmeterCount++;
    //while(!FlowmeterPort); // �ͷż��
}
