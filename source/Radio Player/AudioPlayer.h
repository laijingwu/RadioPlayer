#pragma once
#include "libzplay.h"
using namespace libZPlay;
#pragma comment(lib,"libzplay.lib")

class CAudioPlayer
{
public:
	CAudioPlayer(void);
	~CAudioPlayer(void);
	ZPlay *player;
	bool Load(HWND hWnd, CString strFilePath);
	bool Play(HWND hWnd, CString strFilePath);
	void Pause(void);
	void Resume(void);
	bool OnlyOneOpen;
	void Stop(bool bClose = false);
	CString GetVersion(void);
	CString GetMediaLengthStr(void);
	TID3InfoExW LoadID3Ex(HWND hWnd, CString strFilePath = NULL);
	CString GetMediaTimeStr(void);
	void SetVolume(unsigned int nVol);
	unsigned int GetMediaLength(void);
	unsigned int GetMediaTime(void);
};

