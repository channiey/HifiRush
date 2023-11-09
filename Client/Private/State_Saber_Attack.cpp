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
	ANIM_SA			eAnimID = ANIM_SA_END;
	CAnimation*		pAnimation = nullptr;
	_double			fTimePerFrame = 0.f;

	if (0 == rand() % 2)
	{
		eAnimID = ANIM_SA::ATK_FLYINNG_SA;
		pAnimation = m_pModel->Get_Animation(eAnimID);
		fTimePerFrame = CBeatManager::GetInstance()->Get_SPB(4) / (_double)55.f;


		CModel::SoundEventDesc tSoundEventDesc;
		{
			tSoundEventDesc.eChannelID = ENEMY_SABER;
			tSoundEventDesc.eSoundID = EFC_SABER_SWING;
			tSoundEventDesc.fVolume = 0.8f;
			tSoundEventDesc.iFrame = 44;
		}
		m_pModel->Set_SoundEvent(tSoundEventDesc);

	}
	else
	{
		eAnimID = ANIM_SA::ATK_NONEFLYING_SA;
		pAnimation = m_pModel->Get_Animation(eAnimID);
		fTimePerFrame = CBeatManager::GetInstance()->Get_SPB(6) / (_double)85.f;

		CModel::SoundEventDesc tSoundEventDesc;
		{
			tSoundEventDesc.eChannelID = ENEMY_SABER;
			tSoundEventDesc.eSoundID = EFC_SABER_SWING;
			tSoundEventDesc.fVolume = 0.8f;
			tSoundEventDesc.iFrame = 60;
		}
		m_pModel->Set_SoundEvent(tSoundEventDesc);

	}
	
	m_pModel->Set_Animation(eAnimID, fTimePerFrame, DF_TW_TIME);

	m_pSaber->Get_Child(CSaber::CHILD_TYPE::SA_WEAPON_RIGHT)->Get_Collider_Sphere()->Set_Active(TRUE);

	ENGINE_INSTANCE->Play_Sound(EFC_SABER_CHARGE, ENEMY_SABER, 0.8f);

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
	m_pSaber->Get_Child(CSaber::CHILD_TYPE::SA_WEAPON_RIGHT)->Get_Collider_Sphere()->Set_Active(FALSE);
}

const wstring CState_Saber_Attack::Check_Transition()
{
	if (m_pSaber->m_tFightDesc.bDamaged)
		return StateNames_SA[STATE_DAMAGED_SA];

	if (m_pModel->Is_Tween())
		return m_strName;

	if (m_pModel->Is_ThreeFourths_Animation())
		return StateNames_SA[STATE_IDLE_SA];

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
