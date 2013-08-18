#include "StdAfx.h"
#include "AudioPlayer.h"

CAudioPlayer::CAudioPlayer(void)
	: OnlyOneOpen(false)
{
	player = CreateZPlay();
	player->SetMasterVolume(100,100);
}


CAudioPlayer::~CAudioPlayer(void)
{
}


// �����ļ�
bool CAudioPlayer::Load(HWND hWnd, CString strFilePath)
{
	if(player->OpenFileW(strFilePath,sfAutodetect) == 0){
		MessageBox(hWnd,player->GetErrorW(),L"Core Error",MB_ICONHAND|MB_ICONSTOP|MB_ICONERROR);
		player->Close();
		return false;
	}
	OnlyOneOpen = true;
	return true;
}


// ����
bool CAudioPlayer::Play(HWND hWnd, CString strFilePath)
{
	if(OnlyOneOpen == false){
		if(Load(hWnd,strFilePath) == false){
			return false;
		}
	}
	player->Play();
	return true;
}


// ��ͣ
void CAudioPlayer::Pause(void)
{
	player->Pause();
	return;
}


// �ָ�
void CAudioPlayer::Resume(void)
{
	player->Resume();
	return;
}


// ֹͣ
void CAudioPlayer::Stop(bool bClose)
{
	player->Stop();
	if(bClose == true){
		player->Close();
		OnlyOneOpen = false;
	}
	return;
}


// ��ȡlibZPlay��汾
CString CAudioPlayer::GetVersion(void)
{
	CString m_strVersion;
	int nVer = player->GetVersion();
	m_strVersion.Format(L"%d.%02d",nVer / 100,nVer % 100);
	return m_strVersion;
}


// ��ȡý���ļ���ʱ�䣨��񻯣�
CString CAudioPlayer::GetMediaLengthStr(void)
{
	TStreamInfo m_info;
	player->GetStreamInfo(&m_info);
	CString m_strLength;
	m_strLength.Format(L"%02d:%02d",m_info.Length.hms.hour*60+m_info.Length.hms.minute,m_info.Length.hms.second);
	return m_strLength;
}


// ��ȡý���ļ�ID3��Ϣ
TID3InfoExW CAudioPlayer::LoadID3Ex(HWND hWnd, CString strFilePath)
{
	TID3InfoExW id3_info;
	if(strFilePath.IsEmpty()){
		player->LoadID3ExW(&id3_info,0);
		return id3_info;
	}else{
		if(player->LoadFileID3ExW(strFilePath,sfAutodetect,&id3_info,0) == 0){
			//MessageBox(hWnd,player->GetErrorW(),L"Core Error",MB_ICONHAND|MB_ICONSTOP|MB_ICONERROR);
			return TID3InfoExW();
		}
		return id3_info;
	}
}


// ��ȡ���ŵ�ǰʱ�䣨��񻯣�
CString CAudioPlayer::GetMediaTimeStr(void)
{
	TStreamTime pos;
	player->GetPosition(&pos);
	CString m_strMediaPosition;
	m_strMediaPosition.Format(L"%02d:%02d",pos.hms.hour*60+pos.hms.minute,pos.hms.second);
	return m_strMediaPosition;
}


// ��������
void CAudioPlayer::SetVolume(unsigned int nVol)
{
	player->SetPlayerVolume(nVol,nVol);
	return;
}


// ��ȡý���ļ���ʱ��
unsigned int CAudioPlayer::GetMediaLength(void)
{
	unsigned int m_Length;
	TStreamInfo m_info;
	player->GetStreamInfo(&m_info);
	m_Length = ((m_info.Length.hms.hour*60+m_info.Length.hms.minute)*60+m_info.Length.hms.second)*1000+m_info.Length.hms.millisecond;
	return m_Length;
}


// ��ȡ���ŵ�ǰʱ��
unsigned int CAudioPlayer::GetMediaTime(void)
{
	unsigned int m_Pos;
	TStreamTime pos;
	player->GetPosition(&pos);
	m_Pos = ((pos.hms.hour*60+pos.hms.minute)*60+pos.hms.second)*1000+pos.hms.millisecond;
	return m_Pos;
}
