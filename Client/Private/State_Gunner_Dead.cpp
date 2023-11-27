#include "..\Default\stdafx.h"
#include "State_Gunner_Dead.h"

CState_Gunner_Dead::CState_Gunner_Dead()
{
}

CState_Gunner_Dead::CState_Gunner_Dead(const CState_Gunner_Dead& rhs)
{
}

HRESULT CState_Gunner_Dead::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Gunner_Dead::Enter()
{
	CAnimation* pAnimation = m_pModel->Get_Animation(AnimNames_GU[ANIM_GU::DEAD_GU]);
	_double		fTimePerFrame = 1 / pAnimation->Get_TickPerSecond();

	m_pModel->Set_Animation(pAnimation, fTimePerFrame, DF_TW_TIME);

	ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_ENEMY_DEAD_SPARK, CHANNEL_ID::ENEMY_GUNNER, 0.4f);

	return S_OK;
}

const wstring CState_Gunner_Dead::Tick(const _double& fTimeDelta)
{
	return m_strName;
}

const wstring CState_Gunner_Dead::LateTick()
{
	return Check_Transition();
}

void CState_Gunner_Dead::Exit()
{
	ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_ENEMY_DEAD_EXPLOSION, CHANNEL_ID::ENEMY_GUNNER, 0.4f);

}

const wstring CState_Gunner_Dead::Check_Transition()
{
	if (!m_pModel->Is_Tween() && m_pModel->Is_ThreeFourths_Animation())
	{
		if (m_pGunner->Is_Active())
		{
			//ENGINE_INSTANCE->Play_Sound(EFC_CHAI_KILL, ENEMY_SABER, 0.4f);
			m_pGunner->Return_To_Pool();
		}
	}
	return m_strName;
}

CState_Gunner_Dead* CState_Gunner_Dead::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Gunner_Dead* pInstance = new CState_Gunner_Dead();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Gunner_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Gunner_Dead::Clone(void* pArg)
{
	return nullptr;
}

void CState_Gunner_Dead::Free()
{
}
