#include "stdafx.h"
#include "Radio Player.h"
#include "Ctrl_PlayList.h"


IMPLEMENT_DYNAMIC(CCtrl_PlayList, CListCtrl)

CCtrl_PlayList::CCtrl_PlayList()
{
	SelectItem = SELECT_NONE;
	m_bCanDrag = true;
	ActiveItem = SELECT_NONE;
	IsOut = false;
	IsInFilstPart = false;
	IsInSecondPart = false;
}


CCtrl_PlayList::~CCtrl_PlayList()
{
}


BEGIN_MESSAGE_MAP(CCtrl_PlayList, CListCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, &CCtrl_PlayList::OnLvnBegindrag)
END_MESSAGE_MAP()



// CMyListCtrl 消息处理程序




void CCtrl_PlayList::DrawLine(HWND hWnd, int BeginX, int BeginY, int EndX, int EndY)
{
	HDC hdc = ::GetDC(hWnd);
	HPEN Pen = ::CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	HPEN OldPen = (HPEN)SelectObject(hdc, Pen);
	HBRUSH OldBrush = (HBRUSH)::SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));

	MoveToEx(hdc, BeginX, BeginY, NULL);
	LineTo(hdc, EndX, EndY);

	::SelectObject(hdc, OldBrush);
	::SelectObject(hdc, OldPen);
	::DeleteObject(Pen);
	::ReleaseDC(hWnd, hdc);
}


void CCtrl_PlayList::OnLButtonDown(UINT nFlags, CPoint point)
{
	//if (m_bCanDrag)
	//{
	//	::SetCapture(m_hWnd);
	//	if (GetFocus() != this)
	//		SetFocus();

	//	ItemRect.clear();
	//	CRect TmpRect;
	//	SelectItem = SELECT_NONE;
	//	for (int i = 0; i != GetItemCount(); i++)
	//	{
	//		TmpRect.left = LVIR_SELECTBOUNDS;
	//		GetItemRect(i, &TmpRect, LVIR_BOUNDS);
	//		ItemRect.push_back(TmpRect);

	//		// 选中项高亮
	//		if (::PtInRect(&TmpRect, point))
	//		{
	//			SelectItem = i;
	//			SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	//		} 
	//		else
	//		{
	//			SetItemState(i, 0, LVIS_SELECTED);
	//		}
	//	}
	//	return;
	//}
	CListCtrl::OnLButtonDown(nFlags, point);
}


void CCtrl_PlayList::OnMouseMove(UINT nFlags, CPoint point)
{
	// 当左键按下时
	if (nFlags&MK_LBUTTON && SelectItem != SELECT_NONE && m_bCanDrag)
	{
		// 判断鼠标是否在窗体内
		CPoint MousePoint;
		::GetCursorPos(&MousePoint);
		CRect rect;
		::GetWindowRect(m_hWnd, &rect);
		rect.top += 24;
		if (MousePoint.y > rect.bottom - 10)	// 鼠标超过控件底部
		{
			::SendMessage(m_hWnd, WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), 0);
			if (MousePoint.y > rect.bottom){
				IsOut = true;
				ActiveItem = SELECT_NONE;
				::SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_NODRAG)));	// 改变鼠标样式
			}
			return;
		}
		else if (MousePoint.y < rect.top + 7)	// 鼠标在控件顶部上方
		{
			::SendMessage(m_hWnd, WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), 0);
			if (MousePoint.y < rect.top)
			{
				IsOut = true;
				ActiveItem = SELECT_NONE;
				::SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_NODRAG)));	// 改变鼠标样式
			}
			return;
		}

		// 如果在控件外，将重新接收所有Item的Rect
		if (IsOut)
		{
			TRACE("回到控件");
			IsOut = false;
			IsInFilstPart = false;
			IsInSecondPart = false;
			ItemRect.clear();
			CRect TmpRect;
			for (int i = 0; i != GetItemCount(); i++)
			{
				TmpRect.left = LVIR_SELECTBOUNDS;
				GetItemRect(i, &TmpRect, LVIR_BOUNDS);
				ItemRect.push_back(TmpRect);
			}
		}

		// 获取坐标对应的Item，没有对应的Item则为ItemRect.size
		int i;
		for (i = 0; i < (int)ItemRect.size() && !::PtInRect(&ItemRect[i], point); i++);
		TRACE("%d", i);

		// 找到坐标对应的Item
		if (i != ItemRect.size())
		{
			// 改变鼠标样式
			::SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_DRAG)));

			// 移动到最后一项
			if (i == ItemRect.size() - 1)
			{
				// 是否为第一次移入此Item
				if (ActiveItem != i)
				{
					// 如果需要，更新ItemRect
					if (ActiveItem != SELECT_NONE)
						::InvalidateRect(m_hWnd, &ItemRect[ActiveItem], false);

					// 保存当前焦点索引
					ActiveItem = i;
				}

				//在最后一个item的上半部分上移动
				if (point.y < (ItemRect[ActiveItem].top + ItemRect[ActiveItem].bottom) / 2)
				{
					//进入最后一个item的前半部分
					if (IsInFilstPart == false)
					{
						TRACE("进入最后一个item的前半部分");
						IsInFilstPart = true;
						IsInSecondPart = false;
						::InvalidateRect(m_hWnd, &ItemRect[ActiveItem], false);
						::UpdateWindow(m_hWnd);
						DrawLine(m_hWnd, ItemRect[ActiveItem].left + 4, ItemRect[ActiveItem].top + 2,
							ItemRect[ActiveItem].right - 1, ItemRect[ActiveItem].top + 2);
					}
				}

				//在最后一个item的后半部分上移动
				else
				{
					//进入最后一个item的后半部分
					if (IsInSecondPart == false)
					{
						TRACE("进入最后一个item的后半部分");
						IsInSecondPart = true;
						IsInFilstPart = false;
						::InvalidateRect(m_hWnd, &ItemRect[ActiveItem], false);
						::UpdateWindow(m_hWnd);
						DrawLine(m_hWnd, ItemRect[ActiveItem].left + 4, ItemRect[ActiveItem].bottom - 2,
							ItemRect[ActiveItem].right - 1, ItemRect[ActiveItem].bottom - 2);
					}
				}
				return;
			}

			//是否第一次移入此Item
			if (ActiveItem != i)
			{
				IsInFilstPart = false;
				IsInSecondPart = false;

				//假如需要,更新原来的ItemRect
				if (ActiveItem != SELECT_NONE)
					::InvalidateRect(m_hWnd, &ItemRect[ActiveItem], false);

				//保存当前焦点的按钮索引
				ActiveItem = i;

				::UpdateWindow(m_hWnd);
				DrawLine(m_hWnd, ItemRect[ActiveItem].left + 4, ItemRect[ActiveItem].top + 2,
					ItemRect[ActiveItem].right - 1, ItemRect[ActiveItem].top + 2);

				return;
			}
		}

		// 没有找到坐标对应的Item（可能鼠标超出控件或指向控件空白处）
		else
		{
			//假如第一次移出List
			if (ActiveItem != SELECT_NONE)
			{
				//重绘原来的按钮
				::InvalidateRect(m_hWnd, &ItemRect[ActiveItem], false);
				::UpdateWindow(m_hWnd);

				//表示鼠标在List外
				ActiveItem = SELECT_NONE;
			}

			IsInFilstPart = false;
			IsInSecondPart = false;

			//改变鼠标样式
			::SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_NODRAG)));
		}
	}
	CListCtrl::OnMouseMove(nFlags, point);
}


void CCtrl_PlayList::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bCanDrag)
	{
		TRACE("%d", ActiveItem);
		::ReleaseCapture();
		Invalidate();

		if (ActiveItem != SELECT_NONE && ActiveItem != SelectItem && SelectItem != SELECT_NONE)
		{
			::InvalidateRect(m_hWnd, &ItemRect[ActiveItem], false);

			//调换两个Item
			if (IsInSecondPart == true)
				ActiveItem += 1;

			::PostMessage(GetParent()->m_hWnd, WM_FROM_LISTCTRL, SelectItem, ActiveItem);
		}
		else
		{
			//if the mouse is in the ListCtrl window,post the left button up message to the parent window
			CRect rect;
			::GetWindowRect(m_hWnd, &rect);
			ClientToScreen(&point);
			if (::PtInRect(&rect, point))
				::PostMessage(GetParent()->m_hWnd, WM_FROM_LISTCTRL, LBUTTON_UP, 0);
		}

		ItemRect.clear();

		IsInFilstPart = false;
		IsInSecondPart = false;
		SelectItem = SELECT_NONE;
		ActiveItem = SELECT_NONE;
	}
	CListCtrl::OnLButtonUp(nFlags, point);
}


void CCtrl_PlayList::OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (m_bCanDrag)
	{
		CPoint MousePoint;
		::GetCursorPos(&MousePoint);
		::SetCapture(m_hWnd);
		if (GetFocus() != this)
			SetFocus();

		ItemRect.clear();
		CRect TmpRect;
		SelectItem = SELECT_NONE;
		for (int i = 0; i != GetItemCount(); i++)
		{
			TmpRect.left = LVIR_SELECTBOUNDS;
			GetItemRect(i, &TmpRect, LVIR_BOUNDS);
			ItemRect.push_back(TmpRect);
		}
		SelectItem = pNMLV->iItem;
		SetItemState(SelectItem, LVIS_SELECTED, LVIS_SELECTED);
		return;
	}

	*pResult = 0;
}
