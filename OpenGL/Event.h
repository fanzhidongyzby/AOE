#pragma once
#include"stdafx.h"
#include"Graphic.h"
class Event:Graphic
{
public:
//基本信息
	EVENT_TYPE type;//标示事件的类型
	list <Activity*> preList;//记录事件的前驱活动列表
	list <Activity*> nextList;//记录事件的后继活动列表
	Event(EVENT_TYPE type,float x,float y);
	~Event(void);
	bool isConnected(Event*other);//判断是否和指定节点有关联
//AOV:拓扑排序信息
	int inDegree;//事件节点的入度记录，在拓扑排序之前根据preList的长度进行初始化
	int name;//事件编号，根据拓扑排序顺序编号命名，初始值为0无效
	bool initInDegree();//初始化节点入度
//AOE:计算Ve(事件最早开始时间),Vl(事件最迟开始时间)
	float ve;
	float vl;
	bool isKeyEvent;//标示是否是关键事件
//绘制信息
	float x,y;//位置
	void draw();//绘制
//选择信息
	//bool selected;//被选中
	bool pick(float x0,float y0);//选择
	virtual void move(float x,float y);//移动
	virtual void moveTo(float x,float y);
	virtual bool up(float x,float y,Event*evt);
	virtual void drawTips();
};

