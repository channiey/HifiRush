#include "..\Default\stdafx.h"
#include "State_Saber_Dead.h"

CState_Saber_Dead::CState_Saber_Dead()
{
}

CState_Saber_Dead::CState_Saber_Dead(const CState_Saber_Dead& rhs)
{
}

HRESULT CState_Saber_Dead::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Saber_Dead::Enter()
{
	ANIM_SA		eAnimID = ANIM_SA::DIE_HEAVY_01_SA;
	CAnimation* pAnimation = m_pModel->Get_Animation(eAnimID);
	_double		fTimePerFrame = 1 / pAnimation->Get_TickPerSecond();

	m_pModel->Set_Animation(eAnimID, fTimePerFrame, DF_TW_TIME);

	ENGINE_INSTANCE->Play_Sound(EFC_ENEMY_DAMAGED_00, ENEMY_SABER, 0.4f);


	return S_OK;
}

const wstring CState_Saber_Dead::Tick(const _double& fTimeDelta)
{
	return m_strName;
}

const wstring CState_Saber_Dead::LateTick()
{
	return Check_Transition();
}

void CState_Saber_Dead::Exit()
{

}

const wstring CState_Saber_Dead::Check_Transition()
{
	if (!m_pModel->Is_Tween() && m_pModel->Is_ThreeFourths_Animation())
	{
		if (m_pSaber->Is_Active())
		{
			ENGINE_INSTANCE->Play_Sound(EFC_CHAI_KILL, ENEMY_SABER, 0.4f);
			m_pSaber->Return_To_Pool();
		}
	}
	return m_strName;
}

CState_Saber_Dead* CState_Saber_Dead::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Saber_Dead* pInstance = new CState_Saber_Dead();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Saber_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Saber_Dead::Clone(void* pArg)
{
	return nullptr;
}

void CState_Saber_Dead::Free()
{
}
