#pragma once
#include"stdafx.h"
#include"Graphic.h"
class Program
{
public:
	list <Event*> evtList;//�����¼��б�
	list <Activity*> actList;//���л�б�

	list<Event*> topoList;//�����������¼�˳��
	float period;//����

	Program(void);
	~Program(void);
	Event* addEvent(EVENT_TYPE type,float x,float y);//���һ���¼�
	void delEvent(Event*evt);//ɾ��һ���¼�
	void addActivity(Event*start,Event*end,float cost,float x=0,float y=0);//���һ���
	void delActivity(Activity*act);//ɾ��һ���
	bool topoSort();//�������򣬸�������˳�������¼��ڵ�
	void KeyPath();//����ؼ�·��������ÿ���¼��ͻ��Ve,Vl,Ae,Al,��ʾ�ؼ��¼��͹ؼ��,���㹤��

	void draw();
	Graphic *gHit;//ѡ�е�ͼ��
	Event *evtOver;//ɨ�����¼���Ϊ����޸Ļ׼����Ϣ
	Graphic *gOver;//ɨ����ͼ�Σ���ʾ��

	int pickType;//0-δѡ�У�1-���-1-�¼� ѡ����
	void pick(float x,float y,bool hasRet=true);//ѡ��,Ĭ�ϼ�¼����
	void move(float x,float y);//�ƶ�ĳ��ͼ��
	void up(float x,float y);//��굯��

	void del(float x,float y);//ɾ��ͼ��
	void refresh();//������ж���Ĺؼ�·������

//��ʾ
	bool sortOk;//�����Ƿ����
	void drawTips();
};

