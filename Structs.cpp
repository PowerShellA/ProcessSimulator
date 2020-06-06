#include<iostream>
#include<thread>
#include<chrono>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include "Structs.h"
#include <mutex>

using namespace std;

//������
mutex coutMutex1;
mutex coutMutex2;
mutex coutMutex3;
mutex coutMutex4;

//��ջ���캯��
Stack::Stack() {
	this->top = StackSize - 1;
}

//ѹջ
bool Stack::push(Task *n) {
	if (this->top >=0) {
		this->base[this->top] = n;
		this->top--;
		return true;
	}
	else {
		return false;
	}
}

//��ջ
Task* Stack::pop() {
	if (this->top<StackSize-1) {
		this->top++;
		return this->base[this->top];
	}
	else {
		return nullptr;
	}
}

//�ж��Ƿ�ջ��
bool Stack::isVoid() {
	if (this->top < StackSize-1 ) {
		return false;
	}
	else {
		return true;
	}
}

//���й��캯��
Queue::Queue() {
	this->head = this->tail = nullptr;
	this->size = 0;
}

//���
void Queue::Push(Task *n) {
	if (this->head==nullptr) {
		this->head = this->tail = n;
	}
	else{
		this->tail->next = n;
		n->pre = this->tail;
		this->tail = n;
	}
}

//�������ȼ���ߵ�һ��Task
Task* Queue::PopUrgent(){
	Task * q, * p;
	q = this->head;
	p = this->head;
	for (;q->next!=this->tail &&q->next != nullptr;) {
		if (p->priority < q->next->priority) {
			p = q->next;
		}
		q = q->next;
	}
	if (p == this->head) {
		return this->Pop();
	}
	else if (p == this->tail) {
		Task * t = this->tail;
		this->tail = this->tail->pre;
		return t;
	}
	else {
		return this->Pop(p);
	}
}

//�������ȼ���ߵ�һ��Task
Task* Queue::PopUrgent(Task * q) {
	Task * p,*t;
	if (q == nullptr) {
		return nullptr;
	}
	p=t = q;
	for (; p->next != this->tail&&q->next != nullptr;) {
		if (t->priority < p->next->priority) {
			t = p->next;
		}
		p= p->next;
	}
	if (t == q) {
		return nullptr;
	}
	else {
		return this->Pop(t);
	}
}

//������̽���
Task* Queue::PopShort(){
	Task * q, * p;
	q = this->head;
	p = this->head;
	for (; q->next != this->tail&&q->next != nullptr;) {
		if (p->request_t < q->next->request_t) {
			p = q->next;
		}
		q = q->next;
	}
	if (p==this->head) {
		return this->Pop();
	}
	else {
		return this->Pop(p);
	}
}

//�м����
Task* Queue::Pop(Task * q) {
	if (q==this->head) {
		return this->Pop();
	}
	else if (q==this->tail) {
		Task * t = this->tail;
		this->tail = this->tail->pre;
		return t;
	}
	else {
		q->next->pre = q->pre;
		q->pre->next = q->next;
	}
	return q;
}

//����
Task* Queue::Pop() {
	Task * q;
	for (; this->head==nullptr;);
	q = this->head;
	this->head = this->head->next;
	return q;
}

//�ж��Ƿ�ӿ�
bool Queue::isVoid() {
	if (this->head != nullptr) {
		return false;
	}
	else {
		return true;
	}
}

//���ض�β
Task * Queue::getTail(){
	return this->tail;
}

//��������캯��
Cpu::Cpu(){
	this->q = Queue();//Task����
	this->s = Stack();//Taskջ
	this->record = Queue();//Task��¼
	this->taskNum = TaskNum;
}

//�����ȷ����㷨
int Cpu::FCFS(){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "************************" << endl;
	cout<<"****�����ȷ���(FCFS)****"<<endl;
	cout << "************************" << endl;
	for (int i=0;i<TaskNum;i++) {
		for (; this->q.isVoid(););
		Task *t;
		t = this->q.Pop();
		t->response_t = GetTickCount();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
		cout << "ִ��-����-" << t->index << endl;
		Sleep(t->request_t);
		t->finish_t = GetTickCount();
		t->wait_t=t->response_t - t->submit_t;
		this->record.Push(t);

		coutMutex2.lock();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "���-����-" << t->index << endl;
		coutMutex2.unlock();

	}
	return 0;
}

//�̽��������㷨
int Cpu::SJF(){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "************************" << endl;
	cout << "****����ҵ����(SJF)****" << endl;
	cout << "************************" << endl;
	for (int i = 0; i < TaskNum; i++) {
		for (; this->q.isVoid(););
		Task *t;
		t = this->q.PopShort();
		t->response_t = GetTickCount();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
		cout << "ִ��-����-" << t->index << endl;
		Sleep(t->request_t);
		t->finish_t = GetTickCount();
		t->wait_t=t->response_t - t->submit_t;
		this->record.Push(t);

		coutMutex2.lock();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "���-����-" << t->index << endl;
		coutMutex2.unlock();
	}
	return 0;
}

//���ȼ������㷨(����ռʽ)
int Cpu::Priority_non_Preemptive(){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "**********************************" << endl;
	cout << "****���ȼ������㷨(����ռʽ)****" << endl;
	cout << "**********************************" << endl;
	for (int i = 0; i < TaskNum; i++) {
		for (; this->q.isVoid(););
		Task *t;
		t = this->q.PopUrgent();
		t->response_t = GetTickCount();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
		cout << "ִ��-����-" << t->index << endl;
		Sleep(t->request_t);
		t->finish_t = GetTickCount();
		t->wait_t = t->response_t - t->submit_t;
		this->record.Push(t);

		coutMutex2.lock();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "���-����-" << t->index << endl;
		coutMutex2.unlock();
	}
	return 0;
}

//���ȼ������㷨(��ռʽ)
int Cpu::Priority_Preemptive() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	cout << "**********************************" << endl;
	cout << "****���ȼ������㷨(��ռʽ)******" << endl;
	cout << "**********************************" << endl;
	for (; this->taskNum > 0;) {
		for (; this->q.isVoid(););
		Task *t;
		t = this->q.PopUrgent();
		t->response_t = GetTickCount();
		this->Priority_Preemptive_Execult(t);
	}
	return 0;
}

//��ռʽִ��
int Cpu::Priority_Preemptive_Execult(Task * t){
	Task * x;
	bool flag = false;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE);
	cout << "ִ��-����-" << t->index << endl;
	for (int j = 0; j < (t->request_t - t->run_t); j++) {
		Sleep(1);
		x = this->q.PopUrgent(t);//���Գ������ȼ����ߵ�Task
		if (x!=nullptr) {
			t->run_t += j;
			flag = true;
			this->Interrupt(t, x);//�����ж�
		}
	}
	if (!flag) {
		t->finish_t = GetTickCount();
		this->record.Push(t);
		this->taskNum--;

		coutMutex4.lock();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		cout << "���-����-"<< t->index  << endl;
		coutMutex4.unlock();
	}
	return 0;
}

//�жϴ���
int Cpu::Interrupt(Task * tOld, Task * tNew) {

	coutMutex3.lock();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | BACKGROUND_BLUE);
	cout << "�ж�-����-" << tOld->index << " --->" << tNew->index << endl;
	coutMutex3.unlock();

	tOld->interrupt_t = GetTickCount();
	this->s.push(tOld);//�ɽ���ѹջ
	tNew->response_t = GetTickCount();
	this->Priority_Preemptive_Execult(tNew);//�½���ִ��
	this->Restore();//�жϻָ�
	return 0;
}

//�жϻָ�
int Cpu::Restore() {
	Task * t = this->s.pop();//�ɽ��̵�ջ

	coutMutex2.lock();
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | BACKGROUND_GREEN);
	cout << "�ָ�-����-" << t->index << endl;
	coutMutex2.unlock();

	t->wait_t += (GetTickCount() - t->interrupt_t);
	this->Priority_Preemptive_Execult(t);//�ɽ���ִ��
	return 0;
}

//�㷨����
int Cpu::Evaluate() {
	double avgWait_t = 0, avgTotal_t = 0, avgWeightTotal_t = 0, avgResponse_t = 0;
	double maxWait_t = 0, maxTotal_t = 0, maxWeightTotal_t = 0, maxResponse_t = 0;
	cout << endl <<"���˳��:"<<endl;
	for (int i = 0; i < TaskNum; i++) {
		Task *t = this->record.Pop();
		t->total_t = t->finish_t - t->submit_t;
		t->response_t = t->response_t - t->submit_t;
		t->weight_total_t = t->total_t / t->request_t;
		avgTotal_t += t->total_t;
		avgWeightTotal_t += t->weight_total_t;
		avgWait_t += t->wait_t;
		avgResponse_t += t->response_t;
		if (t->wait_t > maxWait_t) {
			maxWait_t = t->wait_t;
		}
		if (t->total_t > maxTotal_t) {
			maxTotal_t = t->total_t;
		}
		if (t->weight_total_t > maxWeightTotal_t) {
			maxWeightTotal_t = t->weight_total_t;
		}
		if (t->response_t > maxResponse_t) {
			maxResponse_t = t->response_t;
		}
		cout<<t->index<<"-->";
	}
	avgTotal_t /= TaskNum;
	avgWeightTotal_t /= TaskNum;
	avgWait_t /= TaskNum;
	avgResponse_t /= TaskNum;
	cout << endl << "*************************************************" << endl;
	cout << "����Ľ������� = " << TaskNum << endl;
	cout << "*************************************************" << endl;
	cout << "���㷨 ƽ���ȴ�ʱ�� = " << avgWait_t << "ms"
		<< " ƽ����תʱ�� = " << avgTotal_t << "ms"
		<< " ƽ����Ȩ��תʱ�� = " << avgWeightTotal_t << "ms"
		<< " ƽ����Ӧʱ�� = " << avgResponse_t << "ms"
		<< endl;
	cout << "*************************************************" << endl;
	cout << "�������̵����ȴ�ʱ�� = " << maxWait_t << "ms"
		<< " �����תʱ�� = " << maxTotal_t << "ms"
		<< " ����Ȩ��תʱ�� = " << maxWeightTotal_t << "ms"
		<< " �����Ӧʱ�� = " << maxResponse_t << "ms"
		<< endl;
	cout << "*************************************************" << endl << endl << endl;
	return 0;
}

//��ȡ�ö���Ķ��г�Ա��ָ��
Queue * Cpu::getQuene(){
	return &this->q;
}

//���������캯��
Generator::Generator(Queue *q, int taskNum){
	this->existingTasks = 0;
	this->q = q;
	this->taskNum = taskNum;
	this->genThread = nullptr;
}

//�������߳�ִ�еĺ���
void Generator::Generate(){
	for (int i=0;i< this->taskNum;i++) {
		Task *t;
		t = (Task *)malloc(sizeof(Task));
		t->index = i;//TaskΨһʶ�����
		t->submit_t = GetTickCount();//�ύʱ��
		t->request_t = this->Rand(MinWait, MaxWait);//������ʱ
		t->run_t = 0;//������ʱ��
		t->start_t = 0;//��ʼʱ��
		t->response_t = 0;//��Ӧʱ��
		t->wait_t = 0;//�ȴ�ʱ��
		t->finish_t = 0;//���ʱ��
		t->total_t = 0;//��תʱ��
		t->weight_total_t = 0;//��Ȩ��תʱ��
		t->interrupt_t = 0;//�����ж�ʱ��ʱ��
		t->priority = this->Rand(MinPirorty, MaxPirorty);//���ȼ�
		t->pre = nullptr;//ǰһ��Task��ָ��
		t->next = nullptr;//��һ��Task��ָ��
		this->q->Push(t);//�µĽ������
		this->existingTasks ++;

		//coutMutex1.lock();
		//unique_lock<std::mutex> lock(coutMutex1);
		//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		//unique_lock<std::mutex> lock(coutMutex1);
		//cout<<"���ɽ���"<< i <<endl;
		//coutMutex1.unlock();
		this_thread::sleep_for(std::chrono::milliseconds(2));
		//��ʱ����
		if (this->existingTasks%GenerateGap ==0) {
			this_thread::sleep_for(std::chrono::milliseconds(GenerateSleep));
		}
	}
}

void Generator::Start(){
	if (this->genThread==nullptr) {
		//�����������߳�
		this->genThread = new thread(&Generator::Generate, this);
	}
}

//��ֹ����
void Generator::Stop(){
	this->genThread = nullptr;
}

//��ս��̼�����
void Generator::Clear(){
	this->Stop();
	this->existingTasks = 0;
	this->q = nullptr;
}

//���������������Χa-b
int Generator::Rand(int a, int b){
	srand((int) time(0) + rand());
	return (rand() % (b - a)) + a;
}
