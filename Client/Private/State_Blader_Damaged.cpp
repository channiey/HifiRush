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
	Damaged();

	return S_OK;
}

const wstring CState_Blader_Damaged::Tick(const _double& fTimeDelta)
{
	if (m_pBlader->m_tFightDesc.bDamaged)
	{
		Damaged();
	}

	return m_strName;
}

const wstring CState_Blader_Damaged::LateTick()
{
	return Check_Transition();
}

void CState_Blader_Damaged::Exit()
{
	m_pBlader->m_tFightDesc.bDamaged = FALSE;
	m_pBlader->m_tFightDesc.pAttacker = nullptr;
}

const wstring CState_Blader_Damaged::Check_Transition()
{
	if (m_pBlader->m_tFightDesc.bDamaged)
		return StateNames_BL[STATE_BL::STATE_DAMAGED_BL];

	if (!m_pModel->Is_Tween() && m_pModel->Is_ThreeFourths_Animation())
		return StateNames_BL[STATE_BL::STATE_IDLE_BL];

	return m_strName;
}

void CState_Blader_Damaged::Damaged()
{
	CCharacter*				pCharacter = dynamic_cast<CCharacter*>(m_pBlader->m_tFightDesc.pAttacker);
	CCharacter::ATK_TYPE	eAtkType = m_pBlader->m_tFightDesc.eAtkType;
	
	if (nullptr == pCharacter) 
		return;

	/* 패링 이벤트 판별 */
	if (m_pBlader->m_tStatDesc.fCurHp <= 50)
	{
		m_pStateMachine->Set_State(StateNames_BL[STATE_BL::STATE_PARRYEVENT_BL]);
		m_pBlader->m_tFightDesc.bDamaged = FALSE;
		m_pBlader->m_tFightDesc.pAttacker = nullptr;
		return;
	}

	/* 사망 판별 */
	if (m_pBlader->m_tStatDesc.bDead)
	{
		m_pStateMachine->Set_State(StateNames_BL[STATE_BL::STATE_DEAD_BL]);
		return;
	}

	/* Animation Data */
	CAnimation* pAnimation = nullptr;
	_double		fTimePerFrame = 0.f;
	_float		fTweenTime = DF_TW_TIME;

	switch (eAtkType)
	{
	case CCharacter::ATK_TYPE::LIGHT:
	{
		pAnimation		= m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::DMG_LIGHT_01_BL]);
		fTimePerFrame	= pAnimation->Get_TickPerFrame();
	}
		break;
	case CCharacter::ATK_TYPE::HEAVY:
	{
		pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::DMG_LIGHT_01_BL]);
		fTimePerFrame = pAnimation->Get_TickPerFrame();
	}
		break;
	case CCharacter::ATK_TYPE::FLY:
		break;
	case CCharacter::ATK_TYPE::DOWN:
		break;
	case CCharacter::ATK_TYPE::ATK_TYPE_END:
		break;
	default:
		break;
	}

	if (nullptr == pAnimation) return;

	m_pModel->Set_Animation(pAnimation, fTimePerFrame, DF_TW_TIME);

	m_pBlader->m_tFightDesc.bDamaged = FALSE;
	m_pBlader->m_tFightDesc.pAttacker = nullptr;
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
