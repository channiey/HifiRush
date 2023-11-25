#include "..\Default\stdafx.h"
#include "State_Gunner_Idle.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CState_Gunner_Idle::CState_Gunner_Idle()
{
}

CState_Gunner_Idle::CState_Gunner_Idle(const CState_Gunner_Idle& rhs)
{
}

HRESULT CState_Gunner_Idle::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Gunner_Idle::Enter()
{
	srand(time(NULL));

	CAnimation*		pAnimation = m_pModel->Get_Animation(AnimNames_GU[ANIM_GU::IDLE_GU]);

	m_fTimeLimit = (rand() % 10) * 0.1f + 1.f;

	m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame(), DF_TW_TIME);

	return S_OK;
}

const wstring CState_Gunner_Idle::Tick(const _double& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	Look_Target();

	return m_strName;
}

const wstring CState_Gunner_Idle::LateTick()
{
	return Check_Transition();
}

void CState_Gunner_Idle::Exit()
{
	m_fTimeAcc = 0.f;
	m_fTimeLimit = 0.f;
}

const wstring CState_Gunner_Idle::Check_Transition()
{
	if (m_pGunner->m_tFightDesc.bDamaged)
		return StateNames_GU[STATE_DAMAGED_GU];

	if (!m_pModel->Is_Tween() && m_fTimeLimit <= m_fTimeAcc)
	{
		const wstring strNextState = Choice_NextState();

		if (strNextState == m_strName)
			return m_strName;
		else
			return strNextState;
	}

	return m_strName;
}

CState_Gunner_Idle* CState_Gunner_Idle::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Gunner_Idle* pInstance = new CState_Gunner_Idle();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Gunner_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Gunner_Idle::Clone(void* pArg)
{
	return nullptr;
}

void CState_Gunner_Idle::Free()
{
}
