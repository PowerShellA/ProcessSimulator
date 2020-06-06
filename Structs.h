#pragma once
#include<thread>
using namespace std;

const int TaskNum = 15;//���ɵĽ�������Ŀ
const int StackSize = TaskNum;//ջ��С
const int GenerateGap = 300;//���������߼��
const int GenerateSleep = 40;//����������ʱ��
const int MinWait = 1;//����������Сִ��ʱ��
const int MaxWait = 6;//�����������ִ��ʱ��
const int MinPirorty = 0;//����������С���ȼ�
const int MaxPirorty = 10;//��������������ȼ�

//���̽ṹ��
typedef struct Task {
	unsigned int index; //TaskΨһʶ�����
	unsigned long submit_t;//�ύʱ��
	int request_t;//������ʱ
	int run_t;//������ʱ��
	int start_t;//��ʼʱ��
	int response_t;//��Ӧʱ��
	int wait_t;//�ȴ�ʱ��
	unsigned long finish_t;//���ʱ��
	int total_t;//��תʱ��
	double weight_total_t;//��Ȩ��תʱ��
	unsigned long interrupt_t;//�ϴν����ж�ʱ��ʱ��
	int priority;//���ȼ�
	Task * pre;//ǰһ��Task��ָ��
	Task * next;//��һ��Task��ָ��
};

//����, ����ʵ��
class Queue {
	Task * head;//����ָ��
	Task * tail;//��βָ��
	int size;//���д�С
public:
	Queue();//���캯��
	void Push(Task * t);//���
	Task * Pop();//����
	Task * Pop(Task * q);//����
	Task * PopUrgent(Task * q);//���ȼ���߳���
	Task * PopUrgent();//���ȼ���߳���
	Task * PopShort();//��ҵ��̳���
	bool isVoid();//�ж��Ƿ�ӿ�
	Task * getTail();//��ȡ��βָ��
};

//��ջ
class Stack {
private:
	int top;//ջ��ָʾ��
public:
	Stack();//���캯��
	Task * base[StackSize];//��ջ������
	bool push(Task * t);//ѹջ
	Task * pop();//��ջ
	bool isVoid();//�ж��Ƿ�ջ��
};

//����������
class Generator {
private:
	int taskNum;//����Task����
	int existingTasks;//�����ɵĽ��̵�����
	Queue * q;//���������еĴ����Task���ж����ָ��
	thread * genThread;//�������̶߳���
public:
	Generator(Queue * q, int taskNum);//���캯��
	void Generate();//���ɽ���
	void Start();//����������
	void Stop();//��ֹ����
	void Clear();//��ս��̼�����
	static int Rand(int a, int b);//���������������Χa-b
};

//�����
class Cpu {
private:
	Queue q;//Task����
	Stack s;//Taskջ
	Queue record;//Task��¼
	int taskNum;//ʣ��Task����
public:
	Cpu();//���캯��
	int FCFS();//�����ȷ���
	int SJF();//����ҵ����
	int Priority_Preemptive();//���ȼ�����-��ռʽ
	int Priority_non_Preemptive();//���ȼ�����-����ռʽ
	int Interrupt(Task * tOld, Task * tNew);//CPU�ж�
	int Restore();//�жϻָ�
	int Evaluate();//���۵�ǰ�㷨
	int Priority_Preemptive_Execult(Task * t);//��ռʽִ��
	Queue * getQuene();//��ȡ�ö���Ķ��г�Ա��ָ��
};