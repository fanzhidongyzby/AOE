
// OpenGLView.h : COpenGLView ��Ľӿ�
//

#pragma once
#include "OpenGLDoc.h"
#include"Program.h"
class COpenGLView : public CView
{
protected: // �������л�����
	COpenGLView();
	DECLARE_DYNCREATE(COpenGLView)

// ����
public:
	COpenGLDoc* GetDocument() const;
	//^v^
	int height;
	int width;
	CPoint lbDownPos;//�����������
	bool islbDown;//����Ƿ���
	bool addingEvent;//����¼�
	bool addingActivity;//��ӻ

	bool lockScr;//��ס��Ļ����ֹ�϶�

	HGLRC m_hRC; //Rendering Context      
	CDC* m_pDC; //Device Context  
	//^v^
	Program pro;
	float centerX,centerY;
	float sizeX,sizeY;

	GLint viewport[4];   
    GLdouble modelview[16], projection[16]; 
	void getRelativePos(CPoint p,float &x,float &y);//��GDI����ת��Ϊ�任ǰ�������

// ����
public:
	//^v^
	bool InitializeOpenGL();
	bool SetupPixelFormat();
	void RenderScene();
	//^v^
// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~COpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // OpenGLView.cpp �еĵ��԰汾
inline COpenGLDoc* COpenGLView::GetDocument() const
   { return reinterpret_cast<COpenGLDoc*>(m_pDocument); }
#endif

