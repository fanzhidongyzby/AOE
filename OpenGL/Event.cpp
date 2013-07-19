#include"stdafx.h"
#include "Event.h"
#include"Activity.h"
#include"MainFrm.h"
#include"OpenGLView.h"
Event::Event(EVENT_TYPE type,float x,float y)
{
	this->type=type;
	this->name=0;
	this->isKeyEvent=false;
	this->x=x;
	this->y=y;
	selected=false;
}


Event::~Event(void)
{
}

//��ʼ���¼�����ȸ�����ͬʱ����¼��ĳ�����Ƿ����
bool Event::initInDegree()
{
	bool flag=true;
	if(type==START_EVENT)
	{
		flag=(preList.size()==0&&nextList.size()>0);//��ʼ�ڵ���ȱ�����0�����ȱ������0
	}
	else if(type==END_EVENT)
	{
		flag=(preList.size()>0&&nextList.size()==0);//�����ڵ���ȱ������0�����ȱ�����0
	}
	else if(type==NORMAL_EVENT)
	{
		flag=(preList.size()>0&&nextList.size()>0);//һ��ڵ���ȣ����ȱ������0
	}
	this->inDegree=this->preList.size();
	return flag;
}
bool Event::isConnected(Event*other)
{
	list<Activity*>::iterator actIt;
	for(actIt=preList.begin();actIt!=preList.end();++actIt)
	{
		if((*actIt)->startEvent==other)
			return true;
	}
	for(actIt=nextList.begin();actIt!=nextList.end();++actIt)
	{
		if((*actIt)->endEvent==other)
			return true;
	}
	return false;
}

void Event::draw()
{
	float r=R;
	if(selected)//ѡ��
		r=1.1f*R;
	switch(type)
	{
	case NORMAL_EVENT:
		if(isKeyEvent)//�ؼ��¼���ʾ
			glColor4f(1,0,0,0.5);
		else
			glColor4f(1,1,1,0.5);break;
	case START_EVENT:glColor4f(0,1,0,0.5);break;
	case END_EVENT:glColor4f(1,1,0,0.5);break;
	}
	if(selected)
		glColor4f(0.5,0.5,1,0.5);//ѡ�б�ʾ
	glPushMatrix();
	glTranslatef(x,y,0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0,0,0);
	for(float i=0;i<=360;i+=10)
	{
		glVertex3f(r*(float)cos(i*PI/180),r*(float)sin(i*PI/180),0);
	}
	glEnd();
	glColor4f(0,0,0,1);
	glRasterPos2f(-3,-2);//ָ��λ��
	if(name!=0)//������������������
		drawString(name);//��������
	//�������ڻ�
	if(type==END_EVENT)
	{
		glColor4f(0,0,0,1);
		glBegin(GL_LINES);
		for(float i=0;i<=360;i+=10)
		{
			glVertex3f(r*0.75f*(float)cos(i*PI/180),r*0.75f*(float)sin(i*PI/180),0);
		}
		glEnd();
	}
	//��ʼ��ͷ
	if(type==START_EVENT)
	{
		glColor4f(1,0,0,1);
		glBegin(GL_LINES);
		glVertex3f(-R,0,0);
		glVertex3f(-2*R,R/2.0f,0);
		glVertex3f(-R,0,0);
		glVertex3f(-2*R,-R/2.0f,0);
		glVertex3f(-3*R,R/4.0f,0);
		glVertex3f(-1.5f*R,R/4.0f,0);
		glVertex3f(-3*R,-R/4.0f,0);
		glVertex3f(-1.5f*R,-R/4.0f,0);
		glEnd();
	}
	glPopMatrix();
}

bool Event::pick(float x0,float y0)
{
	float disp=sqrt((x0-x)*(x0-x)+(y0-y)*(y0-y));
	return selected=(disp<=R);
}

void Event::move(float x,float y)
{
	this->x+=x;
	this->y+=y;
	//���¼�������������
	list<Activity*>::iterator i;
	for(i=preList.begin();i!=preList.end();++i)//��ǰ�¼������е�ǰ���
	{
		(*i)->rePos();
	}
	for(i=nextList.begin();i!=nextList.end();++i)//��ǰ�¼������еĺ�̻
	{
		(*i)->rePos();
	}
}
void Event::moveTo(float x,float y)
{}

bool Event::up(float x,float y,Event*evt)
{
	return true;
}
void Event::drawTips()
{
	glColor4f(0,1,0,0.8f);
	glRasterPos2f(0, -60);//ָ��λ��
	if(name!=0)
	{
		drawString("Event name:");
		drawString(name);
		drawString(" type:");
	}
	else
		drawString("Event type:");
	if(type==NORMAL_EVENT)drawString("Normal event");
	else if(type==START_EVENT)drawString("Start event");
	else if(type==END_EVENT)drawString("End event");	
	glRasterPos2f(0, -80);//ָ��λ��
	drawString("Precursor activity number:");drawString((int)preList.size());
	drawString(" Successor activity number:");drawString((int)nextList.size());
	glRasterPos2f(0, -100);//ָ��λ��
	if(name!=0)
	{
		drawString("Earlist time:");drawString((int)ve);
		drawString(" Latest time:");drawString((int)vl);
		if(isKeyEvent)drawString(" <Key event>");
	}	
}