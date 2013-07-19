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
	else //新建活动
	{
		this->startEvent=start;
		this->endEvent=end;
		this->timeCost=cost;
		this->isKeyActivity=false;
		selected=1;//选择前端
	}
}
void Activity::rePos()
{
	//根据事件位置计算活动起始终点
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
	if(selected!=0)//选中
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
	if(selected!=0)//选中
		w=0.8f;
	//线条
	glBegin(GL_QUADS);
	glVertex3f(0,w,0);
	glVertex3f(0,-w,0);
	glVertex3f(len-al,-w,0);
	glVertex3f(len-al,w,0);
	glEnd();
	//权值
	glRasterPos2f(len/2, 5);//指定位置
	drawString((int)timeCost);//绘制字体
	//起点
	if(selected==-1)//选中前端
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
	//箭头
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

	//点击坐标
	x0 = x;
	y0 = y;
	bool flag=false;
	if (((x0 >=x1-1 && x0 <=x2+1) || (x0 >=x2-1 && x0 <= x1+1))//判断点击位置是否在范围以内
		&&((y0 >=y1-1 && y0 <=y2+1) || (y0 >=y2-1 && y0 <= y1+1)))
	{
		d1 = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));//点击位置到起点距离
		d2 = sqrt((x2 - x0) * (x2 - x0) + (y2 - y0) * (y2 - y0));//点击位置到终点距离
		d3 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));//直线距离
		selected=(d1<d2)?-1:1;//决定选择的直线的哪个部位
		ave = (d1 + d2 + d3) / 2;

		d0 = sqrt(ave * (ave - d1) * (ave - d2) * (ave - d3)) / d3;//点击位置到直线的距离

		if (d0 < 2)
		{
			flag=true;
		}
	}
	if(!flag)//没有选中
	{
		selected=0;
	}
	return flag;
}
void Activity::move(float x,float y)
{
	if(selected==-1)//选中起点,必然是已经存在的活动，将要修改起始点，终点肯定存在
	{
		moveTo(x0+x,y0+y);
	}
	else if(selected==1)//选中终点，可能是修改终点，也可能是创建新的活动
	{
		moveTo(x1+x,y1+y);
	}
}
void Activity::moveTo(float x,float y)
{
	float sx,sy,ex,ey;
	if(selected==-1)//选中起点,必然是已经存在的活动，将要修改起始点，终点肯定存在
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
	else if(selected==1)//选中终点，可能是修改终点，也可能是创建新的活动
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
	if(selected==-1)//移动起始点，肯定有终点
	{
		if(evt==NULL||evt->type==END_EVENT||this->endEvent==evt)//绝对不合理的节点
			return false;	
		//目标不能是已经和当前节点存在活动的节点
		if(this->endEvent->isConnected(evt))
			return false;
		//解除原来的关系
		this->startEvent->nextList.remove(this);
		//重建当前关系
		this->startEvent=evt;
		evt->nextList.push_back(this);
		rePos();
		return true;
	}
	else if(selected==1)//移动开始节点
	{
		if(evt==NULL||evt->type==START_EVENT||this->startEvent==evt)//绝对不合理的节点
			return false;	
		//目标不能是已经和当前节点存在活动的节点
		if(this->startEvent->isConnected(evt))
			return false;
		//解除原来的关系
		if(this->endEvent!=NULL)//非新建路径
		{
			this->endEvent->preList.remove(this);//需要解除原来关系
		}
		//重建当前关系
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
	glRasterPos2f(0, -60);//指定位置
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
		glRasterPos2f(0, -80);//指定位置
		drawString("Earlist time:");drawString((int)ae);
		drawString(" Latest time:");drawString((int)al);
		if(isKeyActivity)drawString(" <Key activity>");
	}
}