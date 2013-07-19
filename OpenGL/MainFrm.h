
// MainFrm.h : CMainFrame 类的接口
//

#pragma once

class CMainFrame : public CFrameWnd
{

protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	// 特性
public:
	WINDOWPLACEMENT m_OldWndPlacement;//用来保存原窗口位置
	bool m_bFullScreen; //全屏显示标志
	CRect m_FullScreenRect;//表示全屏显示时的窗口位置
	// 操作
public:
	void switchScr(bool autoSw=true);
	// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


	// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnFullscr();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};


