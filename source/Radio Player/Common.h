#pragma once


// CCommon

class CCommon : public CWnd
{
	DECLARE_DYNAMIC(CCommon)

public:
	CCommon();
	virtual ~CCommon();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void MusicListSerialize(CArchive &ar);
	bool StoreMusicList(CString strListPath = NULL);
	CString GetCurrWorkingDir(void);
	bool LoadMusicList(CString strListPath = NULL);
	unsigned int GetTimeUnix(int nYear, int nMon, int nDay, int nHour, int nMin, int nSec);
};


