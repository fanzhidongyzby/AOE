#pragma once
#include"stdafx.h"
#include"Graphic.h"
class Program
{
public:
	list <Event*> evtList;//所有事件列表
	list <Activity*> actList;//所有活动列表

	list<Event*> topoList;//拓扑排序后的事件顺序
	float period;//工期

	Program(void);
	~Program(void);
	Event* addEvent(EVENT_TYPE type,float x,float y);//添加一个事件
	void delEvent(Event*evt);//删除一个事件
	void addActivity(Event*start,Event*end,float cost,float x=0,float y=0);//添加一个活动
	void delActivity(Activity*act);//删除一个活动
	bool topoSort();//拓扑排序，根据拓扑顺序命名事件节点
	void KeyPath();//计算关键路径，计算每个事件和活动的Ve,Vl,Ae,Al,标示关键事件和关键活动,计算工期

	void draw();
	Graphic *gHit;//选中的图形
	Event *evtOver;//扫过的事件，为添加修改活动准备信息
	Graphic *gOver;//扫过的图形，显示用

	int pickType;//0-未选中；1-活动；-1-事件 选择结果
	void pick(float x,float y,bool hasRet=true);//选择,默认记录对象
	void move(float x,float y);//移动某个图形
	void up(float x,float y);//鼠标弹起

	void del(float x,float y);//删除图形
	void refresh();//清除所有对象的关键路径属性

//提示
	bool sortOk;//排序是否完成
	void drawTips();
};

