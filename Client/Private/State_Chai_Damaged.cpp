#include "..\Default\stdafx.h"
#include "State_Chai_Damaged.h"

#include "Enemy.h"

CState_Chai_Damaged::CState_Chai_Damaged()
{
}

CState_Chai_Damaged::CState_Chai_Damaged(const CState_Chai_Damaged& rhs)
{
}

HRESULT CState_Chai_Damaged::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Chai_Damaged::Enter()
{
	/* Damaged Data */
	CEnemy*					pEnemy = dynamic_cast<CEnemy*>(m_pChai->m_tFightDesc.pAttacker);
	CCharacter::ATK_TYPE	eAtkType = m_pChai->m_tFightDesc.eAtkType;

	if (nullptr == pEnemy) 
		return E_FAIL;

	/* Animation Data */
	ANIM_CH		eAnimID = ANIM_CH_END;
	CAnimation* pAnimation = nullptr;
	_double		fTimePerFrame = 0.f;

	/*switch (eAtkType)
	{
	case CCharacter::ATK_TYPE::LIGHT: 
	{
		eAnimID = ANIM_CH::DMG_LIGHT;
		pAnimation = m_pChai->Get_Model()->Get_Animation(eAnimID);
		fTimePerFrame = 1 / pAnimation->Get_TickPerSecond() * 0.5f;
	}
		break;
	case CCharacter::ATK_TYPE::HEAVY:
	{
		eAnimID = ANIM_CH::DMG_HEAVY;
		pAnimation = m_pChai->Get_Model()->Get_Animation(eAnimID);
		fTimePerFrame = 1 / pAnimation->Get_TickPerSecond();
	}
		break;
	case CCharacter::ATK_TYPE::FLY:
		break;
	case CCharacter::ATK_TYPE::DOWN:
		break;
	default:
	{
		eAnimID = ANIM_CH::DMG_LIGHT;
		pAnimation = m_pChai->Get_Model()->Get_Animation(eAnimID);
		fTimePerFrame = 1 / pAnimation->Get_TickPerSecond() * 0.5f;
	}
		break;
	}*/

	eAnimID = ANIM_CH::DMG_HEAVY;
	pAnimation = m_pChai->Get_Model()->Get_Animation(eAnimID);
	fTimePerFrame = 1 / pAnimation->Get_TickPerSecond();

	m_pChai->Get_Model()->Set_Animation(eAnimID, fTimePerFrame, DF_TW_TIME);

	/* Set Look */
	Vec4 vLook = pEnemy->Get_Transform()->Get_FinalPosition().ZeroY() - m_pChai->Get_Transform()->Get_FinalPosition().ZeroY();
	m_pChai->m_pTransformCom->Set_Look(vLook.Normalized());

	/* Play Sound */
	ENGINE_INSTANCE->Play_Sound(EFC_ENEMY_DAMAGED_00, PLAYER_CHAI, 0.4f);

	m_pChai->m_tFightDesc.bDamaged = FALSE;
	m_pChai->m_tFightDesc.pAttacker = nullptr;

	return S_OK;
}

const wstring CState_Chai_Damaged::Tick(const _double& fTimeDelta)
{
	/*if (m_pChai->m_tFightDesc.bDamaged)
	{
		Enter();
	}*/

	return m_strName;
}

const wstring CState_Chai_Damaged::LateTick()
{
	return Check_Transition();
}

void CState_Chai_Damaged::Exit()
{
	m_pChai->m_tFightDesc.bDamaged = FALSE;
	m_pChai->m_tFightDesc.pAttacker = nullptr;
}

const wstring CState_Chai_Damaged::Check_Transition()
{
	if (m_pModel->Is_Tween())
		return m_strName;

	const ANIM_CH eAnimID = (ANIM_CH)m_pModel->Get_CurAnimationIndex();


	if (ANIM_CH::DMG_LIGHT == eAnimID && m_pModel->Is_ThreeFourths_Animation())
	{
		return StateNames_CH[STATE_IDLE_CH];
	}
	else if (ANIM_CH::DMG_HEAVY == eAnimID && 57 <= m_pModel->Get_TweenDesc().cur.iCurFrame)
	{
		return StateNames_CH[STATE_IDLE_CH];
	}

	return m_strName;
}

CState_Chai_Damaged* CState_Chai_Damaged::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Chai_Damaged* pInstance = new CState_Chai_Damaged();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Chai_Damaged");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Chai_Damaged::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_Damaged::Free()
{
}
