#include "..\Default\stdafx.h"
#include "State_Saber_Attack.h"

CState_Saber_Attack::CState_Saber_Attack()
{
}

CState_Saber_Attack::CState_Saber_Attack(const CState_Saber_Attack& rhs)
{
}

HRESULT CState_Saber_Attack::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Saber_Attack::Enter()
{
	ANIM_SA			eAnimID = ANIM_SA::ATK_FLYINNG_SA;
	CAnimation*		pAnimation = m_pModel->Get_Animation(eAnimID);
	const _double	fTimePerFrame = CBeatManager::GetInstance()->Get_SPB(4) / (_double)55.f;

	m_pModel->Set_Animation(eAnimID, fTimePerFrame, DF_TW_TIME);

	return S_OK;
}

const wstring CState_Saber_Attack::Tick(const _double& fTimeDelta)
{
	return m_strName;
}

const wstring CState_Saber_Attack::LateTick()
{
	return Check_Transition();
}

void CState_Saber_Attack::Exit()
{
}

const wstring CState_Saber_Attack::Check_Transition()
{
	if (m_pModel->Is_Tween())
		return m_strName;

	if (m_pSaber->m_tFightDesc.bDamaged)
	{
		return StateNames_SA[STATE_DAMAGED_SA];
	}
	else
	{
		if(m_pModel->Is_ThreeFourths_Animation())
			return StateNames_SA[STATE_IDLE_SA];
	}

	return m_strName;
}

CState_Saber_Attack* CState_Saber_Attack::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Saber_Attack* pInstance = new CState_Saber_Attack();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Saber_Attack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Saber_Attack::Clone(void* pArg)
{
	return nullptr;
}

void CState_Saber_Attack::Free()
{
}
