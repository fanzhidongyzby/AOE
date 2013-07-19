
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "OpenGL.h"
#include "OpenGLView.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

	BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
		ON_COMMAND(ID_FULLSCR, &CMainFrame::OnFullscr)
		ON_WM_GETMINMAXINFO()
	END_MESSAGE_MAP()

	// CMainFrame 构造/析构

	CMainFrame::CMainFrame()
	{
		// TODO: 在此添加成员初始化代码
		m_bFullScreen=false;
	}

	CMainFrame::~CMainFrame()
	{
	}

	BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
	{
		if( !CFrameWnd::PreCreateWindow(cs) )
			return FALSE;
		// TODO: 在此处通过修改
		//  CREATESTRUCT cs 来修改窗口类或样式
		this->SetTitle("AOE操作演示");
		return TRUE;
	}

	// CMainFrame 诊断

#ifdef _DEBUG
	void CMainFrame::AssertValid() const
	{
		CFrameWnd::AssertValid();
	}

	void CMainFrame::Dump(CDumpContext& dc) const
	{
		CFrameWnd::Dump(dc);
	}
#endif //_DEBUG


	// CMainFrame 消息处理程序


	//void CMainFrame::On2d()
	//{
	//	// TODO: 在此添加命令处理程序代码
	//	CMenu *mainMenu=::AfxGetApp()->m_pMainWnd->GetMenu();
	//	mainMenu->CheckMenuItem(ID_3D,MF_UNCHECKED);
	//	mainMenu->CheckMenuItem(ID_2D,MF_CHECKED);
	//	COpenGLView::view_mode=1;
	//	this->Invalidate();
	//}
	void CMainFrame::switchScr(bool autoSw)
	{
		if(m_bFullScreen)
		{
			SetWindowPos(&CWnd::wndNoTopMost,100,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
			//ShowWindow(SW_HIDE);
			SetWindowPlacement(&m_OldWndPlacement);
			m_bFullScreen=false;
		}
		else
		{
			if(autoSw==false)
				return;
			GetWindowPlacement(&m_OldWndPlacement);
			CRect WindowRect;
			GetWindowRect(&WindowRect);
			CRect ClientRect;
			RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &ClientRect);
			ClientToScreen(&ClientRect);
			// 获取屏幕的分辨率
			int nFullWidth=GetSystemMetrics(SM_CXSCREEN);
			int nFullHeight=GetSystemMetrics(SM_CYSCREEN);
			// 将除控制条外的客户区全屏显示到从(0,0)到(nFullWidth, nFullHeight)区域, 将(0,0)和(nFullWidth, nFullHeight)两个点外扩充原窗口和除控制条之外的客
			//户区位置间的差值, 就得到全屏显示的窗口位置
			m_FullScreenRect.left=WindowRect.left-ClientRect.left-2;
			m_FullScreenRect.top=WindowRect.top-ClientRect.top-2;
			m_FullScreenRect.right=WindowRect.right-ClientRect.right+nFullWidth+2;
			m_FullScreenRect.bottom=WindowRect.bottom-ClientRect.bottom+nFullHeight+2;
			m_bFullScreen=true; // 设置全屏显示标志为 TRUE
			// 进入全屏显示状态
			WINDOWPLACEMENT wndpl;
			wndpl.length=sizeof(WINDOWPLACEMENT);
			wndpl.flags=0;
			wndpl.showCmd=SW_SHOWNORMAL;
			wndpl.rcNormalPosition=m_FullScreenRect;
			SetWindowPlacement(&wndpl);
			SetWindowPos(&CWnd::wndTopMost,100,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		}
	}

	void CMainFrame::OnFullscr()
	{
		// TODO: 在此添加命令处理程序代码
		switchScr();
	}


	void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		if(m_bFullScreen)
		{
			lpMMI->ptMaxSize.x=m_FullScreenRect.Width();
			lpMMI->ptMaxSize.y=m_FullScreenRect.Height();
			lpMMI->ptMaxPosition.x=0;
			lpMMI->ptMaxPosition.y=0;
			// 最大的Track尺寸也要改变
			lpMMI->ptMaxTrackSize.x=m_FullScreenRect.Width();
			lpMMI->ptMaxTrackSize.y=m_FullScreenRect.Height();
		}
		CFrameWnd::OnGetMinMaxInfo(lpMMI);
	}

