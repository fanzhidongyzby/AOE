#pragma once
class Graphic
{
public:
	int selected;
	Graphic(void);
	~Graphic(void);
	virtual void move(float x,float y)=0;//�ƶ�ͼ��
	virtual void moveTo(float x,float y)=0;//�ƶ�ͼ�ε�ָ��λ��
	virtual bool up(float x,float y,Event*evt)=0;//�ͷ�
	virtual void drawTips()=0;//������ʾ
};

