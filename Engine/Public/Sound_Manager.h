#pragma once
#include "Base.h"

/* Engine_Define으로 옮겼다가 난리 난다. 게임 인스턴스로 사용 */
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
	HRESULT Initialize(string soundFilePath);
	HRESULT Update(const _float& fTimedelta);

public:
	void	PlaySound(_uint eSoundID, _uint eChannelID, float fVolume);

	HRESULT Play_BGM(_uint eSoundID, _uint eChannelID, float fVolume);
	HRESULT Add_BGM(_uint eSoundID, _uint eChannelID, float fVolume);
	
	void	Stop_Sound(_uint eChannelID);
	void	Stop_All();
	
	void	Set_Volume(_uint eChannelID, float fVolume);

	const _uint Get_BPM();

private:
	void	Load_SoundFile(string soundFilePath);
	HRESULT	Load_SoundFileNames(string soundFilePath);

private:
	map<TCHAR*, FMOD_SOUND*>	m_mapSound;
	FMOD_SYSTEM*				m_pSystem = { nullptr };

	FMOD_CHANNEL*				m_Channels[MAX_CHANNEL];
	vector<TCHAR*>				m_SoundNames;


	/* BGM */
	_int						m_iCurBgmID = -1;
	_int						m_iCurBgmChannel = -1;

	LERP_FLOAT_DESC				m_tCurBgmVolume;


public:
	virtual void Free() override;
};

END

