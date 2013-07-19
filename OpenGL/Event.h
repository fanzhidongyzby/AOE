#pragma once
#include"stdafx.h"
#include"Graphic.h"
class Event:Graphic
{
public:
//������Ϣ
	EVENT_TYPE type;//��ʾ�¼�������
	list <Activity*> preList;//��¼�¼���ǰ����б�
	list <Activity*> nextList;//��¼�¼��ĺ�̻�б�
	Event(EVENT_TYPE type,float x,float y);
	~Event(void);
	bool isConnected(Event*other);//�ж��Ƿ��ָ���ڵ��й���
//AOV:����������Ϣ
	int inDegree;//�¼��ڵ����ȼ�¼������������֮ǰ����preList�ĳ��Ƚ��г�ʼ��
	int name;//�¼���ţ�������������˳������������ʼֵΪ0��Ч
	bool initInDegree();//��ʼ���ڵ����
//AOE:����Ve(�¼����翪ʼʱ��),Vl(�¼���ٿ�ʼʱ��)
	float ve;
	float vl;
	bool isKeyEvent;//��ʾ�Ƿ��ǹؼ��¼�
//������Ϣ
	float x,y;//λ��
	void draw();//����
//ѡ����Ϣ
	//bool selected;//��ѡ��
	bool pick(float x0,float y0);//ѡ��
	virtual void move(float x,float y);//�ƶ�
	virtual void moveTo(float x,float y);
	virtual bool up(float x,float y,Event*evt);
	virtual void drawTips();
};

