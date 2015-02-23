#pragma once
#include "afxwin.h"
struct ITaskbarList3;

class CTaskBar :
	public CWnd
{
public:
	CTaskBar(HWND hWnd);
	virtual ~CTaskBar();
	bool Init();
protected:
	DECLARE_MESSAGE_MAP()
public:
	HWND m_hTargetWnd;	//在任务栏上显示ICON的窗口句柄(一般就是程序的主窗口)
	double m_lfProgress;	//进度条百分比
	ITaskbarList3*	m_pTaskBarList;	//任务栏列表接口
	bool m_bInit;		//是否正确初始化
	HICON m_hIcon;
	CString m_csDescription;
public:
	void Click(WPARAM wParam);
};

