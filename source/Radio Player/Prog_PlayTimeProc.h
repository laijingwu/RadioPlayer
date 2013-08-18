#pragma once


// CProg_PlayTimeProc

class CProg_PlayTimeProc : public CProgressCtrl
{
	DECLARE_DYNAMIC(CProg_PlayTimeProc)

public:
	CProg_PlayTimeProc();
	virtual ~CProg_PlayTimeProc();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	float m_fClickPercent;
	float m_nClickPos;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


