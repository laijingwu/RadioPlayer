
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��


#include <afxsock.h>            // MFC �׽�����չ

#include "Resource.h"

// Ƥ��
//#include "SkinPPWTL.h"
//#pragma comment(lib,"SkinPPWTL.lib")
#include <shobjidl.h> 
#include <Strsafe.h>
// GDI��
#include <comdef.h>	// ��ʼ��COM
#include "GdiPlus.h"	// GDI+ͷ�ļ�
using namespace Gdiplus;//ʹ��Gdiplus���ֿռ�


#define WM_PROGRESS_PLAYTIME WM_USER + 1001

#include <vector>
//typedef std::vector<CString> ListArray;
//typedef std::vector<CString> CStringArray;

// �Զ���ṹ��
struct Set_MList{
	CString defStoreType;
	CString defStoreName;
};

struct MediaVolume{
	unsigned int Master = 0;
	unsigned int Player = 0;	// ����
	unsigned int Real = 0;
};

struct Time{
	unsigned int Sec = 0;
	unsigned int Min = 0;
	unsigned int Hour = 0;
	unsigned int Day = 0;
};

struct Setting{
	Set_MList MusicList;
};

struct MusicInformed
{
	CString MusicTitle;
	CString MusicLength;
	CString MusicAuthor;
	CString MusicPath;
};

typedef std::vector<MusicInformed> MusicListArr;

struct MediaStatus{
	int PlayerStatus = -1;	// -1:δ����·�� 0:�豸���� 1:���ڲ��� 2:��ͣ 3:ֹͣ
	int ListPos = 0;	// ������˳�򣬴�0��ʼ
	MusicInformed Informed;
	MediaVolume Vol;
};






#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


