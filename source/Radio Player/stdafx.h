
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持


#include <afxsock.h>            // MFC 套接字扩展

#include "Resource.h"

// 皮肤
//#include "SkinPPWTL.h"
//#pragma comment(lib,"SkinPPWTL.lib")
#include <shobjidl.h> 
#include <Strsafe.h>
// GDI库
#include <comdef.h>	// 初始化COM
#include "GdiPlus.h"	// GDI+头文件
using namespace Gdiplus;//使用Gdiplus名字空间


#define WM_PROGRESS_PLAYTIME WM_USER + 1001

#include <vector>
//typedef std::vector<CString> ListArray;
//typedef std::vector<CString> CStringArray;

// 自定义结构体
struct Set_MList{
	CString defStoreType;
	CString defStoreName;
};

struct MediaVolume{
	unsigned int Master = 0;
	unsigned int Player = 0;	// 丢弃
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
	int PlayerStatus = -1;	// -1:未加载路径 0:设备就绪 1:正在播放 2:暂停 3:停止
	int ListPos = 0;	// 按数组顺序，从0开始
	MusicInformed Informed;
	MediaVolume Vol;
};

struct OSbuild
{
	CString Win8_1 = L"Windows 8.1";
	CString Server2012_R2 = L"Windows Server 2012 R2";
	CString Server2012 = L"Windows Server 2012";
	CString Win8 = L"Windows 8";
	CString Win7 = L"Windows 7";
	CString Server2008_R2 = L"Windows Server 2008 R2";
	CString Server2008 = L"Windows Server 2008";
	CString WinVista = L"Windows Vista";
	CString Server2003_R2 = L"Windows Server 2003 R2";
	CString Server2003 = L"Windows Server 2003";
	CString WinXP = L"Windows XP";
	CString Win2000 = L"Windows 2000";
	CString Win98 = L"Windows 98或NT";
};

struct OSInformed
{
	CString VersionTitle;
	unsigned int Major;
	unsigned int Minor;
	bool WorkStation;
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


