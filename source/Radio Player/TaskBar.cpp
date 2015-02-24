#include "stdafx.h"
#include "TaskBar.h"
#include "Resource.h"
#include "Radio PlayerDlg.h"
#include "Radio Player.h"

BEGIN_MESSAGE_MAP(CTaskBar, CWnd)
END_MESSAGE_MAP()

CTaskBar::CTaskBar(HWND hWnd)
{
	m_hTargetWnd = hWnd;
	m_lfProgress = 0.0;
	m_pTaskBarList = NULL;
	m_bInit = FALSE;
	m_hIcon = NULL;
	g_hBarPlayIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICONMUSIC1));
	g_hBarPauseIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICONPAUSE));
	g_hBtnL = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICONLAST));
	g_hBtnMPlay = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICONPLAY));
	g_hBtnMPause = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICONPAUSE));
	g_hBtnR = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICONNEXT));
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

	m_pTaskBarList->SetProgressState(m_hTargetWnd, TBPF_NOPROGRESS);	//TBPF_INDETERMINATE

	m_pTaskBarList->SetThumbnailTooltip(m_hTargetWnd, L"Radio Player");

	THUMBBUTTONMASK dwMask = THB_ICON | THB_TOOLTIP;
	THUMBBUTTON thbButtons[3];
	thbButtons[0].dwMask = dwMask;
	thbButtons[0].iId = 0;
	thbButtons[0].hIcon = g_hBtnL;
	StringCbCopy(thbButtons[0].szTip, sizeof (thbButtons[0].szTip), TEXT("Last"));
	thbButtons[1].dwMask = dwMask;
	thbButtons[1].iId = 1;
	thbButtons[1].hIcon = g_hBtnMPlay;
	StringCbCopy(thbButtons[1].szTip, sizeof (thbButtons[1].szTip), TEXT("Play"));
	thbButtons[2].dwMask = dwMask;
	thbButtons[2].iId = 2;
	thbButtons[2].hIcon = g_hBtnR;
	StringCbCopy(thbButtons[2].szTip, sizeof (thbButtons[2].szTip), TEXT("Next"));
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
		//AfxMessageBox(L"L button clicked!");
		::PostMessage(m_hTargetWnd, WM_COMMAND, MAKEWPARAM(IDC_BTN_LASTRADIO, BN_CLICKED), NULL);
		break;
	case 1:
		::PostMessage(m_hTargetWnd, WM_COMMAND, MAKEWPARAM(IDC_BTN_PLAY, BN_CLICKED), NULL);
		//AfxMessageBox(L"M button clicked!");
		break;
	case 2:
		::PostMessage(m_hTargetWnd, WM_COMMAND, MAKEWPARAM(IDC_BTN_NEXTRADIO, BN_CLICKED), NULL);
		//AfxMessageBox(L"R button clicked!");
		break;
	}
	return;
}


void CTaskBar::SetTip(LPCWSTR pwszTip)
{
	m_pTaskBarList->SetThumbnailTooltip(m_hTargetWnd, pwszTip);
	return;
}


void CTaskBar::SetProgState(TBPFLAG tbp_flag)
{
	m_pTaskBarList->SetProgressState(m_hTargetWnd, tbp_flag);
	return;
}


void CTaskBar::SetIcon(HICON g_hIcon)
{
	m_pTaskBarList->SetOverlayIcon(m_hTargetWnd, g_hIcon, L"Radio Player");
	return;
}


void CTaskBar::UpdateButton(UINT cButtons, LPTHUMBBUTTON pButton)
{
	m_pTaskBarList->ThumbBarUpdateButtons(m_hTargetWnd, cButtons, pButton);
	return;
}


void CTaskBar::SetProgPos(ULONG ulPos, ULONG ulMax)
{
	if (ulPos <= ulMax)
		m_pTaskBarList->SetProgressValue(m_hTargetWnd, ulPos, ulMax);
	return;
}


void CTaskBar::SetPlayState(CString strTitle)
{
	SetTip(strTitle + L" - Radio Player");
	SetIcon(g_hBarPlayIcon);
	THUMBBUTTONMASK dwMask = THB_ICON | THB_TOOLTIP;
	THUMBBUTTON thbButtons;
	thbButtons.dwMask = dwMask;
	thbButtons.iId = 1;
	thbButtons.hIcon = g_hBtnMPause;
	StringCbCopy(thbButtons.szTip, sizeof (thbButtons.szTip), L"Pause");
	UpdateButton(1, &thbButtons);
	SetProgState(TBPF_NORMAL);
	//SetProgPos(0, 10000);
	return;
}


void CTaskBar::SetPauseState()
{
	SetTip(L"Radio Player");
	SetIcon(g_hBarPauseIcon);
	THUMBBUTTONMASK dwMask = THB_ICON | THB_TOOLTIP;
	THUMBBUTTON thbButtons;
	thbButtons.dwMask = dwMask;
	thbButtons.iId = 1;
	thbButtons.hIcon = g_hBtnMPlay;
	StringCbCopy(thbButtons.szTip, sizeof (thbButtons.szTip), L"Play");
	UpdateButton(1, &thbButtons);
	SetProgState(TBPF_PAUSED);
	return;
}


void CTaskBar::SetStopState()
{
	SetTip(L"Radio Player");
	SetIcon();
	THUMBBUTTONMASK dwMask = THB_ICON | THB_TOOLTIP;
	THUMBBUTTON thbButtons;
	thbButtons.dwMask = dwMask;
	thbButtons.iId = 1;
	thbButtons.hIcon = g_hBtnMPlay;
	StringCbCopy(thbButtons.szTip, sizeof (thbButtons.szTip), L"Play");
	UpdateButton(1, &thbButtons);
	SetProgState(TBPF_NOPROGRESS);
	return;
}
