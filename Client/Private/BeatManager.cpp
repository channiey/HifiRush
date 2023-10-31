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

	/* �ִϸ��̼� �� ��� �ð� */
	const _float fDuration = pAnim->Get_Duration();

	/* �ִϸ��̼��� ������ ���� */
	const _uint iCount = (_float)pAnim->Get_MaxFrameCount();

	/* �ִϸ��̼� ����� �ʿ��� ��Ʈ ���� */
	_uint	iBeats = fDuration / Get_BPS();

	/* ��Ʈ�� �ִϸ��̼� ��� �ð� */
	_double	fTimePerBeat = fDuration / iBeats;

	/* �����Ӵ� ����ð� */
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
