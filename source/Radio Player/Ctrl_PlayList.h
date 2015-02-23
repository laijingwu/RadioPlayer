#pragma once
#include "afxcmn.h"
#include <vector>
using namespace std;

#define SELECT_NONE -1
#define WM_FROM_LISTCTRL WM_USER+435
#define LBUTTON_UP	-1

class CCtrl_PlayList :
	public CListCtrl
{
	DECLARE_DYNAMIC(CCtrl_PlayList)
public:
	CCtrl_PlayList();
	~CCtrl_PlayList();
protected:
	DECLARE_MESSAGE_MAP()
public:
	void DrawLine(HWND hWnd, int BeginX, int BeginY, int EndX, int EndY);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	vector<CRect> ItemRect;
	int SelectItem;
	bool m_bCanDrag;
	int ActiveItem;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	bool IsOut;
	bool IsInFilstPart;
	bool IsInSecondPart;
	afx_msg void OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
};

