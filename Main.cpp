//���̵��ȳ��� 
//�����ȷ�������㷨
//����ҵ���ȵ����㷨
//���ȼ������㷨������ռʽ ��ռʽ��
//�㷨����
//��� ���Ƚ�� �㷨����ָ��

#include<iostream>
#include "Structs.h"

using namespace std;

//������
int main() {
	//4�����������4������������һһ��Ӧ����������
	Cpu * cpu1 = new Cpu();
	Cpu * cpu2 = new Cpu();
	Cpu * cpu3 = new Cpu();
	Cpu * cpu4 = new Cpu();
	Generator * gen1 = new Generator(cpu1->getQuene(), TaskNum);
	Generator * gen2 = new Generator(cpu2->getQuene(), TaskNum);
	Generator * gen3 = new Generator(cpu3->getQuene(), TaskNum);
	Generator * gen4 = new Generator(cpu4->getQuene(), TaskNum);

	//ִ����һ���㷨֮����ִ����һ��
	gen1->Start();//����������
	cpu1->FCFS();//�����ȷ���
	cpu1->Evaluate();//���۵�ǰ�㷨
	delete gen1;
	delete cpu1;

	gen2->Start();//����������
	cpu2->SJF();//����ҵ����
	cpu2->Evaluate();//���۵�ǰ�㷨
	delete gen2;
	delete cpu2;

	gen3->Start();//����������
	cpu3->Priority_non_Preemptive();//���ȼ�����-����ռʽ
	cpu3->Evaluate();//���۵�ǰ�㷨
	delete gen3;
	delete cpu3;

	gen4->Start();//����������
	cpu4->Priority_Preemptive();//���ȼ�����-��ռʽ
	cpu4->Evaluate();//���۵�ǰ�㷨
	delete gen4;
	delete cpu4;

	//��ͣ����
	cin.get();
	return 0;
}