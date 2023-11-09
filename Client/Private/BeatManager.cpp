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

HRESULT CBeatManager::Update(const _double fTimedelta)
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

const _double CBeatManager::Get_SPB(_uint iNumBeat)
{
	return(_double)60.f / (_double)ENGINE_INSTANCE->Get_BPM() * (_double)iNumBeat;
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
	m_iCurBpm	= ENGINE_INSTANCE->Get_BPM();
	
	m_bSteadyBeat = FALSE;
	m_bHalfBeat = FALSE;

	m_dCurTimeSteadyBeat	= 0.f;
	m_dCurTimeHalfBeat		= 0.f;
	m_dCurTimeInverseBeat	= -(Get_SPB() * 0.5f);
}

HRESULT CBeatManager::Add_StateEvent(const STATE_EVENT_DESC StateEvent)
{
	if (Is_Reserve_StateEvent(StateEvent.pStateMachine))
		return E_FAIL;

	m_StateEvents.push_back(StateEvent);

	return S_OK;
}

const _bool CBeatManager::Is_Reserve_StateEvent(CStateMachine* pStateMachine)
{
	if (nullptr == pStateMachine || m_StateEvents.empty())
		return FALSE;

	auto comp = [pStateMachine](STATE_EVENT_DESC StateEvent) { return (StateEvent.pStateMachine == pStateMachine) ? TRUE : FALSE; };
	auto iter = find_if(m_StateEvents.begin(), m_StateEvents.end(), comp);

	if (iter != m_StateEvents.end())
		return TRUE;

	return FALSE;
}

void CBeatManager::Update_Beat(const _double fTimedelta)
{
	/* Cur Time Update */
	m_dCurTimeSteadyBeat += fTimedelta;
	m_dCurTimeHalfBeat += fTimedelta;
	m_dCurTimeInverseBeat += fTimedelta;

	/* Down Beat */
	const _double dSecondPerDownBeat = Get_SPB();

	if (m_dCurTimeSteadyBeat >= dSecondPerDownBeat)
	{
		m_dCurTimeSteadyBeat -= dSecondPerDownBeat;
		m_bSteadyBeat = TRUE;
		m_bHalfBeat	= TRUE;
	}
	else
	{
		m_bSteadyBeat = FALSE;
	}

	/* Up Beat */
	const _double dSecondPerUpBeat = Get_SPB() * 0.5f;

	if (m_dCurTimeHalfBeat >= dSecondPerUpBeat)
	{
		m_dCurTimeHalfBeat -= dSecondPerUpBeat;
		m_bHalfBeat	= TRUE;
	}
	else
		m_bHalfBeat = FALSE;

	/* Inverse Beat */
	if (m_dCurTimeInverseBeat >= dSecondPerUpBeat)
	{
		m_dCurTimeInverseBeat -= dSecondPerDownBeat;
		m_bInverseBeat = TRUE;
	}
	else
	{
		m_bInverseBeat = FALSE;
	}

}

void CBeatManager::Free()
{
	__super::Free();
}

