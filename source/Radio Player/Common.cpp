// Common.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Radio Player.h"
#include "Radio PlayerDlg.h"
#include "Common.h"


// CCommon

IMPLEMENT_DYNAMIC(CCommon, CWnd)

CCommon::CCommon()
{

}

CCommon::~CCommon()
{
}


BEGIN_MESSAGE_MAP(CCommon, CWnd)
END_MESSAGE_MAP()



// CCommon ��Ϣ�������



// ���л�����
void CCommon::MusicListSerialize(CArchive &ar)
{
	// ��ʼ����������
	CRadioPlayerDlg *pDlg = (CRadioPlayerDlg*)AfxGetMainWnd();
	MusicListArr::size_type m_strSize;
	MusicListArr::iterator iterList;
	m_strSize = pDlg->RadioList.size();
	unsigned int nSize;
	nSize = m_strSize;
	if(ar.IsStoring()){	// ����
		ar<<nSize;
		for (iterList = pDlg->RadioList.begin(); iterList != pDlg->RadioList.end(); iterList++){
			ar<<(*iterList).MusicPath;
		}
	}else{	//��ȡ
		ar>>nSize;
		for(int i = 0; i != nSize; ++i){
			CString strPath;
			ar >> strPath;
			pDlg->AddToList(strPath);
		}
	}
}


// ���沥���б�
bool CCommon::StoreMusicList(CString strListPath)
{
	CRadioPlayerDlg *pDlg = (CRadioPlayerDlg*)AfxGetMainWnd();
	if(strListPath.IsEmpty()){
		strListPath = GetCurrWorkingDir() + pDlg->PlayerSetting.MusicList.defStoreName + pDlg->PlayerSetting.MusicList.defStoreType;
	}
	CFile fp;
	if(fp.Open(strListPath,CFile::modeCreate|CFile::modeWrite)){
		CArchive ar(&fp,CArchive::store);
		this->MusicListSerialize(ar);
		ar.Close();
		fp.Close();
		return true;
	}
	return false;
}


// ��ȡ��������Ŀ¼
CString CCommon::GetCurrWorkingDir(void)
{
	CString strPath;
	wchar_t szwRoot[_MAX_PATH];
	wchar_t szwFull[_MAX_PATH];
	wchar_t szwDrive[_MAX_DRIVE];
	wchar_t szwDir[_MAX_DIR];
	wchar_t szwFName[_MAX_FNAME];
	wchar_t szwExt[_MAX_EXT];
	GetModuleFileName(NULL, szwFull, sizeof(szwFull) / sizeof(wchar_t));
	_wsplitpath_s(szwFull, szwDrive, _MAX_DRIVE, szwDir, _MAX_DIR, szwFName, _MAX_FNAME, szwExt, _MAX_EXT);
	wcscpy_s(szwRoot, _MAX_DRIVE, szwDrive);
	wcscat_s(szwRoot, _MAX_DIR, szwDir);
	strPath = CString(szwRoot);
	return strPath;
}


// ��ȡ�����б�
bool CCommon::LoadMusicList(CString strListPath)
{
	// ��ʼ����������
	CRadioPlayerDlg *pDlg = (CRadioPlayerDlg*)AfxGetMainWnd();
	if(strListPath.IsEmpty()){
		strListPath = GetCurrWorkingDir() + pDlg->PlayerSetting.MusicList.defStoreName + pDlg->PlayerSetting.MusicList.defStoreType;
	}
	CFile fp;
	if(fp.Open(strListPath,CFile::modeRead)){
		CArchive ar(&fp,CArchive::load);
		this->MusicListSerialize(ar);
		ar.Close();
		fp.Close();
		return true;
	}
	return false;	
}


// ��ȡUnixʱ���
unsigned int CCommon::GetTimeUnix(int nYear, int nMon, int nDay, int nHour, int nMin, int nSec)
{
	struct tm stm;
	memset(&stm,0,sizeof(stm));
	stm.tm_year = nYear - 1900;
	stm.tm_mon = nMon - 1;
	stm.tm_mday = nDay;
	stm.tm_hour = nHour;
	stm.tm_min = nMin;
	stm.tm_sec = nSec;  
	return (unsigned int)mktime(&stm);
}
