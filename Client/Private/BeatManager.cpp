#include "../Default/stdafx.h"

#include "BeatManager.h"
#include "EngineInstance.h"

#include "Animation.h"

IMPLEMENT_SINGLETON(CBeatManager)

CBeatManager::CBeatManager()
{
}

HRESULT CBeatManager::Initialize()
{
	return S_OK;
}

HRESULT CBeatManager::Update(const _float fTimedelta)
{
	m_fCurTime += fTimedelta;

	if (m_fCurTime >= 60.f / m_iCurBpm)
	{
		m_fCurTime = 60.f / m_iCurBpm;
		m_bHit = TRUE;
	}
	else
		m_bHit = FALSE;

	return S_OK;
}

const _uint CBeatManager::Get_BPM()
{
	return ENGINE_INSTANCE->Get_BPM(); 
} 

const _double CBeatManager::Get_BPS()
{
	return ENGINE_INSTANCE->Get_BPM() / 60.f;
}

const _float CBeatManager::Get_AnimTimePerFrameSyncBeat(CAnimation* pAnim)
{
	if (nullptr == pAnim)
		return 0.f;

	/* 애니메이션 총 재생 시간 */
	const _float fDuration = pAnim->Get_Duration();

	/* 애니메이션의 프레임 갯수 */
	const _uint iCount = (_float)pAnim->Get_MaxFrameCount();

	/* 애니메이션 재생에 필요한 비트 갯수 */
	_uint	iBeats = fDuration / Get_BPS();

	/* 비트당 애니메이션 재생 시간 */
	_double	fTimePerBeat = fDuration / iBeats;

	/* 프레임당 재생시간 */
	_float	fTimePerFrame = fTimePerBeat / iCount;

	return fTimePerFrame;
}

void CBeatManager::Reset()
{
	m_bHit		= FALSE;

	m_fCurTime	= 0.f;
	m_iCurBpm	= ENGINE_INSTANCE->Get_BPM();
}

void CBeatManager::Free()
{
	__super::Free();
}
