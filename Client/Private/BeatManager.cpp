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
	Update_Beat(fTimedelta);
	
	return S_OK;
}

const _uint CBeatManager::Get_BPM()
{
	return ENGINE_INSTANCE->Get_BPM(); 
} 

const _double CBeatManager::Get_BPS()
{
	return (_double)ENGINE_INSTANCE->Get_BPM() / (_double)60.f;
}

const _double CBeatManager::Get_SPB()
{
	return (_double)60.f / (_double)ENGINE_INSTANCE->Get_BPM();
}

const _double CBeatManager::Get_AnimTimePerFrame(CAnimation* pAnim)
{
	if (nullptr == pAnim)
		return 0.f;

	/* 애니메이션 총 재생 시간 (계산 편의상, 0 ~ 3초 내 범위로 다운, 아이들 기존 60)*/
	const _double	fDuration = (_double)pAnim->Get_Duration() / (_double)60.f;

	/* 애니메이션의 프레임 개수 */
	const _uint		iCount = pAnim->Get_MaxFrameCount();

	/* 기존 애니메이션 총 재생시간을, 비트에 따른 총 재생시간으로 변환한다. */

	/* 애니메이션 재생에 필요한 비트 개수 */
	_uint			iBeats = fDuration / Get_SPB();

	/* 비트에 맞춘 전체 애니메이션 재생 시간 */
	const _double	fDurationByBeat = (_double)iBeats * Get_SPB();

	/* 프레임당 재생시간 */
	const _double	fTimePerFrame = fDurationByBeat / (_double)iCount;
	
	return fTimePerFrame;
}

void CBeatManager::Reset()
{
	m_bHit		= FALSE;

	m_iCurBpm	= ENGINE_INSTANCE->Get_BPM();

	m_dCurTime = 0;
}

void CBeatManager::Update_Beat(const _float fTimedelta)
{
	m_dCurTime += (_double)fTimedelta; // 0.016

	/* 비트 당 시간(초) */
	const _double fTimePerBeat = (_double)60.f / (_double)m_iCurBpm; // 0.44

	if (m_dCurTime >= fTimePerBeat)
	{
		m_dCurTime -= fTimePerBeat;
		m_bHit = TRUE;
	}
	else
		m_bHit = FALSE;
}

void CBeatManager::Free()
{
	__super::Free();
}

