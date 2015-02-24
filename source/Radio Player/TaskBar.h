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
	HWND m_hTargetWnd;	//������������ʾICON�Ĵ��ھ��(һ����ǳ����������)
	double m_lfProgress;	//�������ٷֱ�
	ITaskbarList3*	m_pTaskBarList;	//�������б�ӿ�
	bool m_bInit;		//�Ƿ���ȷ��ʼ��
	HICON m_hIcon;
	CString m_csDescription;
public:
	void Click(WPARAM wParam);
	void SetTip(LPCWSTR pwszTip);
	void SetProgState(TBPFLAG tbp_flag);
	void SetIcon(HICON g_hIcon = NULL);
	void UpdateButton(UINT cButtons, LPTHUMBBUTTON pButton);
	void SetProgPos(ULONG ulPos, ULONG ulMax);
	void SetPlayState(CString strTitle);
	HICON g_hBarPlayIcon;
	HICON g_hBarPauseIcon;
	HICON g_hBtnL;
	HICON g_hBtnMPlay;
	HICON g_hBtnMPause;
	HICON g_hBtnR;
	void SetPauseState();
	void SetStopState();
};

