
// OpenGLView.cpp : COpenGLView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// COpenGLView 构造/析构
	COpenGLView::COpenGLView()
	{
		// TODO: 在此处添加构造代码
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
		// TODO: 在此处通过修改
		//  CREATESTRUCT cs 来修改窗口类或样式
		//^v^
		cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		//^v^
		return CView::PreCreateWindow(cs);
	}
	// COpenGLView 绘制
	void COpenGLView::OnDraw(CDC* /*pDC*/)
	{
		COpenGLDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		// TODO: 在此处为本机数据添加绘制代码
		//^v^
		::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );// Clear out the color & depth buffers
		RenderScene(); //具体的绘图函数，在RC 中绘制
		glFinish();
		::SwapBuffers( m_pDC->GetSafeHdc());// Now Swap the buffers
		//^v^
	}
	void COpenGLView::RenderScene()
	{
		glLoadIdentity();
		//绘图
		glPushMatrix();
		glTranslatef((float)-width/2,(float)height/2,0);
		if(lockScr)
		{
			glColor4f(1,0,0,0.8f);
			glRasterPos2f(0, -20);//指定位置
			drawString("Screen locked !");
		}
		else
		{
			glColor4f(0,1,0,0.8f);
			glRasterPos2f(0, -20);//指定位置
			drawString("Screen unlocked !");
		}
		
		if(pro.sortOk)
		{
			glColor4f(1,1,0,0.8f);
			glRasterPos2f(0, -40);//指定位置
			drawString("Topological sort completed ! Totally timecost:");
			drawString((int)pro.period);
		}	
		else
		{
			glColor4f(1,1,0,0.8f);
			glRasterPos2f(0, -40);//指定位置
			drawString("Topological sort failed !");
		}
		pro.drawTips();
		glPopMatrix();
		glTranslatef(centerX,centerY,0);
		glScalef(sizeX,sizeY,1);
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		pro.draw();
		

	}
	// COpenGLView 诊断
#ifdef _DEBUG
	void COpenGLView::AssertValid() const
	{
		CView::AssertValid();
	}
	void COpenGLView::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}
	COpenGLDoc* COpenGLView::GetDocument() const // 非调试版本是内联的
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
		//允许反走样混合
		glEnable (GL_LINE_SMOOTH);
		glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		////允许多边形填充偏移
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
	// COpenGLView 消息处理程序
	int COpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CView::OnCreate(lpCreateStruct) == -1)
			return -1;

		// TODO:  在此添加您专用的创建代码
		//^v^
		InitializeOpenGL();
		//^v^
		return 0;
	}
	void COpenGLView::OnDestroy()
	{
		CView::OnDestroy();

		// TODO: 在此处添加消息处理程序代码
		wglMakeCurrent(m_pDC->m_hDC,NULL); //释放与m_hDC 对应的RC
		wglDeleteContext(m_hRC); //删除RC if (m_pDC)
		delete m_pDC; //删除当前View 拥有的DC
	}
	BOOL COpenGLView::OnEraseBkgnd(CDC* pDC)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值

		//return CView::OnEraseBkgnd(pDC);
		//把这句话注释掉，否则，Window 会用白色背景来刷新,导致画面闪烁
		return TRUE; //只要空返回即可。
	}
	void COpenGLView::OnSize(UINT nType, int cx, int cy)
	{
		CView::OnSize(nType, cx, cy);
		// TODO: 在此处添加消息处理程序代码
		if ( 0 >= cx || 0 >= cy ) return;
		width = cx;
		height = cy;
		//初始化投影
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
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		if(nChar==VK_ESCAPE)
		{
			CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
			pFrame->switchScr(false);
		}
		else if(nChar=='E')//E按下,添加事件
		{
			addingEvent=true;
		}
		else if(nChar=='A')//A按下,添加活动
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
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		lbDownPos=point;
		islbDown=true;
		float x,y;
		this->getRelativePos(point,x,y);
		pro.pick(x,y);
		if(addingEvent)//添加事件
		{
			pro.addEvent(NORMAL_EVENT,x,y);
		}
		else if(addingActivity)//添加活动
		{
			pro.addActivity(NULL,NULL,0,x,y);
		}
		Invalidate();
		CView::OnLButtonDown(nFlags, point);
	}


	void COpenGLView::OnMouseMove(UINT nFlags, CPoint point)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		float x,y;
		this->getRelativePos(point,x,y);
		pro.pick(x,y,false);
		if(islbDown)
		{
			if(pro.pickType!=0)//选中物体
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
		// TODO: 在此添加消息处理程序代码和/或调用默认值
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
		// TODO: 在此添加消息处理程序代码和/或调用默认值
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
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		centerX=centerY=0;
		sizeX=sizeY=2;
		Invalidate();
		CView::OnMButtonDown(nFlags, point);
	}


	void COpenGLView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值
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
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		float x,y;
		this->getRelativePos(point,x,y);
		pro.del(x,y);
		pro.KeyPath();
		Invalidate();
		CView::OnRButtonDblClk(nFlags, point);
	}
