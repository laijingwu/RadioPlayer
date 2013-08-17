#include "StdAfx.h"
#include "AudioPlayer.h"

CAudioPlayer::CAudioPlayer(void)
	: OnlyOneOpen(false)
{
}


CAudioPlayer::~CAudioPlayer(void)
{
}

ZPlay *player = CreateZPlay();


void CAudioPlayer::Load(HWND hWnd, CString strFilePath)
{
	if(player->OpenFileW(strFilePath,sfAutodetect) == 0){
		MessageBox(hWnd,player->GetErrorW(),L"Core Error",MB_ICONHAND|MB_ICONSTOP|MB_ICONERROR);
		player->Close();
		return;
	}
	OnlyOneOpen = true;
	return;
}


void CAudioPlayer::Play(HWND hWnd, CString strFilePath)
{
	if(OnlyOneOpen == false){
		Load(hWnd,strFilePath);
	}
	player->Play();
	return;
}


void CAudioPlayer::Pause(void)
{
	player->Pause();
	return;
}


void CAudioPlayer::Resume(void)
{
	player->Resume();
	return;
}


void CAudioPlayer::Stop(bool bClose = false)
{
	player->Stop();
	if(bClose == true){
		player->Close();
		OnlyOneOpen = false;
	}
	return;
}


CString CAudioPlayer::GetVersion(void)
{
	CString m_strVersion;
	int nVer = player->GetVersion();
	m_strVersion.Format(L"%d.%02d",nVer / 100,nVer % 100);
	return m_strVersion;
}


CString CAudioPlayer::GetMediaLengthStr(void)
{
	TStreamInfo m_info;
	player->GetStreamInfo(&m_info);
	CString m_strLength;
	if(m_info.Length.hms.hour > 0){
		m_strLength.Format(L"%02d:%02d:%02d",m_info.Length.hms.hour,m_info.Length.hms.minute,m_info.Length.hms.second);
	}else{
		m_strLength.Format(L"%02d:%02d",m_info.Length.hms.minute,m_info.Length.hms.second);
	}	
	return m_strLength;
}


TID3InfoExW CAudioPlayer::LoadID3Ex(HWND hWnd, CString strFilePath = NULL)
{
	TID3InfoExW id3_info;
	if(strFilePath.IsEmpty()){
		player->LoadID3ExW(&id3_info,0);
		return id3_info;
	}else{
		if(player->LoadFileID3ExW(strFilePath,sfAutodetect,&id3_info,0) == 0){
			MessageBox(hWnd,player->GetErrorW(),L"Core Error",MB_ICONHAND|MB_ICONSTOP|MB_ICONERROR);
			return TID3InfoExW();
		}
		return id3_info;
	}
}
