#pragma once
#include"StdAfx.h"
#include"Graphic.h"
class Activity:Graphic
{
public:
//������Ϣ
	Event*startEvent;//��ʼ�¼�
	Event*endEvent;//�����¼�
	float timeCost;//���Ҫʱ�俪��
	Activity(Event*start,Event*end,float cost);
	void rePos();//�����µ��¼��������¼�������
	~Activity(void);
//AOE:����Ae(����翪ʼʱ��),Al(���ٿ�ʼʱ��)
	float ae;
	float al;
	bool isKeyActivity;//��ʾ�Ƿ��ǹؼ��
//������Ϣ
	float x0,y0,x1,y1;//����յ�λ��
	float alpha;//б�ʣ�[-PI/2,3*PI/2]
	float len;//����
	void draw();//���ƻ
//ѡ����Ϣ
	//int selected;//��ѡ��:-1-ǰ�ˣ�1-��ˣ�0-δѡ��
	bool pick(float x,float y);//ѡ��
	virtual void move(float x,float y);//�ƶ�
	virtual void moveTo(float x,float y);
	virtual bool up(float x,float y,Event*evt);
	virtual void drawTips();
};

