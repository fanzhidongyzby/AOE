#include"stdafx.h"

//selectFont(48, ANSI_CHARSET, "Comic Sans MS");
/*
�ַ���:
Ӣ�����������ANSI_CHARSET
�����������������GB2312_CHARSET
�����������������CHINESEBIG5_CHARSET
���ĵ�Windowsϵͳ��Ҳ����ֱ����DEFAULT_CHARSET��ʾĬ���ַ���
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
		// ����ǵ�һ�ε��ã�ִ�г�ʼ��
		// Ϊÿһ��ASCII�ַ�����һ����ʾ�б�
		isFirstCall = 0;
		selectFont(20, ANSI_CHARSET, "Courier New");//��������
		// ����MAX_CHAR����������ʾ�б���
		lists = glGenLists(MAX_CHAR);
		// ��ÿ���ַ��Ļ������װ����Ӧ����ʾ�б���
		wglUseFontBitmaps(wglGetCurrentDC(),0, MAX_CHAR, lists);
	}
	// ����ÿ���ַ���Ӧ����ʾ�б�����ÿ���ַ�
	for(; *str!='\0'; ++str)
		glCallList(lists + *str);
}

void drawString(int x)
{
	//���֧��9λ�Ǹ�����
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
