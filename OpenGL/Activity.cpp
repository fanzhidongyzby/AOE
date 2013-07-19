#include"StdAfx.h"
#include "Activity.h"
#include"Event.h"
//#include"Event.h"
Activity::Activity(Event*start,Event*end,float cost)
{
	if(cost==0)
	{
		srand((unsigned int)time(0));
		cost=rand()%10+1.0f;
	}
	if(end!=NULL)
	{
		this->startEvent=start;
		this->endEvent=end;
		this->timeCost=cost;
		this->isKeyActivity=false;
		selected=false;
		rePos();
	}
	else //�½��
	{
		this->startEvent=start;
		this->endEvent=end;
		this->timeCost=cost;
		this->isKeyActivity=false;
		selected=1;//ѡ��ǰ��
	}
}
void Activity::rePos()
{
	//�����¼�λ�ü�����ʼ�յ�
	float sx=startEvent->x;
	float sy=startEvent->y;
	float ex=endEvent->x;
	float ey=endEvent->y;
	alpha=(float)atan((ey-sy)/(ex-sx));
	if(ex<sx)alpha+=PI;
	float r=R;
	x0=r*cos(alpha)+sx;
	y0=r*sin(alpha)+sy;
	x1=r*cos(alpha+PI)+ex;
	y1=r*sin(alpha+PI)+ey;
	len=sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
}

Activity::~Activity(void)
{
}

void Activity::draw()
{
	float al=8;
	if(selected!=0)//ѡ��
		glColor4f(1,0.5f,0.5f,1);
	else
	{
		if(this->isKeyActivity)
			glColor4f(1,1,0,1);
		else
			glColor4f(0.7f,0.7f,0.7f,1);
	}
	glPushMatrix();
	glTranslatef(x0,y0,0);
	glRotatef(alpha*180/PI,0,0,1);
	float w=0.5f;
	if(selected!=0)//ѡ��
		w=0.8f;
	//����
	glBegin(GL_QUADS);
	glVertex3f(0,w,0);
	glVertex3f(0,-w,0);
	glVertex3f(len-al,-w,0);
	glVertex3f(len-al,w,0);
	glEnd();
	//Ȩֵ
	glRasterPos2f(len/2, 5);//ָ��λ��
	drawString((int)timeCost);//��������
	//���
	if(selected==-1)//ѡ��ǰ��
		glColor4f(1,0.5f,0.5f,1);
	else 
	{
		if(this->isKeyActivity)
			glColor4f(1,1,0,1);
		else
			glColor4f(0.7f,0.7f,0.7f,1);
	}
	glBegin(GL_TRIANGLE_FAN);
	float r=1.5f;
	glVertex3f(0,0,0);
	for(float i=0;i<=360;i+=10)
	{
		glVertex3f(r*(float)cos(i*PI/180),r*(float)sin(i*PI/180),0);
	}
	glEnd();
	//��ͷ
	if(selected==1)
		glColor4f(1,0.5f,0.5f,1);
	else
	{
		if(this->isKeyActivity)
			glColor4f(1,1,0,1);
		else
			glColor4f(0.7f,0.7f,0.7f,1);
	}
	glBegin(GL_TRIANGLES);
	glVertex3f(len,0,0);
	glVertex3f(len-al,al/4,0);
	glVertex3f(len-al,-al/4,0);
	glEnd();
	glPopMatrix();
}

bool Activity::pick(float x,float y)
{
	double x1, x2, y1, y2, d0, d1, d2, d3, x0, y0, ave;

	x1 = this->x0;
	y1 = this->y0;
	x2 = this->x1;
	y2 = this->y1;

	//�������
	x0 = x;
	y0 = y;
	bool flag=false;
	if (((x0 >=x1-1 && x0 <=x2+1) || (x0 >=x2-1 && x0 <= x1+1))//�жϵ��λ���Ƿ��ڷ�Χ����
		&&((y0 >=y1-1 && y0 <=y2+1) || (y0 >=y2-1 && y0 <= y1+1)))
	{
		d1 = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));//���λ�õ�������
		d2 = sqrt((x2 - x0) * (x2 - x0) + (y2 - y0) * (y2 - y0));//���λ�õ��յ����
		d3 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));//ֱ�߾���
		selected=(d1<d2)?-1:1;//����ѡ���ֱ�ߵ��ĸ���λ
		ave = (d1 + d2 + d3) / 2;

		d0 = sqrt(ave * (ave - d1) * (ave - d2) * (ave - d3)) / d3;//���λ�õ�ֱ�ߵľ���

		if (d0 < 2)
		{
			flag=true;
		}
	}
	if(!flag)//û��ѡ��
	{
		selected=0;
	}
	return flag;
}
void Activity::move(float x,float y)
{
	if(selected==-1)//ѡ�����,��Ȼ���Ѿ����ڵĻ����Ҫ�޸���ʼ�㣬�յ�϶�����
	{
		moveTo(x0+x,y0+y);
	}
	else if(selected==1)//ѡ���յ㣬�������޸��յ㣬Ҳ�����Ǵ����µĻ
	{
		moveTo(x1+x,y1+y);
	}
}
void Activity::moveTo(float x,float y)
{
	float sx,sy,ex,ey;
	if(selected==-1)//ѡ�����,��Ȼ���Ѿ����ڵĻ����Ҫ�޸���ʼ�㣬�յ�϶�����
	{
		sx=x;
		sy=y;
		ex=endEvent->x;
		ey=endEvent->y;
		alpha=(float)atan((ey-sy)/(ex-sx));
		if(ex<sx)alpha+=PI;
		float r=R;
		x0=sx;
		y0=sy;
		x1=r*cos(alpha+PI)+ex;
		y1=r*sin(alpha+PI)+ey;
		len=sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
	}
	else if(selected==1)//ѡ���յ㣬�������޸��յ㣬Ҳ�����Ǵ����µĻ
	{
		sx=startEvent->x;
		sy=startEvent->y;
		ex=x;
		ey=y;
		alpha=(float)atan((ey-sy)/(ex-sx));
		if(ex<sx)alpha+=PI;
		float r=R;
		x0=r*cos(alpha)+sx;
		y0=r*sin(alpha)+sy;
		x1=ex;
		y1=ey;
		len=sqrt((x1-x0)*(x1-x0)+(y1-y0)*(y1-y0));
	}	
}

bool Activity::up(float x,float y,Event*evt)
{
	if(selected==-1)//�ƶ���ʼ�㣬�϶����յ�
	{
		if(evt==NULL||evt->type==END_EVENT||this->endEvent==evt)//���Բ�����Ľڵ�
			return false;	
		//Ŀ�겻�����Ѿ��͵�ǰ�ڵ���ڻ�Ľڵ�
		if(this->endEvent->isConnected(evt))
			return false;
		//���ԭ���Ĺ�ϵ
		this->startEvent->nextList.remove(this);
		//�ؽ���ǰ��ϵ
		this->startEvent=evt;
		evt->nextList.push_back(this);
		rePos();
		return true;
	}
	else if(selected==1)//�ƶ���ʼ�ڵ�
	{
		if(evt==NULL||evt->type==START_EVENT||this->startEvent==evt)//���Բ�����Ľڵ�
			return false;	
		//Ŀ�겻�����Ѿ��͵�ǰ�ڵ���ڻ�Ľڵ�
		if(this->startEvent->isConnected(evt))
			return false;
		//���ԭ���Ĺ�ϵ
		if(this->endEvent!=NULL)//���½�·��
		{
			this->endEvent->preList.remove(this);//��Ҫ���ԭ����ϵ
		}
		//�ؽ���ǰ��ϵ
		this->endEvent=evt;
		evt->preList.push_back(this);
		rePos();
		return true;
	}
	return false;
}

void Activity::drawTips()
{
	glColor4f(0,1,0,0.8f);
	glRasterPos2f(0, -60);//ָ��λ��
	drawString("Activity ");
	if(startEvent->name!=0)
	{
		drawString("<");
		drawString(startEvent->name);
		drawString(",");
		drawString(endEvent->name);
		drawString("> ");
	}
	drawString("timeCost:");
	drawString((int)timeCost);
	if(startEvent->name!=0)
	{
		glRasterPos2f(0, -80);//ָ��λ��
		drawString("Earlist time:");drawString((int)ae);
		drawString(" Latest time:");drawString((int)al);
		if(isKeyActivity)drawString(" <Key activity>");
	}
}