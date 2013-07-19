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

//初始化事件的入度个数，同时检查事件的出入度是否合理
bool Event::initInDegree()
{
	bool flag=true;
	if(type==START_EVENT)
	{
		flag=(preList.size()==0&&nextList.size()>0);//开始节点入度必须是0，出度必须大于0
	}
	else if(type==END_EVENT)
	{
		flag=(preList.size()>0&&nextList.size()==0);//结束节点入度必须大于0，出度必须是0
	}
	else if(type==NORMAL_EVENT)
	{
		flag=(preList.size()>0&&nextList.size()>0);//一般节点入度，出度必须大于0
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
	if(selected)//选中
		r=1.1f*R;
	switch(type)
	{
	case NORMAL_EVENT:
		if(isKeyEvent)//关键事件标示
			glColor4f(1,0,0,0.5);
		else
			glColor4f(1,1,1,0.5);break;
	case START_EVENT:glColor4f(0,1,0,0.5);break;
	case END_EVENT:glColor4f(1,1,0,0.5);break;
	}
	if(selected)
		glColor4f(0.5,0.5,1,0.5);//选中标示
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
	glRasterPos2f(-3,-2);//指定位置
	if(name!=0)//经过拓扑排序后的名称
		drawString(name);//绘制字体
	//结束符内环
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
	//开始箭头
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
	//重新计算关联活动的坐标
	list<Activity*>::iterator i;
	for(i=preList.begin();i!=preList.end();++i)//当前事件的所有的前驱活动
	{
		(*i)->rePos();
	}
	for(i=nextList.begin();i!=nextList.end();++i)//当前事件的所有的后继活动
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
	glRasterPos2f(0, -60);//指定位置
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
	glRasterPos2f(0, -80);//指定位置
	drawString("Precursor activity number:");drawString((int)preList.size());
	drawString(" Successor activity number:");drawString((int)nextList.size());
	glRasterPos2f(0, -100);//指定位置
	if(name!=0)
	{
		drawString("Earlist time:");drawString((int)ve);
		drawString(" Latest time:");drawString((int)vl);
		if(isKeyEvent)drawString(" <Key event>");
	}	
}