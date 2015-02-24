
// Radio PlayerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Radio Player.h"
#include "Radio PlayerDlg.h"
#include "afxdialogex.h"

#pragma warning(disable: 4996)    

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WM_TASKBARBUTTONCREATED = ::RegisterWindowMessage(L"TaskbarButtonCreated");

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSite();
};


// 添加文件夹线程
DWORD __stdcall Thread_AddFolderToList(LPVOID lParam);

DWORD __stdcall Thread_AddFolderToList(LPVOID lParam){
	CRadioPlayerDlg *dlg=(CRadioPlayerDlg*)lParam;
	dlg->AddFolderToList(dlg->tmp_strAddFolderPath);
	return 0;
}

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SITE, &CAboutDlg::OnBnClickedBtnSite)
END_MESSAGE_MAP()


// CRadioPlayerDlg 对话框




CRadioPlayerDlg::CRadioPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRadioPlayerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRadioPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MUSICLIST, ctrl_MusicList);
	DDX_Control(pDX, IDC_SLIDER_VOLUME, ctrl_SliderVol);
	DDX_Control(pDX, IDC_PROGRESS_PLAYTIME, prog_PlayTime);
}

BEGIN_MESSAGE_MAP(CRadioPlayerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_ABOUT, &CRadioPlayerDlg::OnBnClickedBtnAbout)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CRadioPlayerDlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_ADDFILE, &CRadioPlayerDlg::OnBnClickedBtnAddfile)
	ON_BN_CLICKED(IDC_BTN_ADDFOLDER, &CRadioPlayerDlg::OnBnClickedBtnAddfolder)
	ON_NOTIFY(NM_DBLCLK, IDC_MUSICLIST, &CRadioPlayerDlg::OnNMDblclkMusiclist)
	ON_BN_CLICKED(IDC_BTN_PLAY, &CRadioPlayerDlg::OnBnClickedBtnPlay)
	ON_BN_CLICKED(IDC_BTN_STOP, &CRadioPlayerDlg::OnBnClickedBtnStop)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_QUIET, &CRadioPlayerDlg::OnBnClickedBtnQuiet)
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()	// 设置字体
	ON_MESSAGE(WM_PROGRESS_PLAYTIME,Prog_PlayTimeProc)
	ON_BN_CLICKED(IDC_BTN_LOADLIST, &CRadioPlayerDlg::OnBnClickedBtnLoadlist)
	ON_BN_CLICKED(IDC_BTN_STORELIST, &CRadioPlayerDlg::OnBnClickedBtnStorelist)
	ON_BN_CLICKED(IDC_BTN_LASTRADIO, &CRadioPlayerDlg::OnBnClickedBtnLastradio)
	ON_BN_CLICKED(IDC_BTN_NEXTRADIO, &CRadioPlayerDlg::OnBnClickedBtnNextradio)
	ON_NOTIFY(NM_RCLICK, IDC_MUSICLIST, &CRadioPlayerDlg::OnNMRClickMusiclist)
	ON_COMMAND(ID_MENU_DELETE, &CRadioPlayerDlg::OnMenuDelete)
	ON_MESSAGE(WM_FROM_LISTCTRL, &CRadioPlayerDlg::MessageFromListCtrl) 	/*将消息和消息响应函数联系起来*/
	ON_REGISTERED_MESSAGE(WM_TASKBARBUTTONCREATED, OnCreateTaskBar)
END_MESSAGE_MAP()


// CRadioPlayerDlg 消息处理程序

BOOL CRadioPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 加载皮肤
	//skinppLoadSkin("skin/spring.ssk");
	OSVersion = GetOSVersion();
	// 音乐列表控件设置
	ctrl_MusicList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);	// 设置点选整行、显示线框
	LVCOLUMN lvColumn;
	lvColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 40;
	lvColumn.pszText = L"ID";
	ctrl_MusicList.InsertColumn(0,&lvColumn);
	lvColumn.cx = 320;
	lvColumn.pszText = L"Radio Name";
	ctrl_MusicList.InsertColumn(1,&lvColumn);
	lvColumn.cx = 60;
	lvColumn.pszText = L"Length";
	ctrl_MusicList.InsertColumn(2,&lvColumn);
	lvColumn.cx = 90;
	lvColumn.pszText = L"Author";
	ctrl_MusicList.InsertColumn(3,&lvColumn);

	// 音量控件设置
	AudioPlayer.SetVolume(80);	// 初始音量
	CurrentPlayStatus.Vol.Master = 80;
	CurrentPlayStatus.Vol.Real = 80;
	//CurrentPlayStatus.Vol.Player = 80;
	SetDlgItemText(IDC_STATIC_VOLUME,L"80%");
	ctrl_SliderVol.SetRange(0,100,true);
	ctrl_SliderVol.SetPos(80);

	// 额外初始化代码
	TitleFont.CreatePointFont(150,L"微软雅黑");	// 设置主标题字体
	SetDlgItemText(IDC_STATIC_RUNTIME, NULL);
	SetTimer(1,1000,NULL);
	prog_PlayTime.SetRange(0, 10000);
	PlayerSetting.MusicList.defStoreName = L"Default";	// 设置播放列表文件名称和类型
	PlayerSetting.MusicList.defStoreType = L".mlist";

	// 设置电台默认LOGO
	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRsrc = FindResource(hInst,MAKEINTRESOURCE(IDB_PNG1),TEXT("png")); // type

	// 读取资源到内存
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	PrintRadioLogo(lpRsrc, len);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRadioPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRadioPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		//CImage RadioLogo;
		//HRESULT hr = RadioLogo.Load(L"F:\\2.png");
		//for(int i = 0; i < imgRadioLogo.GetWidth(); i++){
		//	for(int j = 0; j < imgRadioLogo.GetHeight(); j++){
		//		unsigned char* pucColor = reinterpret_cast<unsigned char *>(imgRadioLogo.GetPixelAddress(i,j));
		//		pucColor[0] = pucColor[0] * pucColor[3] / 255;
		//		pucColor[1] = pucColor[1] * pucColor[3] / 255;
		//		pucColor[2] = pucColor[2] * pucColor[3] / 255;
		//	}
		//}
		//CRect rect;
		////int cx = imgRadioLogo.GetWidth();
		////int cy = imgRadioLogo.GetHeight();
		//CWnd *pWnd = GetDlgItem(IDC_RADIOLOGO);
		//CRect winrect;
		//pWnd->GetWindowRect(winrect);
		//ScreenToClient(&winrect);
		/////*pWnd->MoveWindow(rect.left,rect.top,100,100,TRUE);*/
		//pWnd->GetClientRect(&rect);
		//CDC *pDC = pWnd->GetDC();
		////pDC->SetStretchBltMode(HALFTONE);
		//int ccx = winrect.Width();
		//int ccy = cy*ccx/cx;
		//if(ccy > winrect.Height()){
		//	ccy = winrect.Height();
		//	ccx = cx*ccy/cy;
		//}
		//imgRadioLogo.StretchBlt(pDC->m_hDC,rect.left,rect.top,ccx,ccy,SRCCOPY);
		//imgRadioLogo.Draw(pDC->m_hDC,rect.left,rect.top,ccx,ccy);
		//ReleaseDC(pDC);

		TID3InfoExW m_id3_info;
		m_id3_info = AudioPlayer.LoadID3Ex(this->m_hWnd);
		if (m_id3_info.Picture.PicturePresent == 1)
		{
			PrintRadioLogo((BYTE *)m_id3_info.Picture.PictureData, m_id3_info.Picture.PictureDataSize);
		}
		else
		{
			HINSTANCE hInst = AfxGetResourceHandle();
			HRSRC hRsrc = FindResource(hInst, MAKEINTRESOURCE(IDB_PNG1), TEXT("png")); // type
			DWORD len = SizeofResource(hInst, hRsrc);
			BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
			PrintRadioLogo(lpRsrc, len);
		}
			

		//HINSTANCE hInst = AfxGetResourceHandle();
		//HRSRC hRsrc = FindResource(hInst,MAKEINTRESOURCE(IDB_PNG1),TEXT("png")); // type

		// 读取资源到内存
		//unsigned int leng = m_id3_info.Picture.PictureDataSize;
		////DWORD len = SizeofResource(hInst, hRsrc);
		//BYTE* lpRsrc = (BYTE*)m_id3_info.Picture.PictureData;//(BYTE*)LoadResource(hInst, hRsrc);

		//// 分配的全局内存以创建流
		//HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, leng);
		//BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
		//memcpy(pmem,lpRsrc,leng);
		//IStream* pstm;
		//CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);

		//// 从流读取
		//Image* pLogoLoad = Image::FromStream(pstm);

		//// 释放
		//GlobalUnlock(m_hMem);
		//pstm->Release();
		//FreeResource(lpRsrc);

		//Graphics imgRadioLogo(*pDC);	// 创建GDI+
		////Image* pLogoLoad = Image::FromFile(L"F:\\1.png");	// 读取文件
		//int cx = pLogoLoad->GetWidth();
		//int cy = pLogoLoad->GetHeight();
		//int ccx = IDCRect.Width();
		//int ccy = cy*ccx/cx;
		//if(ccy > IDCRect.Height()){
		//	ccy = IDCRect.Height();
		//	ccx = cx*ccy/cy;
		//}
		//imgRadioLogo.SetInterpolationMode(InterpolationModeHighQualityBicubic);	// 设置模式
		//InterpolationModeInvalid          = QualityModeInvalid,    //插值无效  
		//InterpolationModeDefault          = QualityModeDefault,    //指定默认模式  
		//InterpolationModeLowQuality       = QualityModeLow,        //指定低质量插值法  
		//InterpolationModeHighQuality      = QualityModeHigh,       //指定高质量插值法  
		//InterpolationModeBilinear,                                 //指定双线性插值法  
		//InterpolationModeBicubic,                                  //指定双三次插值法  
		//InterpolationModeNearestNeighbor,                          //指定最临近插值法  
		//InterpolationModeHighQualityBilinear,                      //指定高质量双线性插值法  
		//InterpolationModeHighQualityBicubic                        //指定高质量双三次插值法  
		//
		//imgRadioLogo.DrawImage(pLogoLoad,Rect(LogoRect.left,LogoRect.top,ccx,ccy));	// 显示
		//ReleaseDC(pDC);
	}
}


//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRadioPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// 重绘函数
HBRUSH CRadioPlayerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TITLE){	// 设置字体
		pDC->SelectObject(&TitleFont);
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


// 关于按钮
void CRadioPlayerDlg::OnBnClickedBtnAbout()
{
	//PlaySound(L"SystemStart",NULL,SND_ALIAS|SND_ASYNC);
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


// 退出按钮
void CRadioPlayerDlg::OnBnClickedBtnExit()
{
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}


// 添加文件按钮
void CRadioPlayerDlg::OnBnClickedBtnAddfile()
{
	const int FILE_NUMBER = 30;	// 最多添加文件个数
	// 文件过滤器
	LPCTSTR lpFilter = L"Audio Files(*.mp3;*.mp2;*.mp1;*.ogg;*.flac;*.ac3;*.aac;*.oga;*.wav;*.pcm)|*.mp3;*.mp2;*.mp1;*.ogg;*.flac;*.ac3;*.aac;*.oga;*.wav;*.pcm|Mp3 Files(*.mp3)|*.mp3|Mp2 Files(*.mp2)|*.mp2|Mp1 Files(*.mp1)|*.mp1|OGG Files(*.ogg)|*.ogg|FLAC Files(*.flac)|*.flac|AC-3 Files(*.ac3)|*.ac3|AAC Files(*.aac)|*.aac|OGA Files(*.oga)|*.oga|Wave Files(*.wav)|*.wav|PCM Files(*.pcm)|*.pcm|All Files(*.*)|*.*|";
	//CFileDialog fileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,lpFilter,this);	// 设置单选
	CFileDialog fileDlg(TRUE,NULL,NULL,OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT,lpFilter,this);	// 设置多选
	fileDlg.m_ofn.lpstrTitle = L"Open";	// 打开文件对话框标题
	fileDlg.m_ofn.lpstrFile = new wchar_t[_MAX_PATH * FILE_NUMBER];	// 设置缓冲区
	memset(fileDlg.m_ofn.lpstrFile,0,_MAX_PATH * FILE_NUMBER);	// 初始化缓冲区
	fileDlg.m_ofn.nMaxFile = _MAX_PATH * FILE_NUMBER;
	if (fileDlg.DoModal() == IDCANCEL){
		return;
	}

	POSITION posFile = fileDlg.GetStartPosition();
	while (posFile)
	{
		if (AddToList(fileDlg.GetNextPathName(posFile)) == false)
			continue;
	}

	delete[] fileDlg.m_ofn.lpstrFile;	// 释放缓冲区
	return;
}


// 添加文件夹按钮
void CRadioPlayerDlg::OnBnClickedBtnAddfolder()
{
	BROWSEINFO bi;
	wchar_t chBuf[_MAX_PATH];
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = (LPWSTR)&chBuf;
	bi.lpszTitle = L"Folder:";
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.iImage = 0;
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);
	if(pIDList){
		SHGetPathFromIDList(pIDList,(LPWSTR)&chBuf);
		tmp_strAddFolderPath = chBuf;
		HANDLE hThread = CreateThread(NULL,0,Thread_AddFolderToList,this,0,NULL);
	}
	return;
}


// 添加文件夹线程处理函数
void CRadioPlayerDlg::AddFolderToList(CString strPath)
{
	CFileFind finder;
	CString m_strFilePath,m_strFileName,m_strFileTitle;
	BOOL bWorking = finder.FindFile(strPath + "\\*.*");
	while(bWorking){
		bWorking = finder.FindNextFile();
		m_strFileName = finder.GetFileName();
		//m_strFileTitle = finder.GetFileTitle();

		if(m_strFileName.Right(3) == "mp3" || m_strFileName.Right(3) == "mp2" || m_strFileName.Right(3) == "mp1" || m_strFileName.Right(3) == "ogg" || m_strFileName.Right(4) == "flac" || m_strFileName.Right(3) == "ac3" || m_strFileName.Right(3) == "aac" || m_strFileName.Right(3) == "oga" || m_strFileName.Right(3) == "wav" || m_strFileName.Right(3) == "pcm"){
			m_strFilePath = strPath + "\\" + m_strFileName;
			
			if (AddToList(m_strFilePath) == false)
				continue;

		}
		if(finder.IsDirectory() && !finder.IsDots())
			AddFolderToList(finder.GetFilePath());
	}
	return;
}


// MusicList双击行
void CRadioPlayerDlg::OnNMDblclkMusiclist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nIndex = ctrl_MusicList.GetSelectionMark();
	if(nIndex != -1){
		CString strBufID = ctrl_MusicList.GetItemText(nIndex,0);
		int nPos = _ttoi(strBufID);
		AudioPlayer.Stop(true);
		GetDlgItem(IDC_BTN_PLAY)->SetWindowTextW(L"Play");
		CurrentPlayStatus.PlayerStatus = 0;
		CurrentPlayStatus.ListPos = nPos - 1;
		CurrentPlayStatus.Informed = RadioList.at(nPos - 1);
		Sleep(300);
		OnBnClickedBtnPlay();
	}
	*pResult = 0;
}


// 播放按钮
void CRadioPlayerDlg::OnBnClickedBtnPlay()
{
	if(ctrl_MusicList.GetItemCount() == 0){	// 列表无项目
		OnBnClickedBtnAddfile();
		if(ctrl_MusicList.GetItemCount() != 0){
			CurrentPlayStatus.PlayerStatus = 0;
			CurrentPlayStatus.ListPos = 0;
			CurrentPlayStatus.Informed = RadioList.at(0);
			OnBnClickedBtnPlay();
			return;
		}
	}
	else if (CurrentPlayStatus.PlayerStatus == -1){	// 未加载路径
		CurrentPlayStatus.PlayerStatus = 0;
		CurrentPlayStatus.ListPos = 0;
		CurrentPlayStatus.Informed = RadioList.at(0);
		OnBnClickedBtnPlay();
	}
	else if (CurrentPlayStatus.PlayerStatus == 0 || CurrentPlayStatus.PlayerStatus == 3){	// 设备就绪、停止
		if (AudioPlayer.Play(this->m_hWnd, CurrentPlayStatus.Informed.MusicPath) == false)
			return;

		// Logo显示
		TID3InfoExW m_id3_info;
		m_id3_info = AudioPlayer.LoadID3Ex(this->m_hWnd);
		if (m_id3_info.Picture.PicturePresent == 1)
		{
			PrintRadioLogo((BYTE *)m_id3_info.Picture.PictureData, m_id3_info.Picture.PictureDataSize);
		}
		else
		{
			HINSTANCE hInst = AfxGetResourceHandle();
			HRSRC hRsrc = FindResource(hInst, MAKEINTRESOURCE(IDB_PNG1), TEXT("png")); // type
			DWORD len = SizeofResource(hInst, hRsrc);
			BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
			PrintRadioLogo(lpRsrc, len);
		}

		CurrentPlayStatus.PlayerStatus = 1;
		GetDlgItem(IDC_BTN_PLAY)->SetWindowTextW(L"Pause");
		if (OSVersion.Major >= 6 && OSVersion.Minor >= 1)
			m_pProgressTaskBar->SetPlayState(CurrentPlayStatus.Informed.MusicTitle);
		SetDlgItemText(IDC_STATIC_TITLE, CurrentPlayStatus.Informed.MusicTitle);
		SetTimer(2,100,NULL);
		SetTimer(3,100,NULL);
	}
	else if (CurrentPlayStatus.PlayerStatus == 1){	// 正在播放
		AudioPlayer.Pause();
		CurrentPlayStatus.PlayerStatus = 2;
		GetDlgItem(IDC_BTN_PLAY)->SetWindowTextW(L"Play");
		if (OSVersion.Major >= 6 && OSVersion.Minor >= 1)
			m_pProgressTaskBar->SetPauseState();
	}
	else if (CurrentPlayStatus.PlayerStatus == 2){	// 暂停
		AudioPlayer.Resume();
		CurrentPlayStatus.PlayerStatus = 1;
		GetDlgItem(IDC_BTN_PLAY)->SetWindowTextW(L"Pause");
		if (OSVersion.Major >= 6 && OSVersion.Minor >= 1)
			m_pProgressTaskBar->SetPlayState(CurrentPlayStatus.Informed.MusicTitle);
	}
	return;
}


// 停止按钮
void CRadioPlayerDlg::OnBnClickedBtnStop()
{
	AudioPlayer.Stop();
	CurrentPlayStatus.PlayerStatus = 3;
	GetDlgItem(IDC_BTN_PLAY)->SetWindowTextW(L"Play");
	if (OSVersion.Major >= 6 && OSVersion.Minor >= 1)
		m_pProgressTaskBar->SetStopState();
	return;
}


// 关闭会话前
void CRadioPlayerDlg::OnClose()
{
	if (CurrentPlayStatus.PlayerStatus == 1){
		if(MessageBox(L"You are listening to the radio, are you sure you want to quit?",L"Tips",MB_YESNO|MB_ICONINFORMATION) == IDNO){
			return;
		}
	}
	CDialogEx::OnClose();
}


// 窗体销毁前
void CRadioPlayerDlg::OnDestroy()
{
	KillTimer(1);
	KillTimer(2);
	KillTimer(3);
	AudioPlayer.Stop(true);
	AudioPlayer.player->Release();
	CDialogEx::OnDestroy();
}


// 静音按钮
void CRadioPlayerDlg::OnBnClickedBtnQuiet()
{
	CString tmp_strQUIET;
	GetDlgItemText(IDC_BTN_QUIET,tmp_strQUIET);// && tmp_strQUIET.Compare(L"Silent") == 0   CurrentPlayStatus.Vol.Master == 100
	if(tmp_strQUIET.Compare(L"Silent") == 0){
		//AudioPlayer.player->SetMasterVolume(0,0);
		//CurrentPlayStatus.Vol.Master = 0;
		AudioPlayer.SetVolume(0);
		CurrentPlayStatus.Vol.Real = 0;
		ctrl_SliderVol.SetPos(0);
		GetDlgItem(IDC_BTN_QUIET)->SetWindowTextW(L"Silent √");
		SetDlgItemText(IDC_STATIC_VOLUME,L"0%");
	}else{
		AudioPlayer.SetVolume(CurrentPlayStatus.Vol.Master);
		CurrentPlayStatus.Vol.Real = CurrentPlayStatus.Vol.Master;
		//AudioPlayer.player->SetMasterVolume(100,100);
		//CurrentPlayStatus.Vol.Master = 100;
		CString tmp_strVol;
		tmp_strVol.Format(L"%d%%", CurrentPlayStatus.Vol.Real);
		ctrl_SliderVol.SetPos(CurrentPlayStatus.Vol.Real);
		GetDlgItem(IDC_BTN_QUIET)->SetWindowTextW(L"Silent");
		SetDlgItemText(IDC_STATIC_VOLUME,tmp_strVol);
	}
	return;
}


// Slider滑块调节音量
void CRadioPlayerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSliderCtrl *pSlider = (CSliderCtrl*)pScrollBar;
	CWnd *pSliderVolume = this->GetDlgItem(IDC_SLIDER_VOLUME);
	if(pSlider == pSliderVolume){
		int nVolumn = pSlider->GetPos();
		CString tmp_strQUIET;
		GetDlgItemText(IDC_BTN_QUIET, tmp_strQUIET);
		AudioPlayer.SetVolume(nVolumn);
		CurrentPlayStatus.Vol.Master = nVolumn;
		CurrentPlayStatus.Vol.Real = nVolumn;
		CString tmp_strVol;
		tmp_strVol.Format(L"%d%%", nVolumn);
		SetDlgItemText(IDC_STATIC_VOLUME, tmp_strVol);
		if (nVolumn != 0 && tmp_strQUIET.Compare(L"Silent") != 0)
			GetDlgItem(IDC_BTN_QUIET)->SetWindowTextW(L"Silent");
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


// 定时器
void CRadioPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	CString tmp_strRunTime,tmp_strMediaTime;
	int nMListCount;
	COleDateTime NowTime;
	switch(nIDEvent){
	case 1:
		//RunTime 运行时间
		++RunTime.Sec;
		if (RunTime.Sec == 60){
			++RunTime.Min;
			RunTime.Sec = 0;
		}
		if (RunTime.Min == 60){
			++RunTime.Hour;
			RunTime.Min = 0;
		}
		if (RunTime.Hour == 24){
			++RunTime.Day;
			RunTime.Hour = 0;
		}
		// 获取现在时间
		NowTime = COleDateTime::GetCurrentTime();	
		tmp_strRunTime.Format(L"RunTime: %u day %02u:%02u:%02u    Time: %04u-%02u-%02u %02u:%02u:%02u", RunTime.Day, RunTime.Hour, RunTime.Min, RunTime.Sec, NowTime.GetYear(), NowTime.GetMonth(), NowTime.GetDay(), NowTime.GetHour(), NowTime.GetMinute(), NowTime.GetSecond());
		SetDlgItemText(IDC_STATIC_RUNTIME,tmp_strRunTime);
		break;
	case 2:
		// 时间显示更新
		tmp_strMediaTime = AudioPlayer.GetMediaTimeStr() + "/" + CurrentPlayStatus.Informed.MusicLength;
		SetDlgItemText(IDC_STATIC_MEDIATIME,tmp_strMediaTime);
		// 列表播放
		if (AudioPlayer.GetMediaTime() == 0 && CurrentPlayStatus.PlayerStatus == 1){
			Sleep(1);	// 判断是否为停止，而非开始
			if(AudioPlayer.GetMediaTime() == 0){
				OnBnClickedBtnStop();
				AudioPlayer.Stop(true);
				// 单曲循环
				CurrentPlayStatus.PlayerStatus = 0;
				nMListCount = ctrl_MusicList.GetItemCount();
				OnBnClickedBtnPlay();
				// 列表顺序播放
				//if(CurrentPlayStatus.ListPos < nMListCount-1){
				//	CurrentPlayStatus.ListPos++;
				//CurrentPlayStatus.Informed = RadioList.at(CurrentPlayStatus.ListPos);
				//	OnBnClickedBtnPlay();
				//}
				// 列表循环播放
				//else{
				//	CurrentPlayStatus.ListPos = 0;
				//CurrentPlayStatus.Informed = RadioList.at(CurrentPlayStatus.ListPos);
				//	OnBnClickedBtnPlay();
				//}
			}
		}
		break;
	case 3:
		// 进度条走动
		float fTime = (float)AudioPlayer.GetMediaTime();
		float fLength = (float)AudioPlayer.GetMediaLength();
		int nPosProg = (int)((fTime / fLength) * 10000);
		prog_PlayTime.SetPos(nPosProg);
		if (OSVersion.Major >= 6 && OSVersion.Minor >= 1)
			m_pProgressTaskBar->SetProgPos(nPosProg, 10000);
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


// 进度条
LRESULT CRadioPlayerDlg::Prog_PlayTimeProc(WPARAM wParam, LPARAM lParam)
{
	int nCurPos = (int)(prog_PlayTime.m_fClickPercent * AudioPlayer.GetMediaLength());
	Sleep(1);
	TStreamTime pTime;
	pTime.ms = nCurPos;
	AudioPlayer.player->Seek(tfMillisecond, &pTime, smFromBeginning);
	return 0;
}


// 读取播放列表按钮
void CRadioPlayerDlg::OnBnClickedBtnLoadlist()
{
	// 文件过滤器
	CString strFilter;
	strFilter = L"PlayList Files(*" + PlayerSetting.MusicList.defStoreType + L")|*" + PlayerSetting.MusicList.defStoreType + L"|";
	CFileDialog fileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,(LPCTSTR)strFilter,this);	// 设置单选
	fileDlg.m_ofn.lpstrTitle = L"Open";
	if(fileDlg.DoModal() == IDOK){
		AudioPlayer.Stop(true);
		ctrl_MusicList.DeleteAllItems();
		RadioList.clear();
		Common.LoadMusicList(fileDlg.GetPathName());
		CurrentPlayStatus.PlayerStatus = 0;
		CurrentPlayStatus.ListPos = 0;
		CurrentPlayStatus.Informed = RadioList.at(0);
		return;
	}
}


// 保存播放列表按钮
void CRadioPlayerDlg::OnBnClickedBtnStorelist()
{
	if(ctrl_MusicList.GetItemCount() != 0){
		// 文件过滤器
		CString strFilter;
		strFilter = L"PlayList Files(*" + PlayerSetting.MusicList.defStoreType + L")|*" + PlayerSetting.MusicList.defStoreType + L"|";
		CFileDialog fileDlg(FALSE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,(LPCTSTR)strFilter,this);	// 设置单选
		fileDlg.m_ofn.lpstrTitle = L"Save";
		if(fileDlg.DoModal() == IDOK){
			CString SaveFile;
			SaveFile = fileDlg.GetPathName();
			if(SaveFile.Right(6) != PlayerSetting.MusicList.defStoreType){
				SaveFile += PlayerSetting.MusicList.defStoreType;
			}
			Common.StoreMusicList(SaveFile);
			MessageBox(L"Have been saved successfully!",L"Store List",MB_OK|MB_ICONINFORMATION|MB_ICONASTERISK);
		}
	}else{
		MessageBox(L"Please ensure that at least one radio in your playlist.",L"Warning",MB_OK|MB_ICONWARNING|MB_ICONEXCLAMATION|MB_APPLMODAL);
	}
	return;
}


// 换台
void CRadioPlayerDlg::ChangeRadio(CString strPath)
{
	// 播放换台音效  SND_ASYNC 异步方式播放声音(开始返回);SND_SYNC 同步播放声音(结束返回)
	PlaySound(MAKEINTRESOURCE(IDR_WAVE1),AfxGetResourceHandle(),SND_ASYNC|SND_RESOURCE|SND_NODEFAULT);
	Sleep(700);	// 短暂停顿，用于播放换台音效
	AudioPlayer.Load(this->m_hWnd,strPath);
	unsigned int nLength = AudioPlayer.GetMediaLength();	// 单位 毫秒
	//unsigned int nRunTime = (((RunTime.day*24+RunTime.hour)*60+RunTime.min)*60+RunTime.sec)*1000;	// 转换成毫秒
	COleDateTime NowTime = COleDateTime::GetCurrentTime();	// 获取现在时间
	unsigned int nNowTime = (((NowTime.GetDay()*24+NowTime.GetHour())*60+NowTime.GetMinute())*60+NowTime.GetSecond())*1000;
	unsigned int nCurPos = nNowTime % nLength;
	Sleep(1);
	TStreamTime pTime;
	pTime.ms = nCurPos;
	AudioPlayer.player->Seek(tfMillisecond,&pTime,smFromBeginning);
	OnBnClickedBtnPlay();
	return;
}


// 上一台按钮
void CRadioPlayerDlg::OnBnClickedBtnLastradio()
{
	int nMListCount = ctrl_MusicList.GetItemCount();
	if(nMListCount != 0){
		if (CurrentPlayStatus.PlayerStatus != -1){
			AudioPlayer.Stop(true);
			GetDlgItem(IDC_BTN_PLAY)->SetWindowTextW(L"Play");
			if(CurrentPlayStatus.ListPos > 0){
				CurrentPlayStatus.ListPos--;
			}else{
				CurrentPlayStatus.ListPos = nMListCount - 1;
			}
		}else{
			CurrentPlayStatus.ListPos = 0;
		}
		CurrentPlayStatus.PlayerStatus = 0;
		CurrentPlayStatus.Informed = RadioList.at(CurrentPlayStatus.ListPos);
		ChangeRadio(CurrentPlayStatus.Informed.MusicPath);
	}
	return;
}


// 下一台按钮
void CRadioPlayerDlg::OnBnClickedBtnNextradio()
{
	int nMListCount = ctrl_MusicList.GetItemCount();
	if(nMListCount != 0){
		if (CurrentPlayStatus.PlayerStatus != -1){
			AudioPlayer.Stop(true);
			GetDlgItem(IDC_BTN_PLAY)->SetWindowTextW(L"Play");
			if(CurrentPlayStatus.ListPos < nMListCount - 1){
				CurrentPlayStatus.ListPos++;
			}else{
				CurrentPlayStatus.ListPos = 0;
			}
		}else{
			CurrentPlayStatus.ListPos = 0;
		}
		CurrentPlayStatus.PlayerStatus = 0;
		CurrentPlayStatus.Informed = RadioList.at(CurrentPlayStatus.ListPos);
		ChangeRadio(CurrentPlayStatus.Informed.MusicPath);
	}
	return;
}


// 右击播放列表
void CRadioPlayerDlg::OnNMRClickMusiclist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if(pNMItemActivate->iItem != -1){
		CPoint point;
		GetCursorPos(&point);
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU_LISTRCLICK));
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup!=NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
	}
	*pResult = 0;
}


// 播放列表右键菜单删除按钮
void CRadioPlayerDlg::OnMenuDelete()
{
	int nOldPlayStatus = CurrentPlayStatus.PlayerStatus;	// 函数未修改前的播放状态
	std::vector<int> vecDelIndex;	// 得到需要删除行的序号
	int m_nDeleteNum = 0;	// 用于删除算法
	int nSelectCount = ctrl_MusicList.GetSelectedCount();	// 得到被选择的行数
	POSITION pos = ctrl_MusicList.GetFirstSelectedItemPosition();	//被选择的第一行
	while(pos){
		vecDelIndex.push_back(ctrl_MusicList.GetNextSelectedItem(pos));	//下一行
	}
	for(int i = 0; i < nSelectCount; ++i){
		//当正在播放、暂停、停止时停止播放，并关闭文件流
		if (CurrentPlayStatus.ListPos == vecDelIndex.at(i) && CurrentPlayStatus.PlayerStatus != -1){
			OnBnClickedBtnStop();
			AudioPlayer.Stop(true);
			CurrentPlayStatus.PlayerStatus = -1;
		}
		//删除行、更新数组
		ctrl_MusicList.DeleteItem(vecDelIndex.at(i) - m_nDeleteNum);
		RadioList.erase(RadioList.begin() + (vecDelIndex.at(i) - m_nDeleteNum));
		++m_nDeleteNum;
	}
	// 列表ID更新
	RefMusicList();
	int nMListCount = ctrl_MusicList.GetItemCount();
	// 列表Pos移动 播放
	if(nMListCount <= 0){	// 列表无项目
		SetDlgItemText(IDC_STATIC_TITLE,L"Radio Player");
		CurrentPlayStatus.PlayerStatus = -1;	// 未载入路径状态
		CurrentPlayStatus.ListPos = 0;
		CurrentPlayStatus.Informed = { NULL, L"00:00", NULL, NULL };
	}else if(nMListCount > 0 && nOldPlayStatus != -1){
		int nBuf = 0;
		std::vector<int>::iterator iterDelIndex;
		for(iterDelIndex = vecDelIndex.begin(); iterDelIndex != vecDelIndex.end(); ++iterDelIndex){
			if(CurrentPlayStatus.ListPos == *iterDelIndex){
				CurrentPlayStatus.ListPos--;
			}
			if(CurrentPlayStatus.ListPos > *iterDelIndex){
				++nBuf;
			}
		}
		if(CurrentPlayStatus.ListPos < 0){	// 列表Pos到顶
			CurrentPlayStatus.ListPos = 0;
		}else{
			CurrentPlayStatus.ListPos -= nBuf;
			// 设置选中
			ctrl_MusicList.SetItemState(CurrentPlayStatus.ListPos,LVNI_SELECTED,LVNI_SELECTED);
		}
		CurrentPlayStatus.PlayerStatus = 0;
		CurrentPlayStatus.Informed = RadioList.at(CurrentPlayStatus.ListPos);
		if(nOldPlayStatus != 3){	// 正在播放、暂停时自动播放
			Sleep(1);
			OnBnClickedBtnPlay();
		}else{	// 停止时只更新标题显示
			SetDlgItemText(IDC_STATIC_TITLE, CurrentPlayStatus.Informed.MusicTitle);
		}
	}
	vecDelIndex.clear();
	return;
}


// 电台Logo打印
void CRadioPlayerDlg::PrintRadioLogo(BYTE * byData, unsigned int unSize)
{
	CWnd *pWnd = GetDlgItem(IDC_RADIOLOGO);
	CRect IDCRect;	// 获取控件大小
	pWnd->GetWindowRect(IDCRect);	// 获取坐标
	ScreenToClient(&IDCRect);
	CRect LogoRect;	// 图片大小
	pWnd->GetClientRect(&LogoRect);	// 获取图片坐标
	CDC *pDC = pWnd->GetDC();

	BYTE* lpRsrc = byData;

	// 分配的全局内存以创建流
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, unSize);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem, lpRsrc, unSize);
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);

	// 从流读取
	Image* pLogoLoad = Image::FromStream(pstm);
	
	// 释放
	GlobalUnlock(m_hMem);
	pstm->Release();
	FreeResource(lpRsrc);

	Graphics imgRadioLogo(*pDC);	// 创建GDI+
	int cx = pLogoLoad->GetWidth();
	int cy = pLogoLoad->GetHeight();
	int ccx = IDCRect.Width();
	int ccy = cy*ccx / cx;
	if (ccy > IDCRect.Height()){
		ccy = IDCRect.Height();
		ccx = cx*ccy / cy;
	}
	imgRadioLogo.SetInterpolationMode(InterpolationModeHighQualityBicubic);	// 设置模式
	//InterpolationModeInvalid          = QualityModeInvalid,    //插值无效  
	//InterpolationModeDefault          = QualityModeDefault,    //指定默认模式  
	//InterpolationModeLowQuality       = QualityModeLow,        //指定低质量插值法  
	//InterpolationModeHighQuality      = QualityModeHigh,       //指定高质量插值法  
	//InterpolationModeBilinear,                                 //指定双线性插值法  
	//InterpolationModeBicubic,                                  //指定双三次插值法  
	//InterpolationModeNearestNeighbor,                          //指定最临近插值法  
	//InterpolationModeHighQualityBilinear,                      //指定高质量双线性插值法  
	//InterpolationModeHighQualityBicubic                        //指定高质量双三次插值法  

	imgRadioLogo.DrawImage(pLogoLoad, Rect(LogoRect.left, LogoRect.top, ccx, ccy));	// 显示
	ReleaseDC(pDC);
}


// 播放列表拖动
LRESULT CRadioPlayerDlg::MessageFromListCtrl(WPARAM wParam, LPARAM lParam)
{
	if (wParam == LBUTTON_UP)
	{
		//handle the left button up message
		TRACE("left button up message");
		return 0;
	}

	int nSrc = wParam; // 起始行
	int nDes = lParam;	// 目标行
	int nPlayPos = CurrentPlayStatus.ListPos;
	//暂存要移动的项
	int nListPos = nDes;
	CString m_strCount;
	if (nDes > nSrc)
		nListPos--;
	m_strCount.Format(L"%d", nListPos);

	CString strName = RadioList[nSrc].MusicTitle;
	CString strTime = RadioList[nSrc].MusicLength;
	CString strAuthor = RadioList[nSrc].MusicAuthor;

	if (nDes > nSrc)
	{
		MusicListArr::iterator iterDes = RadioList.begin() + nDes;
		RadioList.insert(iterDes, RadioList[nSrc]);
		MusicListArr::iterator iterSrc = RadioList.begin() + nSrc;
		RadioList.erase(iterSrc);

		//将item移到要插入的位置
		int nRow = ctrl_MusicList.InsertItem(nDes, m_strCount, 0);//插入1行代表第0列的数据 参数(行数,标题,图标索引)
		ctrl_MusicList.SetItemText(nRow, 1, strName);	//设置第1列数据
		ctrl_MusicList.SetItemText(nRow, 2, strTime);	//设置第2列数据
		ctrl_MusicList.SetItemText(nRow, 3, strAuthor);	//设置第3列数据
		
		//删除要移动的项
		ctrl_MusicList.DeleteItem(nSrc);
		if (nPlayPos < nDes && nPlayPos > nSrc)
			nPlayPos--;
	}
	else
	{
		MusicListArr::iterator iterDes = RadioList.begin() + nDes;
		RadioList.insert(iterDes, RadioList[nSrc]);
		MusicListArr::iterator iterSrc = RadioList.begin() + nSrc + 1;
		RadioList.erase(iterSrc);
		//删除要移动的项
		ctrl_MusicList.DeleteItem(nSrc);
		
		//将item移到要插入的位置
		int nRow = ctrl_MusicList.InsertItem(nDes, m_strCount, 0);//插入1行代表第0列的数据 参数(行数,标题,图标索引)
		ctrl_MusicList.SetItemText(nRow, 1, strName);	//设置第1列数据
		ctrl_MusicList.SetItemText(nRow, 2, strTime);	//设置第2列数据
		ctrl_MusicList.SetItemText(nRow, 3, strAuthor);	//设置第3列数据
		if (nPlayPos > nDes && nPlayPos < nSrc)
			nPlayPos++;
	}

	if (nDes > nSrc)
		nDes--;

	if (nPlayPos < 0)
		nPlayPos = 0;

	if (CurrentPlayStatus.ListPos == nSrc)	// 拖动行为正在播放行
		CurrentPlayStatus.ListPos = nDes;
	else
		CurrentPlayStatus.ListPos = nPlayPos;
	RefMusicList();	// 列表ID刷新
	ctrl_MusicList.EnsureVisible(nDes, FALSE);	//让item在可见范围内
	ctrl_MusicList.SetItemState(nDes, LVIS_SELECTED, LVIS_SELECTED);	//高亮item

	return 0;
}


// 添加文件至数组
bool CRadioPlayerDlg::AddToList(CString strPath)
{
	CAudioPlayer m_AddFile;
	CString m_strFilePath, m_strCount;	//m_strFileName
	wchar_t wFileName[_MAX_FNAME];
	int nListCount = 0;
	TID3InfoExW m_id3_info;
	MusicInformed NewAddMusic = { NULL, L"00:00", L"-", NULL };

	nListCount = ctrl_MusicList.GetItemCount();
	m_strCount.Format(L"%d", nListCount + 1);
	NewAddMusic.MusicPath = strPath;
	_wsplitpath_s(NewAddMusic.MusicPath, NULL, 0, NULL, 0, wFileName, _MAX_FNAME, NULL, 0);	// 通过路径获取文件名
	NewAddMusic.MusicTitle = wFileName;

	if (m_AddFile.Load(this->m_hWnd, NewAddMusic.MusicPath) == false)
		return false;
	NewAddMusic.MusicLength = m_AddFile.GetMediaLengthStr();

	m_id3_info = m_AddFile.LoadID3Ex(this->m_hWnd);
	if (m_id3_info.Title != NULL){
		if (isprint(m_id3_info.Title[0]))
			NewAddMusic.MusicTitle = m_id3_info.Title;
	}
	//m_strFileName.Format(L"%s - %s", m_id3_info.Title, m_id3_info.Artist);

	if (m_id3_info.Artist != NULL){
		if (isprint(m_id3_info.Artist[0]))
			NewAddMusic.MusicAuthor = m_id3_info.Artist;
	}

	ctrl_MusicList.InsertItem(nListCount, m_strCount);
	ctrl_MusicList.SetItemText(nListCount, 1, NewAddMusic.MusicTitle);
	ctrl_MusicList.SetItemText(nListCount, 2, NewAddMusic.MusicLength);
	ctrl_MusicList.SetItemText(nListCount, 3, NewAddMusic.MusicAuthor);
	RadioList.push_back(NewAddMusic);
	m_AddFile.player->Close();

	m_AddFile.player->Release();
	return true;
}


// 响应Win7任务栏
LRESULT	CRadioPlayerDlg::OnCreateTaskBar(WPARAM wParam, LPARAM lParam)
{
	// 判断任务栏是否可用
	OSVersion = GetOSVersion();
	if (OSVersion.Major >= 6 && OSVersion.Minor >= 1)
	{
		m_pProgressTaskBar = new CTaskBar(GetSafeHwnd());
		m_pProgressTaskBar->Init();
	}
	return 0;
}


// 消息函数
LRESULT CRadioPlayerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (OSVersion.Major >= 6 && OSVersion.Minor >= 1){
		if (HIWORD(wParam) == THBN_CLICKED)
		{
			m_pProgressTaskBar->Click(wParam);
			return 0;
		}
	}
	//::SendMessage(this->m_hWnd, WM_ARGS_RECEIVED, wParam, lParam);
	return CDialogEx::WindowProc(message, wParam, lParam);
}


// 列表ID更新
void CRadioPlayerDlg::RefMusicList()
{
	int nListCount = ctrl_MusicList.GetItemCount();
	CString m_strID;
	for (int i = 0; i < nListCount; ++i){
		m_strID.Format(L"%d", i + 1);
		ctrl_MusicList.SetItemText(i, 0, m_strID);
	}
}


void CAboutDlg::OnBnClickedBtnSite()
{
	// 打开官网
	ShellExecute(this->m_hWnd, L"open", L"http://www.laijingwu.com", NULL, NULL, SW_SHOWNORMAL);
}


// 检测系统版本
OSInformed CRadioPlayerDlg::GetOSVersion()
{
	OSVERSIONINFOEX Os_WindVerInfoEx;  // 版本信息 
	int inR2;  // 版本信息 
	Os_WindVerInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO *)&Os_WindVerInfoEx); // 注意转换类型 
	inR2 = GetSystemMetrics(SM_SERVERR2);
	OSbuild pOS;
	CString  pszOS;

	if (Os_WindVerInfoEx.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		if (Os_WindVerInfoEx.dwMajorVersion <= 4)
			pszOS = pOS.Win98;
		switch (Os_WindVerInfoEx.dwMajorVersion)
		{
		case 5:
			if (Os_WindVerInfoEx.dwMinorVersion == 0)
				pszOS = pOS.Win2000;
			if (Os_WindVerInfoEx.dwMinorVersion == 1)
				pszOS = pOS.WinXP;
			if ((Os_WindVerInfoEx.dwMinorVersion == 2) && (inR2 == 0))
				pszOS = pOS.Server2003;
			if ((Os_WindVerInfoEx.dwMinorVersion == 2) && (inR2 != 0))
				pszOS = pOS.Server2003_R2;
			break;
		case 6:
			if ((Os_WindVerInfoEx.dwMinorVersion == 0) && (Os_WindVerInfoEx.wProductType == VER_NT_WORKSTATION))
				pszOS = pOS.WinVista;
			if ((Os_WindVerInfoEx.dwMinorVersion == 0) && (Os_WindVerInfoEx.wProductType != VER_NT_WORKSTATION))
				pszOS = pOS.Server2008;
			if ((Os_WindVerInfoEx.dwMinorVersion == 1) && (Os_WindVerInfoEx.wProductType == VER_NT_WORKSTATION))
				pszOS = pOS.Win7;
			if ((Os_WindVerInfoEx.dwMinorVersion == 1) && (Os_WindVerInfoEx.wProductType != VER_NT_WORKSTATION))
				pszOS = pOS.Server2008_R2;
			if ((Os_WindVerInfoEx.dwMinorVersion == 2) && (Os_WindVerInfoEx.wProductType == VER_NT_WORKSTATION))
				pszOS = pOS.Win8;
			if ((Os_WindVerInfoEx.dwMinorVersion == 2) && (Os_WindVerInfoEx.wProductType != VER_NT_WORKSTATION))
				pszOS = pOS.Server2012;
			if ((Os_WindVerInfoEx.dwMinorVersion == 3) && (Os_WindVerInfoEx.wProductType == VER_NT_WORKSTATION))
				pszOS = pOS.Win8_1;
			if ((Os_WindVerInfoEx.dwMinorVersion == 3) && (Os_WindVerInfoEx.wProductType != VER_NT_WORKSTATION))
				pszOS = pOS.Server2012_R2;
			break;
		}
	}

	OSInformed OSVersion;
	OSVersion.VersionTitle = pszOS;
	OSVersion.Major = Os_WindVerInfoEx.dwMajorVersion;
	OSVersion.Minor = Os_WindVerInfoEx.dwMinorVersion;
	if (Os_WindVerInfoEx.wProductType == VER_NT_WORKSTATION)
		OSVersion.WorkStation = true;
	else
		OSVersion.WorkStation = false;

	return OSVersion;
}
