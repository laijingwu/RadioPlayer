
// Radio Player.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRadioPlayerApp:
// �йش����ʵ�֣������ Radio Player.cpp
//

class CRadioPlayerApp : public CWinApp
{
public:
	CRadioPlayerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRadioPlayerApp theApp;