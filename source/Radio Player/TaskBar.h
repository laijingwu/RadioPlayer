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
};

