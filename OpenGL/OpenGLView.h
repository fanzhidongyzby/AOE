
// OpenGLView.h : COpenGLView 类的接口
//

#pragma once
#include "OpenGLDoc.h"
#include"Program.h"
class COpenGLView : public CView
{
protected: // 仅从序列化创建
	COpenGLView();
	DECLARE_DYNCREATE(COpenGLView)

// 特性
public:
	COpenGLDoc* GetDocument() const;
	//^v^
	int height;
	int width;
	CPoint lbDownPos;//左键按下坐标
	bool islbDown;//左键是否按下
	bool addingEvent;//添加事件
	bool addingActivity;//添加活动

	bool lockScr;//锁住屏幕，禁止拖动

	HGLRC m_hRC; //Rendering Context      
	CDC* m_pDC; //Device Context  
	//^v^
	Program pro;
	float centerX,centerY;
	float sizeX,sizeY;

	GLint viewport[4];   
    GLdouble modelview[16], projection[16]; 
	void getRelativePos(CPoint p,float &x,float &y);//将GDI坐标转换为变换前相对坐标

// 操作
public:
	//^v^
	bool InitializeOpenGL();
	bool SetupPixelFormat();
	void RenderScene();
	//^v^
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~COpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // OpenGLView.cpp 中的调试版本
inline COpenGLDoc* COpenGLView::GetDocument() const
   { return reinterpret_cast<COpenGLDoc*>(m_pDocument); }
#endif

