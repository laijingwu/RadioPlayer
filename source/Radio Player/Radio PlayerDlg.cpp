
// Radio PlayerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Radio Player.h"
#include "Radio PlayerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
END_MESSAGE_MAP()


// CRadioPlayerDlg 对话框




CRadioPlayerDlg::CRadioPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRadioPlayerDlg::IDD, pParent)
	, tmp_strAddFolderPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRadioPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MUSICLIST, ctrl_MusicList);
	DDX_Control(pDX, IDC_SLIDER_VOLUME, ctrl_SliderVol);
}

BEGIN_MESSAGE_MAP(CRadioPlayerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()	// 设置字体
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

	// 音乐列表控件设置
	ctrl_MusicList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);	// 设置点选整行、显示线框
	LVCOLUMN lvColumn;
	lvColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 50;
	lvColumn.pszText = L"ID";
	ctrl_MusicList.InsertColumn(0,&lvColumn);
	lvColumn.cx = 300;
	lvColumn.pszText = L"Radio Name";
	ctrl_MusicList.InsertColumn(1,&lvColumn);
	lvColumn.cx = 70;
	lvColumn.pszText = L"Length";
	ctrl_MusicList.InsertColumn(2,&lvColumn);
	lvColumn.cx = 80;
	lvColumn.pszText = L"Author";
	ctrl_MusicList.InsertColumn(3,&lvColumn);

	// 音量控件设置
	AudioPlayer.SetVolume(100);	// 初始音量
	PlayingStatus.Vol.Master = 100;
	PlayingStatus.Vol.Player = 100;
	SetDlgItemText(IDC_STATIC_VOLUME,L"100%");
	ctrl_SliderVol.SetRange(0,100,true);
	ctrl_SliderVol.SetPos(100);

	// 额外初始化代码
	TitleFont.CreatePointFont(150,L"微软雅黑");	// 设置主标题字体
	PlayingStatus.PlayStatus = -1;	// 未载入路径状态

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
	CAudioPlayer m_AddFile;
	const int FILE_NUMBER = 30;	// 最多添加文件个数
	// 文件过滤器
	LPCTSTR lpFilter = L"Audio Files(*.mp3;*.mp2;*.mp1;*.ogg;*.flac;*.ac3;*.aac;*.oga;*.wav;*.pcm)|*.mp3;*.mp2;*.mp1;*.ogg;*.flac;*.ac3;*.aac;*.oga;*.wav;*.pcm|Mp3 Files(*.mp3)|*.mp3|Mp2 Files(*.mp2)|*.mp2|Mp1 Files(*.mp1)|*.mp1|Ogg Files(*.ogg)|*.ogg|Flac Files(*.flac)|*.flac|Ac3 Files(*.ac3)|*.ac3|Aac Files(*.aac)|*.aac|Oga Files(*.oga)|*.oga|Wave Files(*.wav)|*.wav|PCM Files(*.pcm)|*.pcm|All Files(*.*)|*.*|";
	//CFileDialog fileDlg(true,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,lpFilter,this);	// 设置单选
	CFileDialog fileDlg(TRUE,NULL,NULL,OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT,lpFilter,this);	// 设置多选
	fileDlg.m_ofn.lpstrTitle = L"Open";	// 打开文件对话框标题
	fileDlg.m_ofn.lpstrFile = new wchar_t[_MAX_PATH * FILE_NUMBER];	// 设置缓冲区
	memset(fileDlg.m_ofn.lpstrFile,0,_MAX_PATH * FILE_NUMBER);	// 初始化缓冲区
	fileDlg.m_ofn.nMaxFile = _MAX_PATH * FILE_NUMBER;
	if (fileDlg.DoModal() == IDCANCEL){
		return;
	}
	CString m_strFilePath,m_strFileName,m_strCount,m_strID3Artist;
	wchar_t wFileName[_MAX_FNAME];
	POSITION posFile = fileDlg.GetStartPosition();
	int nMListCount;
	TID3InfoExW m_id3_info;
	while(posFile){
		nMListCount = ctrl_MusicList.GetItemCount();
		m_strFilePath = fileDlg.GetNextPathName(posFile);
		_wsplitpath_s(m_strFilePath,NULL,0,NULL,0,wFileName,_MAX_FNAME,NULL,0);	// 通过路径获取文件名
		m_strFileName = wFileName;
		arrMusicListPath.push_back(m_strFilePath);	// 添加路径到数组

		if(m_AddFile.Load(this->m_hWnd,m_strFilePath) == false){
			continue;
		}
		m_id3_info = m_AddFile.LoadID3Ex(this->m_hWnd);
		m_strCount.Format(L"%d",nMListCount+1);
		if(m_id3_info.Title){
			//m_strFileName.Format(L"%s - %s",id3_info.Title,id3_info.Artist);
			m_strFileName = m_id3_info.Title;
			m_strID3Artist = m_id3_info.Artist;
		}
		ctrl_MusicList.InsertItem(nMListCount,m_strCount);
		ctrl_MusicList.SetItemText(nMListCount,1,m_strFileName);
		ctrl_MusicList.SetItemText(nMListCount,2,m_AddFile.GetMediaLengthStr());
		ctrl_MusicList.SetItemText(nMListCount,3,m_strID3Artist);
		m_strID3Artist.Empty();
		m_AddFile.player->Close();
	}
	m_AddFile.player->Release();
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
	CAudioPlayer m_AddFolder;
	CFileFind finder;
	int nMListCount = 0;
	TID3InfoExW m_id3_info;
	CString m_strFilePath,m_strFileName,m_strFileTitle,m_strCount,m_strID3Artist;
	BOOL bWorking = finder.FindFile(strPath + "\\*.*");
	while(bWorking){
		bWorking = finder.FindNextFile();
		m_strFileName = finder.GetFileName();
		m_strFileTitle = finder.GetFileTitle();
		nMListCount = ctrl_MusicList.GetItemCount();

		if(m_strFileName.Right(3) == "mp3" || m_strFileName.Right(3) == "mp2" || m_strFileName.Right(3) == "mp1" || m_strFileName.Right(3) == "ogg" || m_strFileName.Right(4) == "flac" || m_strFileName.Right(3) == "ac3" || m_strFileName.Right(3) == "aac" || m_strFileName.Right(3) == "oga" || m_strFileName.Right(3) == "wav" || m_strFileName.Right(3) == "pcm"){
			m_strFilePath = strPath + "\\" + m_strFileName;
			arrMusicListPath.push_back(m_strFilePath);
			if(m_AddFolder.Load(this->m_hWnd,m_strFilePath) == false){
				continue;
			}
			m_id3_info = m_AddFolder.LoadID3Ex(this->m_hWnd);
			m_strCount.Format(L"%d",nMListCount+1);
			if(m_id3_info.Title){
				//m_strFileName.Format(L"%s - %s",id3_info.Title,id3_info.Artist);
				m_strFileTitle = m_id3_info.Title;
				m_strID3Artist = m_id3_info.Artist;
			}
			ctrl_MusicList.InsertItem(nMListCount,m_strCount);
			ctrl_MusicList.SetItemText(nMListCount,1,m_strFileTitle);
			ctrl_MusicList.SetItemText(nMListCount,2,m_AddFolder.GetMediaLengthStr());
			ctrl_MusicList.SetItemText(nMListCount,3,m_strID3Artist);
			m_strID3Artist.Empty();
			m_AddFolder.player->Close();
		}
		if(finder.IsDirectory() && !finder.IsDots()){
			AddFolderToList(finder.GetFilePath());
		}
	}
	m_AddFolder.player->Release();
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
		PlayingStatus.Path = arrMusicListPath.at(nPos-1);
		PlayingStatus.PlayStatus = 0;
		PlayingStatus.ListPos = nPos - 1;
		PlayingStatus.Title = ctrl_MusicList.GetItemText(nPos-1,1);
		PlayingStatus.Length = ctrl_MusicList.GetItemText(nPos-1,2);
		PlayingStatus.Artist = ctrl_MusicList.GetItemText(nPos-1,3);
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
			PlayingStatus.Path = arrMusicListPath.at(0);
			PlayingStatus.PlayStatus = 0;
			PlayingStatus.ListPos = 0;
			PlayingStatus.Title = ctrl_MusicList.GetItemText(0,1);
			PlayingStatus.Length = ctrl_MusicList.GetItemText(0,2);
			PlayingStatus.Artist = ctrl_MusicList.GetItemText(0,3);
			OnBnClickedBtnPlay();
			return;
		}
	}else if(PlayingStatus.PlayStatus == -1){	// 未加载路径
		PlayingStatus.Path = arrMusicListPath.at(0);
		PlayingStatus.PlayStatus = 0;
		PlayingStatus.ListPos = 0;
		PlayingStatus.Title = ctrl_MusicList.GetItemText(0,1);
		PlayingStatus.Length = ctrl_MusicList.GetItemText(0,2);
		PlayingStatus.Artist = ctrl_MusicList.GetItemText(0,3);
		OnBnClickedBtnPlay();
	}else if(PlayingStatus.PlayStatus == 0){	// 设备就绪
		if(AudioPlayer.Play(this->m_hWnd,PlayingStatus.Path) == false){
			return;
		}
		PlayingStatus.PlayStatus = 1;
		GetDlgItem(IDC_BTN_PLAY)->SetWindowTextW(L"Pause");
		SetDlgItemText(IDC_STATIC_TITLE,PlayingStatus.Title);
	}else if(PlayingStatus.PlayStatus == 1){	// 正在播放
		AudioPlayer.Pause();
		PlayingStatus.PlayStatus = 2;
		GetDlgItem(IDC_BTN_PLAY)->SetWindowTextW(L"Play");
	}else if(PlayingStatus.PlayStatus == 2){	// 暂停
		AudioPlayer.Resume();
		PlayingStatus.PlayStatus = 1;
		GetDlgItem(IDC_BTN_PLAY)->SetWindowTextW(L"Pause");
	}
	return;
}


// 停止按钮
void CRadioPlayerDlg::OnBnClickedBtnStop()
{
	AudioPlayer.Stop();
	PlayingStatus.PlayStatus = 0;
	GetDlgItem(IDC_BTN_PLAY)->SetWindowTextW(L"Play");
	return;
}


// 关闭会话前
void CRadioPlayerDlg::OnClose()
{
	if(PlayingStatus.PlayStatus == 1){
		if(MessageBox(L"You are listening to the radio, are you sure you want to quit?",L"Tips",MB_YESNO|MB_ICONINFORMATION) == IDNO){
			return;
		}
	}
	CDialogEx::OnClose();
}


// 窗体销毁前
void CRadioPlayerDlg::OnDestroy()
{
	AudioPlayer.Stop(true);
	AudioPlayer.player->Release();
	CDialogEx::OnDestroy();
}


// 静音按钮
void CRadioPlayerDlg::OnBnClickedBtnQuiet()
{
	//CString tmp_strQUIET;
	//GetDlgItemText(IDC_BTN_QUIET,tmp_strQUIET); && tmp_strQUIET.Compare(L"Silent") == 0
	if(PlayingStatus.Vol.Master == 100){
		AudioPlayer.player->SetMasterVolume(0,0);
		PlayingStatus.Vol.Master = 0;
		ctrl_SliderVol.SetPos(0);
		GetDlgItem(IDC_BTN_QUIET)->SetWindowTextW(L"Silent √");
		SetDlgItemText(IDC_STATIC_VOLUME,L"0%");
	}else{
		AudioPlayer.player->SetMasterVolume(100,100);
		PlayingStatus.Vol.Master = 100;
		CString tmp_strVol;
		tmp_strVol.Format(L"%d%%",PlayingStatus.Vol.Player);
		ctrl_SliderVol.SetPos(PlayingStatus.Vol.Player);
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
		AudioPlayer.SetVolume(nVolumn);
		PlayingStatus.Vol.Player = nVolumn;
		CString tmp_strVol;
		tmp_strVol.Format(L"%d%%",nVolumn);
		SetDlgItemText(IDC_STATIC_VOLUME,tmp_strVol);
		if(nVolumn != 0){
			GetDlgItem(IDC_BTN_QUIET)->SetWindowTextW(L"Silent");
		}
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
