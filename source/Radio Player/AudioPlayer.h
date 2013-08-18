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
	void Play(HWND hWnd, CString strFilePath);
	void Pause(void);
	void Resume(void);
	bool OnlyOneOpen;
	void Stop(bool bClose);
	CString GetVersion(void);
	CString GetMediaLengthStr(void);
	TID3InfoExW LoadID3Ex(HWND hWnd, CString strFilePath = NULL);
	CString GetMediaTimeStr(void);
	void SetVolumn(unsigned int nVol);
};

