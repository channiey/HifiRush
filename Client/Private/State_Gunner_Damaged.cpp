#include "..\Default\stdafx.h"
#include "State_Gunner_Damaged.h"

#include "EffectManager.h"
#include "Effect.h"

CState_Gunner_Damaged::CState_Gunner_Damaged()
{
}

CState_Gunner_Damaged::CState_Gunner_Damaged(const CState_Gunner_Damaged& rhs)
{
}

HRESULT CState_Gunner_Damaged::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Gunner_Damaged::Enter()
{
	Damaged();

	return S_OK;
}

const wstring CState_Gunner_Damaged::Tick(const _double& fTimeDelta)
{
	if (m_pGunner->m_tFightDesc.bDamaged)
		Damaged();

	return m_strName;
}

const wstring CState_Gunner_Damaged::LateTick()
{
	return Check_Transition();
}

void CState_Gunner_Damaged::Exit()
{
	m_pGunner->m_tFightDesc.bDamaged = FALSE;
	m_pGunner->m_tFightDesc.pAttacker = nullptr;

	m_bPlayEffect = FALSE;
}

const wstring CState_Gunner_Damaged::Check_Transition()
{
	if (m_pModel->Is_Tween())
		return m_strName;

	if (m_pModel->Is_ThreeFourths_Animation())
		return StateNames_GU[STATE_IDLE_GU];

	return m_strName;
}

void CState_Gunner_Damaged::Damaged()
{
	PlayEffect();

	if (m_pGunner->m_tStatDesc.bDead)
	{
		m_pStateMachine->Set_State(StateNames_GU[STATE_DEAD_GU]);
		return;
	}

	CAnimation* pAnimation = nullptr;

	if (m_pGunner->m_tPhysicsDesc.bJump)
	{
		const _int iRand = rand() % 6;
		switch (iRand)
		{
		case 0 :
		{
			pAnimation = m_pModel->Get_Animation(AnimNames_GU[ANIM_GU::DMG_LIGHT_00_GU]);
		}
			break;
		case 1:
		{
			pAnimation = m_pModel->Get_Animation(AnimNames_GU[ANIM_GU::DMG_LIGHT_01_GU]);
		}
		break;
		case 2:
		{
			pAnimation = m_pModel->Get_Animation(AnimNames_GU[ANIM_GU::DMG_LIGHT_02_GU]);
		}
		break;
		case 3:
		{
			pAnimation = m_pModel->Get_Animation(AnimNames_GU[ANIM_GU::DMG_MIDDLE_00_GU]);
		}
		break;
		case 4:
		{
			pAnimation = m_pModel->Get_Animation(AnimNames_GU[ANIM_GU::DMG_MIDDLE_01_GU]);
		}
		break;
		case 5:
		{
			pAnimation = m_pModel->Get_Animation(AnimNames_GU[ANIM_GU::DMG_MIDDLE_02_GU]);
		}
		break;
		default:
			pAnimation = m_pModel->Get_Animation(AnimNames_GU[ANIM_GU::DMG_MIDDLE_00_GU]);
			break;
		}
	}
	else
	{
		pAnimation = m_pModel->Get_Animation(AnimNames_GU[ANIM_GU::DMG_MIDDLE_00_GU]);
	}


	m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame() * 0.75f);
	ENGINE_INSTANCE->Play_Sound(EFC_ENEMY_DAMAGED_00, ENEMY_GUNNER, 0.4f);


	m_pGunner->m_tFightDesc.bDamaged = FALSE;
	m_pGunner->m_tFightDesc.pAttacker = nullptr;
}

void CState_Gunner_Damaged::PlayEffect()
{
	CGameObject* pClone = ENGINE_INSTANCE->Pop_Pool(ENGINE_INSTANCE->Get_CurLevelIndex(), L"Effect_Damaged_Enemy");
	if (nullptr != pClone)
	{
		CEffect* pEffect = dynamic_cast<CEffect*>(pClone);
		if (nullptr != pEffect)
		{
			m_bPlayEffect = TRUE;

			Vec4 vPos = m_pGunner->Get_Transform()->Get_FinalPosition();
			vPos.y += 1.f;
			pEffect->Get_Transform()->Set_Position(vPos);
			pEffect->Start_Effect();
		}
	}
}

CState_Gunner_Damaged* CState_Gunner_Damaged::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Gunner_Damaged* pInstance = new CState_Gunner_Damaged();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Gunner_Damaged");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Gunner_Damaged::Clone(void* pArg)
{
	return nullptr;
}

void CState_Gunner_Damaged::Free()
{
}
