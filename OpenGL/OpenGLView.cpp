
// OpenGLView.cpp : COpenGLView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "OpenGL.h"
#endif
#include "MainFrm.h"
#include "OpenGLDoc.h"
#include "OpenGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenGLView

IMPLEMENT_DYNCREATE(COpenGLView, CView)

	BEGIN_MESSAGE_MAP(COpenGLView, CView)
		ON_WM_CREATE()
		ON_WM_DESTROY()
		ON_WM_ERASEBKGND()
		ON_WM_SIZE()
		ON_WM_KEYDOWN()
		ON_WM_LBUTTONDOWN()
		ON_WM_MOUSEMOVE()
		ON_WM_LBUTTONUP()
		ON_WM_MOUSEWHEEL()
		ON_WM_MBUTTONDOWN()
		ON_WM_KEYUP()
		ON_WM_RBUTTONDBLCLK()
	END_MESSAGE_MAP()
	// COpenGLView ����/����
	COpenGLView::COpenGLView()
	{
		// TODO: �ڴ˴���ӹ������
		centerX=0;
		centerY=0;
		sizeX=2;
		sizeY=2;
		islbDown=false;
		addingEvent=false;
		addingActivity=false;
		lockScr=false;
	}
	COpenGLView::~COpenGLView()
	{
	}
	BOOL COpenGLView::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: �ڴ˴�ͨ���޸�
		//  CREATESTRUCT cs ���޸Ĵ��������ʽ
		//^v^
		cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		//^v^
		return CView::PreCreateWindow(cs);
	}
	// COpenGLView ����
	void COpenGLView::OnDraw(CDC* /*pDC*/)
	{
		COpenGLDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
		//^v^
		::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );// Clear out the color & depth buffers
		RenderScene(); //����Ļ�ͼ��������RC �л���
		glFinish();
		::SwapBuffers( m_pDC->GetSafeHdc());// Now Swap the buffers
		//^v^
	}
	void COpenGLView::RenderScene()
	{
		glLoadIdentity();
		//��ͼ
		glPushMatrix();
		glTranslatef((float)-width/2,(float)height/2,0);
		if(lockScr)
		{
			glColor4f(1,0,0,0.8f);
			glRasterPos2f(0, -20);//ָ��λ��
			drawString("Screen locked !");
		}
		else
		{
			glColor4f(0,1,0,0.8f);
			glRasterPos2f(0, -20);//ָ��λ��
			drawString("Screen unlocked !");
		}
		
		if(pro.sortOk)
		{
			glColor4f(1,1,0,0.8f);
			glRasterPos2f(0, -40);//ָ��λ��
			drawString("Topological sort completed ! Totally timecost:");
			drawString((int)pro.period);
		}	
		else
		{
			glColor4f(1,1,0,0.8f);
			glRasterPos2f(0, -40);//ָ��λ��
			drawString("Topological sort failed !");
		}
		pro.drawTips();
		glPopMatrix();
		glTranslatef(centerX,centerY,0);
		glScalef(sizeX,sizeY,1);
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		pro.draw();
		

	}
	// COpenGLView ���
#ifdef _DEBUG
	void COpenGLView::AssertValid() const
	{
		CView::AssertValid();
	}
	void COpenGLView::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}
	COpenGLDoc* COpenGLView::GetDocument() const // �ǵ��԰汾��������
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGLDoc)));
		return (COpenGLDoc*)m_pDocument;
	}
#endif //_DEBUG
	//^v^
	bool COpenGLView::InitializeOpenGL()
	{
		//Get a DC for the Client Area
		m_pDC = new CClientDC(this);
		//Failure to Get DC
		if(m_pDC == NULL)
		{
			MessageBox("Error Obtaining DC");
			return FALSE;
		}
		//Failure to set the pixel format
		if(!SetupPixelFormat())
		{
			return FALSE;
		}
		//Create Rendering Context
		m_hRC = ::wglCreateContext (m_pDC->GetSafeHdc ());
		//Failure to Create Rendering Context
		if(m_hRC == 0)
		{
			MessageBox("Error Creating RC");
			return FALSE;
		}
		//Make the RC Current
		if(::wglMakeCurrent (m_pDC->GetSafeHdc (), m_hRC)==FALSE)
		{
			MessageBox("Error making RC Current");
			return FALSE;
		}
		//Specify Black as the clear color
		//::glClearColor(1.0f,1.0f,1.0f,1.0f);
		::glClearColor(0.1f,0.1f,0.4f,1.0f);
		//Specify the back of the buffer as clear depth
		::glClearDepth(1.0f);
		//�����������
		glEnable (GL_LINE_SMOOTH);
		glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		////�����������ƫ��
		//glPolygonOffset(1,1);
		//glEnable(GL_POLYGON_OFFSET_FILL);
		return true;
	}
	bool COpenGLView::SetupPixelFormat()
	{
		static PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd
			1, // version number
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL // support window | support OpenGL|
			| PFD_DOUBLEBUFFER , //double buffered
			PFD_TYPE_RGBA, // RGBA type
			24, // 24-bit color depth
			0, 0, 0, 0, 0, 0, // color bits ignored
			0, 0, // no alpha buffer // shift bit ignored
			0, // no accumulation buffer
			0, 0, 0, 0, // accum bits ignored
			16, 0, // 16-bit z-buffer // no stencil buffer
			0, PFD_MAIN_PLANE, 0, // no auxiliary buffer // main layer// reserved
			0, 0, 0 // layer masks ignored
		};
		int m_nPixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
		if ( m_nPixelFormat == 0 ) 
			return FALSE;
		if ( ::SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd) == FALSE) 
			return FALSE;
		return true;
	}
	// COpenGLView ��Ϣ�������
	int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CView::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  �ڴ������ר�õĴ�������
		//^v^
		InitializeOpenGL();
		//^v^
		return 0;
	}
	void COpenGLView::OnDestroy()
	{
		CView::OnDestroy();

		// TODO: �ڴ˴������Ϣ����������
		wglMakeCurrent(m_pDC->m_hDC,NULL); //�ͷ���m_hDC ��Ӧ��RC
		wglDeleteContext(m_hRC); //ɾ��RC if (m_pDC)
		delete m_pDC; //ɾ����ǰView ӵ�е�DC
	}
	BOOL COpenGLView::OnEraseBkgnd(CDC* pDC)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

		//return CView::OnEraseBkgnd(pDC);
		//����仰ע�͵�������Window ���ð�ɫ������ˢ��,���»�����˸
		return TRUE; //ֻҪ�շ��ؼ��ɡ�
	}
	void COpenGLView::OnSize(UINT nType, int cx, int cy)
	{
		CView::OnSize(nType, cx, cy);
		// TODO: �ڴ˴������Ϣ����������
		if ( 0 >= cx || 0 >= cy ) return;
		width = cx;
		height = cy;
		//��ʼ��ͶӰ
		::glViewport(0, 0, width, height); // select the full client area
		// select the projection matrix and clear it
		::glMatrixMode(GL_PROJECTION);
		::glLoadIdentity();
		gluOrtho2D(-width/2, width/2,-height/2, height/2);
		//gluOrtho2D(0, width,height, 0);
		//gluOrtho2D(-width/2, width/2,height/2, -height/2);
		// switch back to the modelview matrix and clear it
		::glMatrixMode(GL_MODELVIEW);
		::glLoadIdentity();
		glGetIntegerv(GL_VIEWPORT, viewport); 
		glGetDoublev(GL_PROJECTION_MATRIX, projection);
	}
	//^v^

	void COpenGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		if(nChar==VK_ESCAPE)
		{
			CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
			pFrame->switchScr(false);
		}
		else if(nChar=='E')//E����,����¼�
		{
			addingEvent=true;
		}
		else if(nChar=='A')//A����,��ӻ
		{
			addingActivity=true;
		}
		CView::OnKeyDown(nChar, nRepCnt, nFlags);
	}

	void COpenGLView::getRelativePos(CPoint p,float &x,float &y)
	{
		GLdouble xx,yy,z;
		gluUnProject(p.x,height-p.y,0,modelview,projection,viewport,&xx, &yy, &z);
		x=(float)xx;
		y=(float)yy;
	}
	void COpenGLView::OnLButtonDown(UINT nFlags, CPoint point)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		lbDownPos=point;
		islbDown=true;
		float x,y;
		this->getRelativePos(point,x,y);
		pro.pick(x,y);
		if(addingEvent)//����¼�
		{
			pro.addEvent(NORMAL_EVENT,x,y);
		}
		else if(addingActivity)//��ӻ
		{
			pro.addActivity(NULL,NULL,0,x,y);
		}
		Invalidate();
		CView::OnLButtonDown(nFlags, point);
	}


	void COpenGLView::OnMouseMove(UINT nFlags, CPoint point)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		float x,y;
		this->getRelativePos(point,x,y);
		pro.pick(x,y,false);
		if(islbDown)
		{
			if(pro.pickType!=0)//ѡ������
			{
				float x0,y0;
				this->getRelativePos(lbDownPos,x0,y0);
				pro.move(x-x0,y-y0);
			}
			else if(!lockScr)
			{
				centerX+=point.x-lbDownPos.x;
				centerY+=-point.y+lbDownPos.y;
			}
			lbDownPos=point;
		}
		Invalidate();
		CView::OnMouseMove(nFlags, point);
	}


	void COpenGLView::OnLButtonUp(UINT nFlags, CPoint point)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		islbDown=false;
		float x,y;
		this->getRelativePos(point,x,y);
		pro.up(x,y);
		pro.KeyPath();
		Invalidate();
		CView::OnLButtonUp(nFlags, point);
	}


	BOOL COpenGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		if(zDelta>0)
		{
			sizeX+=0.1f;
			sizeY+=0.1f;
			if(sizeX>4)sizeX=4;
			if(sizeY>4)sizeY=4;
		}
		else
		{
			sizeX-=0.1f;
			sizeY-=0.1f;
			if(sizeX<2)sizeX=2;
			if(sizeY<2)sizeY=2;
		}
		Invalidate();
		return CView::OnMouseWheel(nFlags, zDelta, pt);
	}


	void COpenGLView::OnMButtonDown(UINT nFlags, CPoint point)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		centerX=centerY=0;
		sizeX=sizeY=2;
		Invalidate();
		CView::OnMButtonDown(nFlags, point);
	}


	void COpenGLView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		if(nChar=='E')
		{
			addingEvent=false;
		}
		else if(nChar=='A')
		{
			addingActivity=false;
		}
		else if(nChar==VK_SPACE)
		{
			lockScr=!lockScr;
			Invalidate();
		}
		CView::OnKeyUp(nChar, nRepCnt, nFlags);
	}


	void COpenGLView::OnRButtonDblClk(UINT nFlags, CPoint point)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		float x,y;
		this->getRelativePos(point,x,y);
		pro.del(x,y);
		pro.KeyPath();
		Invalidate();
		CView::OnRButtonDblClk(nFlags, point);
	}
