#pragma once
#include "Engine_Defines.h"
#include "Client_Defines.h"
#include "fmod.h"

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

class CSoundMgr
{
public:
	static CSoundMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSoundMgr;

		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
public:
	
private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Initialize();

	void Release();
public:
	int  VolumeUp(CHANNELID eID, _float _vol);
	int  VolumeDown(CHANNELID eID, _float _vol);
	int  BGMVolumeUp(_float _vol);
	int  BGMVolumeDown(_float _vol);
	int  Pause(CHANNELID eID);
	void PlaySound(const TCHAR* pSoundKey, CHANNELID eID, _float _vol);
	void PlayBGM(const TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetSoundOffset(float fSoundOffset) {
		m_fSoundOffset = fSoundOffset;
	}

private:
	float m_volume = SOUND_DEFAULT;
	float m_BGMvolume = 0.2f;
	float m_fSoundOffset = 1.f;
	FMOD_BOOL m_bool;

private:
	void LoadSoundFile();

private:
	static CSoundMgr* m_pInstance;
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
	FMOD_SYSTEM* m_pSystem;
	_bool		m_bPause = false;
};
#define SoundMgr CSoundMgr::Get_Instance()
