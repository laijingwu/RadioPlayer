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


// 加载文件
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


// 播放
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


// 暂停
void CAudioPlayer::Pause(void)
{
	player->Pause();
	return;
}


// 恢复
void CAudioPlayer::Resume(void)
{
	player->Resume();
	return;
}


// 停止
void CAudioPlayer::Stop(bool bClose)
{
	player->Stop();
	if(bClose == true){
		player->Close();
		OnlyOneOpen = false;
	}
	return;
}


// 获取libZPlay库版本
CString CAudioPlayer::GetVersion(void)
{
	CString m_strVersion;
	int nVer = player->GetVersion();
	m_strVersion.Format(L"%d.%02d",nVer / 100,nVer % 100);
	return m_strVersion;
}


// 获取媒体文件总时间（风格化）
CString CAudioPlayer::GetMediaLengthStr(void)
{
	TStreamInfo m_info;
	player->GetStreamInfo(&m_info);
	CString m_strLength;
	m_strLength.Format(L"%02d:%02d",m_info.Length.hms.hour*60+m_info.Length.hms.minute,m_info.Length.hms.second);
	return m_strLength;
}


// 获取媒体文件ID3信息
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


// 获取播放当前时间（风格化）
CString CAudioPlayer::GetMediaTimeStr(void)
{
	TStreamTime pos;
	player->GetPosition(&pos);
	CString m_strMediaPosition;
	m_strMediaPosition.Format(L"%02d:%02d",pos.hms.hour*60+pos.hms.minute,pos.hms.second);
	return m_strMediaPosition;
}


// 设置音量
void CAudioPlayer::SetVolume(unsigned int nVol)
{
	player->SetPlayerVolume(nVol,nVol);
	return;
}


// 获取媒体文件总时间
unsigned int CAudioPlayer::GetMediaLength(void)
{
	unsigned int m_Length;
	TStreamInfo m_info;
	player->GetStreamInfo(&m_info);
	m_Length = ((m_info.Length.hms.hour*60+m_info.Length.hms.minute)*60+m_info.Length.hms.second)*1000+m_info.Length.hms.millisecond;
	return m_Length;
}


// 获取播放当前时间
unsigned int CAudioPlayer::GetMediaTime(void)
{
	unsigned int m_Pos;
	TStreamTime pos;
	player->GetPosition(&pos);
	m_Pos = ((pos.hms.hour*60+pos.hms.minute)*60+pos.hms.second)*1000+pos.hms.millisecond;
	return m_Pos;
}
