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

	/* �ִϸ��̼� �� ��� �ð� (��� ���ǻ�, 0 ~ 3�� �� ������ �ٿ�, ���̵� ���� 60)*/
	const _double	fDuration = (_double)pAnim->Get_Duration() / (_double)60.f;

	/* �ִϸ��̼��� ������ ���� */
	const _uint		iCount = pAnim->Get_MaxFrameCount();

	/* ���� �ִϸ��̼� �� ����ð���, ��Ʈ�� ���� �� ����ð����� ��ȯ�Ѵ�. */

	/* �ִϸ��̼� ����� �ʿ��� ��Ʈ ���� */
	_uint			iBeats = fDuration / Get_SPB();

	/* ��Ʈ�� ���� ��ü �ִϸ��̼� ��� �ð� */
	const _double	fDurationByBeat = (_double)iBeats * Get_SPB();

	/* �����Ӵ� ����ð� */
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

	/* ��Ʈ �� �ð�(��) */
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

