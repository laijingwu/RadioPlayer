
// Radio PlayerDlg.h : ͷ�ļ�
//

#pragma once
#include "audioplayer.h"
#include "AudioPlayer.h"
#include "afxcmn.h"
#include "afxwin.h"

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
	afx_msg void OnNMDblclkMusiclist(NMHDR *pNMHDR, LRESULT *pResult);
	CFont TitleFont;
	MediaStatus PlayingStatus;
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnQuiet();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
