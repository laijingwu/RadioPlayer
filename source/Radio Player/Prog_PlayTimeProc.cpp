// Prog_PlayTimeProc.cpp : 实现文件
//

#include "stdafx.h"
#include "Radio Player.h"
#include "Prog_PlayTimeProc.h"
#include "Radio PlayerDlg.h"


// CProg_PlayTimeProc

IMPLEMENT_DYNAMIC(CProg_PlayTimeProc, CProgressCtrl)

CProg_PlayTimeProc::CProg_PlayTimeProc()
{

}

CProg_PlayTimeProc::~CProg_PlayTimeProc()
{
}


BEGIN_MESSAGE_MAP(CProg_PlayTimeProc, CProgressCtrl)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CProg_PlayTimeProc 消息处理程序




void CProg_PlayTimeProc::OnLButtonUp(UINT nFlags, CPoint point)
{
	CRadioPlayerDlg *pDlg = (CRadioPlayerDlg*)AfxGetMainWnd();
	if(pDlg->PlayingStatus.PlayStatus == 1){
		AfxGetMainWnd()->KillTimer(3);
		DWORD nPos = GetPos();
		int nRangeMin,nRangeMax;
		GetRange(nRangeMin,nRangeMax);
		CRect rect;
		GetClientRect(rect);
		m_fClickPercent = (point.x - rect.left)/(float)rect.Width();
		m_nClickPos = nRangeMax * m_fClickPercent;
		SetPos((DWORD)m_nClickPos);
		::PostMessageW(AfxGetMainWnd()->m_hWnd,WM_PROGRESS_PLAYTIME,0,0);
		AfxGetMainWnd()->SetTimer(3,100,NULL);
		CProgressCtrl::OnLButtonUp(nFlags, point);
	}
}


void CProg_PlayTimeProc::OnMouseMove(UINT nFlags, CPoint point)
{
	CRadioPlayerDlg *pDlg = (CRadioPlayerDlg*)AfxGetMainWnd();
	if(pDlg->PlayingStatus.PlayStatus == 1 && nFlags == MK_LBUTTON){
		AfxGetMainWnd()->KillTimer(3);
		DWORD nPos = GetPos();
		int nRangeMin,nRangeMax;
		GetRange(nRangeMin,nRangeMax);
		CRect rect;
		GetClientRect(rect);
		m_fClickPercent = (point.x - rect.left)/(float)rect.Width();
		m_nClickPos = nRangeMax * m_fClickPercent;
		SetPos((DWORD)m_nClickPos);
		CProgressCtrl::OnMouseMove(nFlags, point);
	}
}
