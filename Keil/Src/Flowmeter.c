#include "Flowmeter.h"

// ˮ�����Ƶ�������
uint flowmeterCount=0;

// ��ǰ����ҳ��
extern enum MenuPage page;

// ��ʼ�� �ⲿ�ж�1 ��ˮ������
void InitFlowmeter()
{
	EX0=1;  // �ⲿ�ж�0��
	IT0=1;  // �½��ش�����ʽ
    EA=1;   // ȫ���жϿ�
}

void Flowmeter_INT() interrupt 2
{
    // ����������ˮ����, ͳ����ˮ��
    if (page == UsePage)
        flowmeterCount++;
    //while(!FlowmeterPort); // �ͷż��
}
