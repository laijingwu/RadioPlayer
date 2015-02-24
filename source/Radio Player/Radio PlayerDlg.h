
// Radio PlayerDlg.h : ͷ�ļ�
//

#pragma once
#include "AudioPlayer.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "Prog_PlayTimeProc.h"
#include "Common.h"
#include "stdafx.h"
#include <time.h>
#include <MMSystem.h>
#pragma comment(lib,"Winmm.lib")
#include "Ctrl_PlayList.h"
#include "TaskBar.h"

// CRadioPlayerDlg �Ի���
class CRadioPlayerDlg : public CDialogEx
{
// ����
public:
	CRadioPlayerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RADIOPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT MessageFromListCtrl(WPARAM wParam, LPARAM lParam); 	/*�����Զ������Ϣ��Ӧ����*/
	DECLARE_MESSAGE_MAP()
public:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CTaskBar *m_pProgressTaskBar;
	afx_msg LRESULT OnCreateTaskBar(WPARAM wParam, LPARAM lParam);
	// TEMP
	CString tmp_strAddFolderPath;

	afx_msg void OnBnClickedBtnAbout();
	afx_msg void OnBnClickedBtnExit();
	CAudioPlayer AudioPlayer;
	CCtrl_PlayList ctrl_MusicList;
	CSliderCtrl ctrl_SliderVol;
	afx_msg void OnBnClickedBtnAddfile();
	MusicListArr RadioList;
	afx_msg void OnBnClickedBtnAddfolder();
	void AddFolderToList(CString strFilePath);
	afx_msg void OnNMDblclkMusiclist(NMHDR *pNMHDR, LRESULT *pResult);
	CFont TitleFont;
	MediaStatus CurrentPlayStatus;
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnQuiet();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	Time RunTime;
	CProg_PlayTimeProc prog_PlayTime;
	LRESULT Prog_PlayTimeProc(WPARAM wParam, LPARAM lParam);
	Setting PlayerSetting;
	CCommon Common;
	afx_msg void OnBnClickedBtnLoadlist();
	afx_msg void OnBnClickedBtnStorelist();
	void ChangeRadio(CString strPath);
	afx_msg void OnBnClickedBtnLastradio();
	afx_msg void OnBnClickedBtnNextradio();
	afx_msg void OnNMRClickMusiclist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuDelete();
	void PrintRadioLogo(BYTE * byData, unsigned int unSize);
	bool AddToList(CString strPath);
	void RefMusicList();
	OSInformed GetOSVersion();
	OSInformed OSVersion;
};
