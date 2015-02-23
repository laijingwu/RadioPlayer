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



// CMyListCtrl ��Ϣ�������




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

	//		// ѡ�������
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
	// ���������ʱ
	if (nFlags&MK_LBUTTON && SelectItem != SELECT_NONE && m_bCanDrag)
	{
		// �ж�����Ƿ��ڴ�����
		CPoint MousePoint;
		::GetCursorPos(&MousePoint);
		CRect rect;
		::GetWindowRect(m_hWnd, &rect);
		rect.top += 24;
		if (MousePoint.y > rect.bottom - 10)	// ��곬���ؼ��ײ�
		{
			::SendMessage(m_hWnd, WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), 0);
			if (MousePoint.y > rect.bottom){
				IsOut = true;
				ActiveItem = SELECT_NONE;
				::SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_NODRAG)));	// �ı������ʽ
			}
			return;
		}
		else if (MousePoint.y < rect.top + 7)	// ����ڿؼ������Ϸ�
		{
			::SendMessage(m_hWnd, WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), 0);
			if (MousePoint.y < rect.top)
			{
				IsOut = true;
				ActiveItem = SELECT_NONE;
				::SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_NODRAG)));	// �ı������ʽ
			}
			return;
		}

		// ����ڿؼ��⣬�����½�������Item��Rect
		if (IsOut)
		{
			TRACE("�ص��ؼ�");
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

		// ��ȡ�����Ӧ��Item��û�ж�Ӧ��Item��ΪItemRect.size
		int i;
		for (i = 0; i < (int)ItemRect.size() && !::PtInRect(&ItemRect[i], point); i++);
		TRACE("%d", i);

		// �ҵ������Ӧ��Item
		if (i != ItemRect.size())
		{
			// �ı������ʽ
			::SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_DRAG)));

			// �ƶ������һ��
			if (i == ItemRect.size() - 1)
			{
				// �Ƿ�Ϊ��һ�������Item
				if (ActiveItem != i)
				{
					// �����Ҫ������ItemRect
					if (ActiveItem != SELECT_NONE)
						::InvalidateRect(m_hWnd, &ItemRect[ActiveItem], false);

					// ���浱ǰ��������
					ActiveItem = i;
				}

				//�����һ��item���ϰ벿�����ƶ�
				if (point.y < (ItemRect[ActiveItem].top + ItemRect[ActiveItem].bottom) / 2)
				{
					//�������һ��item��ǰ�벿��
					if (IsInFilstPart == false)
					{
						TRACE("�������һ��item��ǰ�벿��");
						IsInFilstPart = true;
						IsInSecondPart = false;
						::InvalidateRect(m_hWnd, &ItemRect[ActiveItem], false);
						::UpdateWindow(m_hWnd);
						DrawLine(m_hWnd, ItemRect[ActiveItem].left + 4, ItemRect[ActiveItem].top + 2,
							ItemRect[ActiveItem].right - 1, ItemRect[ActiveItem].top + 2);
					}
				}

				//�����һ��item�ĺ�벿�����ƶ�
				else
				{
					//�������һ��item�ĺ�벿��
					if (IsInSecondPart == false)
					{
						TRACE("�������һ��item�ĺ�벿��");
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

			//�Ƿ��һ�������Item
			if (ActiveItem != i)
			{
				IsInFilstPart = false;
				IsInSecondPart = false;

				//������Ҫ,����ԭ����ItemRect
				if (ActiveItem != SELECT_NONE)
					::InvalidateRect(m_hWnd, &ItemRect[ActiveItem], false);

				//���浱ǰ����İ�ť����
				ActiveItem = i;

				::UpdateWindow(m_hWnd);
				DrawLine(m_hWnd, ItemRect[ActiveItem].left + 4, ItemRect[ActiveItem].top + 2,
					ItemRect[ActiveItem].right - 1, ItemRect[ActiveItem].top + 2);

				return;
			}
		}

		// û���ҵ������Ӧ��Item��������곬���ؼ���ָ��ؼ��հ״���
		else
		{
			//�����һ���Ƴ�List
			if (ActiveItem != SELECT_NONE)
			{
				//�ػ�ԭ���İ�ť
				::InvalidateRect(m_hWnd, &ItemRect[ActiveItem], false);
				::UpdateWindow(m_hWnd);

				//��ʾ�����List��
				ActiveItem = SELECT_NONE;
			}

			IsInFilstPart = false;
			IsInSecondPart = false;

			//�ı������ʽ
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

			//��������Item
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
