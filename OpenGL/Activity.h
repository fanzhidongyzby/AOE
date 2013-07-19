#pragma once
#include"StdAfx.h"
#include"Graphic.h"
class Activity:Graphic
{
public:
//基本信息
	Event*startEvent;//开始事件
	Event*endEvent;//结束事件
	float timeCost;//活动需要时间开销
	Activity(Event*start,Event*end,float cost);
	void rePos();//根据新的事件坐标重新计算活动坐标
	~Activity(void);
//AOE:计算Ae(活动最早开始时间),Al(活动最迟开始时间)
	float ae;
	float al;
	bool isKeyActivity;//标示是否是关键活动
//绘制信息
	float x0,y0,x1,y1;//起点终点位置
	float alpha;//斜率：[-PI/2,3*PI/2]
	float len;//长度
	void draw();//绘制活动
//选择信息
	//int selected;//被选中:-1-前端；1-后端；0-未选中
	bool pick(float x,float y);//选择
	virtual void move(float x,float y);//移动
	virtual void moveTo(float x,float y);
	virtual bool up(float x,float y,Event*evt);
	virtual void drawTips();
};

