#include"stdafx.h"

//selectFont(48, ANSI_CHARSET, "Comic Sans MS");
/*
字符集:
英文字体可以用ANSI_CHARSET
简体中文字体可以用GB2312_CHARSET
繁体中文字体可以用CHINESEBIG5_CHARSET
中文的Windows系统，也可以直接用DEFAULT_CHARSET表示默认字符集
*/
void selectFont(int size, int charset, const char* face) 
{
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

void drawString(const char* str)
{
	static int isFirstCall = 1;
	static GLuint lists;
	if( isFirstCall ) { 
		// 如果是第一次调用，执行初始化
		// 为每一个ASCII字符产生一个显示列表
		isFirstCall = 0;
		selectFont(20, ANSI_CHARSET, "Courier New");//设置字体
		// 申请MAX_CHAR个连续的显示列表编号
		lists = glGenLists(MAX_CHAR);
		// 把每个字符的绘制命令都装到对应的显示列表中
		wglUseFontBitmaps(wglGetCurrentDC(),0, MAX_CHAR, lists);
	}
	// 调用每个字符对应的显示列表，绘制每个字符
	for(; *str!='\0'; ++str)
		glCallList(lists + *str);
}

void drawString(int x)
{
	//最大支持9位非负数字
	char str[10]={0};
	int i=8;
	do
	{
		str[i]=x%10+'0';
		x/=10;
		i--;
		if(i<0)return;
	}
	while(x!=0);
	drawString(&str[i+1]);
}
