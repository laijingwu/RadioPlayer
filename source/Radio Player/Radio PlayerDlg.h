
// Radio PlayerDlg.h : 头文件
//

#pragma once
#include "audioplayer.h"
#include "AudioPlayer.h"
#include "afxcmn.h"


// 自定义结构体
//struct AddFolderThreadParam{
//	CString Path;
//	PVOID dlg;
//};


// CRadioPlayerDlg 对话框
class CRadioPlayerDlg : public CDialogEx
{
// 构造
public:
	CRadioPlayerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RADIOPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// TEMP
	CString tmp_strAddFolderPath;

	afx_msg void OnBnClickedBtnAbout();
	afx_msg void OnBnClickedBtnExit();
	CAudioPlayer AudioPlayer;
	CListCtrl ctrl_MusicList;
	CSliderCtrl ctrl_SliderVol;
	afx_msg void OnBnClickedBtnAddfile();
	ListArray arrMusicListPath;
	afx_msg void OnBnClickedBtnAddfolder();
	void AddFolderToList(CString strFilePath);
};
