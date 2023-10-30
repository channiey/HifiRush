#include "Sound_Manager.h"
#include "Util_String.h"

IMPLEMENT_SINGLETON(CSound_Manager)

CSound_Manager::CSound_Manager()
{
}

HRESULT CSound_Manager::Initialize(string soundFilePath)
{
	FMOD_System_Create(&m_pSystem);

	FMOD_System_Init(m_pSystem, MAX_CHANNEL, FMOD_INIT_NORMAL, NULL);

	Load_SoundFile(soundFilePath);

	return S_OK;
}

void CSound_Manager::PlaySound(TCHAR* pSoundKey, _uint eChannelID, float fVolume)
{
	Stop_Sound(eChannelID);

	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;

	if (FMOD_Channel_IsPlaying(m_Channels[eChannelID], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_Channels[eChannelID]);
	}

	FMOD_Channel_SetVolume(m_Channels[eChannelID], fVolume);

	FMOD_System_Update(m_pSystem);

}

HRESULT CSound_Manager::Play_BGM(TCHAR* pSoundKey, _uint eChannelID, float fVolume)
{

	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return E_FAIL;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_Channels[eChannelID]);
	FMOD_Channel_SetMode(m_Channels[eChannelID], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_Channels[eChannelID], fVolume);
	FMOD_System_Update(m_pSystem);

	return S_OK;
}

void CSound_Manager::Stop_Sound(_uint eChannelID)
{
	FMOD_Channel_Stop(m_Channels[eChannelID]);

}

void CSound_Manager::Stop_All()
{
	for (int i = 0; i < MAX_CHANNEL; ++i)
		FMOD_Channel_Stop(m_Channels[i]);
}

void CSound_Manager::Set_Volume(_uint eChannelID, float fVolume)
{
	FMOD_Channel_SetVolume(m_Channels[eChannelID], fVolume);

	FMOD_System_Update(m_pSystem);
}

void CSound_Manager::Load_SoundFile(string soundFilePath)
{	
	_finddata_t fd;

	string tempPath = soundFilePath + "*.*";

	long long handle = _findfirst(tempPath.c_str(), &fd); // 64bit니까 long lone이나 intptr_t으로 사용 (32bit는 long 사용 가능)

	if (handle == -1)
		return;

	int iResult = 0;

	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, soundFilePath.c_str());
		strcat_s(szFullPath, fd.name);

		FMOD_SOUND* pSound = nullptr;
		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = (int)strlen(fd.name) + 1;

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);

			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			m_mapSound.emplace(pSoundKey, pSound);
		}

		iResult = _findnext(handle, &fd);
	}

	FMOD_System_Update(m_pSystem);

	_findclose(handle);
}


void CSound_Manager::Free()
{
	__super::Free();

	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}
