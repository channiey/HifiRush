#pragma once
#include "Base.h"

/* Engine_Define���� �Ű�ٰ� ���� ����. ���� �ν��Ͻ��� ��� */
#include "fmod.h"
#include "fmod.hpp"
#pragma comment(lib, "fmodex_vc.lib")
#include <io.h>

BEGIN(Engine)

class CSound_Manager final : public CBase
{
	DECLARE_SINGLETON(CSound_Manager)

private:
	CSound_Manager();
	virtual ~CSound_Manager() = default;

public:
	HRESULT Initialize(const char* strFilePath);

public:
	void	PlaySound(TCHAR* pSoundKey, _uint eChannelID, float fVolume);
	HRESULT PlayBGM(TCHAR* pSoundKey, _uint eChannelID, float fVolume);
	void	StopSound(_uint eChannelID);
	void	StopAll();
	void	SetChannelVolume(_uint eChannelID, float fVolume);

private:
	void	LoadSoundFile(const char* strFilePath);

private:
	map<TCHAR*, FMOD_SOUND*>	m_mapSound;
	FMOD_SYSTEM*				m_pSystem = { nullptr };

	FMOD_CHANNEL*				m_Channels[MAX_CHANNEL];

public:
	virtual void Free() override;
};

END

