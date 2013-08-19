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
	ListArray::size_type m_strSize;
	ListArray::iterator iterList;
	m_strSize = pDlg->arrMusicListPath.size();
	unsigned int nSize;
	nSize = m_strSize;
	if(ar.IsStoring()){	// ����
		ar<<nSize;
		for(iterList = pDlg->arrMusicListPath.begin(); iterList != pDlg->arrMusicListPath.end(); iterList++){
			ar<<*iterList;
		}
	}else{	//��ȡ
		ar>>nSize;
		for(int i = 0; i != nSize; ++i){
			pDlg->arrMusicListPath.push_back(L"");
			ar>>pDlg->arrMusicListPath[i];
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
	TCHAR szFull[_MAX_PATH];
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	::GetModuleFileName(NULL, szFull, sizeof(szFull)/sizeof(TCHAR));
	_tsplitpath(szFull, szDrive, szDir, NULL, NULL);
	_tcscpy(szFull, szDrive);
	_tcscat(szFull, szDir);
	strPath = CString(szFull);
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
