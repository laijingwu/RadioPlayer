#include "stdafx.h"
#include "TaskBar.h"
#include "Resource.h"

BEGIN_MESSAGE_MAP(CTaskBar, CWnd)
END_MESSAGE_MAP()

CTaskBar::CTaskBar(HWND hWnd)
{
	m_hTargetWnd = hWnd;
	m_lfProgress = 0.0;
	m_pTaskBarList = NULL;
	m_bInit = FALSE;
	m_hIcon = NULL;
}


CTaskBar::~CTaskBar()
{
	if (m_pTaskBarList)
		m_pTaskBarList->Release();
}


bool CTaskBar::Init()
{
	HRESULT hr = ::CoCreateInstance(CLSID_TaskbarList, 0, CLSCTX_INPROC_SERVER,
		__uuidof(ITaskbarList3), (void**)&m_pTaskBarList);

	m_pTaskBarList->HrInit();

	m_pTaskBarList->SetProgressState(m_hTargetWnd, TBPF_INDETERMINATE);

	m_pTaskBarList->SetProgressValue(m_hTargetWnd, 80, 100);
	HICON g_hRed = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON4));
	HICON g_hGreen = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON4));
	HICON g_hBlue = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON4));
	m_pTaskBarList->SetOverlayIcon(m_hTargetWnd, g_hRed, TEXT("Error"));
	m_pTaskBarList->SetThumbnailTooltip(m_hTargetWnd, TEXT("Some information"));

	THUMBBUTTONMASK dwMask = THB_ICON | THB_TOOLTIP;
	THUMBBUTTON thbButtons[3];
	thbButtons[0].dwMask = dwMask;
	thbButtons[0].iId = 0;
	thbButtons[0].hIcon = g_hRed;
	StringCbCopy(thbButtons[0].szTip, sizeof (thbButtons[0].szTip), TEXT("Red"));
	thbButtons[1].dwMask = dwMask;
	thbButtons[1].iId = 1;
	thbButtons[1].hIcon = g_hGreen;
	StringCbCopy(thbButtons[1].szTip, sizeof (thbButtons[1].szTip), TEXT("Green"));
	thbButtons[2].dwMask = dwMask;
	thbButtons[2].iId = 2;
	thbButtons[2].hIcon = g_hBlue;
	StringCbCopy(thbButtons[2].szTip, sizeof (thbButtons[2].szTip), TEXT("Blue"));
	m_pTaskBarList->ThumbBarAddButtons(m_hTargetWnd, ARRAYSIZE(thbButtons), thbButtons);


	BOOL truth = TRUE;
	//DwmSetWindowAttribute(m_hTargetWnd, DWMWA_HAS_ICONIC_BITMAP, &truth, sizeof(truth));
	//DwmSetWindowAttribute(m_hTargetWnd, DWMWA_FORCE_ICONIC_REPRESENTATION, &truth, sizeof(truth));

	//m_bInit = bRet && bWin7 && SUCCEEDED(hr);
	//if (m_bInit)
	//{
	//	Reset();
	//}
	//return m_bInit;
	return true;
}


void CTaskBar::Click(WPARAM wParam)
{
	switch (LOWORD(wParam))
	{
	case 0:
		//SetForegroundWindow();
		AfxMessageBox(L"Red button clicked!");
		break;
	case 1:
		//SetForegroundWindow();
		break;
	case 2:
		//SetForegroundWindow();
		break;
	}
	return;
}
