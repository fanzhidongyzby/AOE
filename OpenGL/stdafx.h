
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"glu32.lib")
#include <GL\gl.h>
#include <GL\glu.h>

#include<list>
using namespace std;
//��ǰ����������ֹ���໥���ô���
class Event;
class Activity;
//�¼����ͣ���ʼ����������ͨ�¼�
enum EVENT_TYPE{START_EVENT,NORMAL_EVENT,END_EVENT};
#define PI 3.14159265358979323846f
#define R 10.0f

// ASCII�ַ��ܹ�ֻ��0��127��һ��128���ַ�
#define MAX_CHAR 128
void drawString(const char* str);//����ASCLL�ַ���
void selectFont(int size, int charset, const char* face);//ѡ������
void drawString(int x);
