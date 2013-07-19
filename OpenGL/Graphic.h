#pragma once
class Graphic
{
public:
	int selected;
	Graphic(void);
	~Graphic(void);
	virtual void move(float x,float y)=0;//移动图形
	virtual void moveTo(float x,float y)=0;//移动图形到指定位置
	virtual bool up(float x,float y,Event*evt)=0;//释放
	virtual void drawTips()=0;//绘制提示
};

