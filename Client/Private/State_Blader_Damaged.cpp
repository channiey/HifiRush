#include "..\Default\stdafx.h"
#include "State_Blader_Damaged.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CState_Blader_Damaged::CState_Blader_Damaged()
{
}

CState_Blader_Damaged::CState_Blader_Damaged(const CState_Blader_Damaged& rhs)
{
}

HRESULT CState_Blader_Damaged::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Blader_Damaged::Enter()
{
	CAnimation* pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::IDLE_BL]);

	if (nullptr == pAnimation)
		return E_FAIL;

	_double	fTimePerFrame = 1.f / pAnimation->Get_TickPerSecond();

	m_pModel->Set_Animation(pAnimation, fTimePerFrame, DF_TW_TIME);

	return S_OK;
}

const wstring CState_Blader_Damaged::Tick(const _double& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	Look_Target();

	return m_strName;
}

const wstring CState_Blader_Damaged::LateTick()
{
	return Check_Transition();
}

void CState_Blader_Damaged::Exit()
{
	m_fTimeAcc = 0.f;
	m_fTimeLimit = 0.f;
}

const wstring CState_Blader_Damaged::Check_Transition()
{
	/*if (m_pSaber->m_tFightDesc.bDamaged)
	{
		return StateNames_SA[STATE_DAMAGED_SA];
	}

	if (m_pModel->Is_Tween() || m_fTimeLimit > m_fTimeAcc)
		return m_strName;

	const wstring strNextState = Choice_NextState();

	if (strNextState == m_strName)
	{
		Set_NextAnimation();

		return m_strName;
	}
	else
		return strNextState;*/

	return m_strName;
}

CState_Blader_Damaged* CState_Blader_Damaged::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Blader_Damaged* pInstance = new CState_Blader_Damaged();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Blader_Damaged");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Blader_Damaged::Clone(void* pArg)
{
	return nullptr;
}

void CState_Blader_Damaged::Free()
{
}
