#include "..\Default\stdafx.h"
#include "State_Blader_Attack.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CState_Blader_Attack::CState_Blader_Attack()
{
}

CState_Blader_Attack::CState_Blader_Attack(const CState_Blader_Attack& rhs)
{
}

HRESULT CState_Blader_Attack::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	m_pLeftArmCollider	= m_pBlader->Get_Child(CBlader::CHILD_TYPE::ARM_LEFT_BL)->Get_Collider_Sphere();
	m_pRightArmCollider = m_pBlader->Get_Child(CBlader::CHILD_TYPE::ARM_RIGHT_BL)->Get_Collider_Sphere();
	m_pBodyCollider		= m_pBlader->Get_Collider_Sphere();

	if (nullptr == m_pLeftArmCollider || nullptr == m_pRightArmCollider || nullptr == m_pBodyCollider)
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Blader_Attack::Enter()
{
	CAnimation* pAnimation = nullptr;
	_double		fTimePerFrame = 0.f;
	const _float fDistance = Get_Distance();

	if (fDistance <= m_fTargetMinDist) // 타겟 최소 허용 거리보다 가까움
	{
		pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::ATK_SIMPLE]);
		fTimePerFrame = pAnimation->Get_TickPerFrame() * 0.5f;
	}
	else
	{
		const _int iRand = rand() % 3;
	
		switch (iRand)
		{
		case 0 :
		{
			pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::ATK_JUMP_STRKIE_01_BL]);
			fTimePerFrame = pAnimation->Get_TickPerFrame() * 0.75f;

		}
		break;
		case 1 :
		{
			pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::ATK_COMBO_BL]);
			fTimePerFrame = pAnimation->Get_TickPerFrame() * 0.75f;

		}
		break;
		default:
		{
			pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::ATK_BLADE_BL]);
			fTimePerFrame = pAnimation->Get_TickPerFrame() * 0.75f;
		}
			break;
		}
	}

	if (nullptr == pAnimation)
		return E_FAIL;

	m_pModel->Set_Animation(pAnimation, fTimePerFrame, DF_TW_TIME);

	return S_OK;
}

const wstring CState_Blader_Attack::Tick(const _double& fTimeDelta)
{
	Activate_Collider();;

	return m_strName;
}

const wstring CState_Blader_Attack::LateTick()
{
	return Check_Transition();
}

void CState_Blader_Attack::Exit()
{
	m_pBodyCollider->Set_Active(TRUE);

	m_fTimeAcc = m_fTimeLimit = 0.f;
}

void CState_Blader_Attack::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	CCharacter*				pCharacter = dynamic_cast<CCharacter*>(pCollider->Get_Owner());
	const string&			strCurAnimName = m_pModel->Get_CurAnimation()->Get_Name();
	CCharacter::ATK_TYPE	eAtkType = CCharacter::ATK_TYPE::ATK_TYPE_END;

	if (nullptr != pCharacter && LayerNames[LAYER_ID::LAYER_PLAYER] == pCharacter->Get_LayerTag())
	{
		/* 블레이더 손 충돌 */
		if (CBlader::CHILD_TYPE::ARM_LEFT_BL == iIndexAsChild || CBlader::CHILD_TYPE::ARM_RIGHT_BL == iIndexAsChild)
		{
			if (strCurAnimName == AnimNames_BL[ANIM_BL::ATK_SIMPLE])
				eAtkType = CCharacter::ATK_TYPE::LIGHT;
			else if (strCurAnimName == AnimNames_BL[ANIM_BL::ATK_JUMP_STRKIE_01_BL])
				eAtkType = CCharacter::ATK_TYPE::LIGHT;
			else if (strCurAnimName == AnimNames_BL[ANIM_BL::ATK_COMBO_BL])
				eAtkType = CCharacter::ATK_TYPE::LIGHT;
		}
		
		if (-1 == iIndexAsChild) /* 블레이더 몸 충돌 */
		{
			if (strCurAnimName == AnimNames_BL[ANIM_BL::ATK_SIMPLE])
				eAtkType;
			else if (strCurAnimName == AnimNames_BL[ANIM_BL::ATK_JUMP_STRKIE_01_BL])
				eAtkType = CCharacter::ATK_TYPE::HEAVY;
			else if (strCurAnimName == AnimNames_BL[ANIM_BL::ATK_COMBO_BL])
				eAtkType = CCharacter::ATK_TYPE::HEAVY;
			else if (strCurAnimName == AnimNames_BL[ANIM_BL::ATK_BLADE_BL])
				eAtkType = CCharacter::ATK_TYPE::HEAVY;
		}

		if(CCharacter::ATK_TYPE::ATK_TYPE_END != eAtkType)
			pCharacter->Damaged(m_pBlader, eAtkType);
	}
}

void CState_Blader_Attack::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
}

void CState_Blader_Attack::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
}

const wstring CState_Blader_Attack::Check_Transition()
{
	if (m_pBlader->m_tFightDesc.bDamaged)
		return StateNames_BL[STATE_BL::STATE_DAMAGED_BL];

	const string strCurAnimName = m_pModel->Get_CurAnimation()->Get_Name();
	CModel::TweenDesc desc = m_pModel->Get_TweenDesc();

	if (!m_pModel->Is_Tween())
	{
		if (AnimNames_BL[ANIM_BL::ATK_SIMPLE] == strCurAnimName && 80 <= desc.cur.iCurFrame)
			return StateNames_BL[STATE_BL::STATE_IDLE_BL];

		if (AnimNames_BL[ANIM_BL::ATK_JUMP_STRKIE_01_BL] == strCurAnimName && 100 <= desc.cur.iCurFrame)
			return StateNames_BL[STATE_BL::STATE_IDLE_BL];

		if (AnimNames_BL[ANIM_BL::ATK_BLADE_BL] == strCurAnimName && 110 <= desc.cur.iCurFrame)
			return StateNames_BL[STATE_BL::STATE_IDLE_BL];

		if (AnimNames_BL[ANIM_BL::ATK_COMBO_BL] == strCurAnimName && 160 <= desc.cur.iCurFrame)
			return StateNames_BL[STATE_BL::STATE_IDLE_BL];
	}

	return m_strName;
}

HRESULT CState_Blader_Attack::Set_Animation()
{
	return S_OK;
}

const wstring CState_Blader_Attack::Choose_NextState()
{
	return wstring();
}

void CState_Blader_Attack::Activate_Collider()
{
	const _uint		iCurFrame = m_pModel->Get_TweenDesc().cur.iCurFrame;
	const string&	strCurAnimName = m_pModel->Get_CurAnimation()->Get_Name();

	if (strCurAnimName == AnimNames_BL[ANIM_BL::ATK_SIMPLE])
	{
		if (40 == iCurFrame)
		{
			m_pLeftArmCollider->Set_Active(TRUE);
			m_pRightArmCollider->Set_Active(TRUE);
		}

		if (60 == iCurFrame)
		{
			m_pLeftArmCollider->Set_Active(FALSE);
			m_pRightArmCollider->Set_Active(FALSE);
		}
	}
	else if (strCurAnimName == AnimNames_BL[ANIM_BL::ATK_JUMP_STRKIE_01_BL])
	{
		if (55 == iCurFrame)
		{
			m_pLeftArmCollider->Set_Active(TRUE);
			m_pRightArmCollider->Set_Active(TRUE);
		}
		if (65 == iCurFrame)
		{
			m_pLeftArmCollider->Set_Active(FALSE);
			m_pRightArmCollider->Set_Active(FALSE);

			m_pBodyCollider->Set_Active(TRUE);
		}
		if (90 == iCurFrame)
		{
			m_pBodyCollider->Set_Active(FALSE);
		}
	}
	else if (strCurAnimName == AnimNames_BL[ANIM_BL::ATK_COMBO_BL])
	{
		if (60 == iCurFrame)
		{
			m_pLeftArmCollider->Set_Active(TRUE);
			m_pRightArmCollider->Set_Active(TRUE);
		}

		if (120 == iCurFrame)
		{
			m_pLeftArmCollider->Set_Active(FALSE);
			m_pRightArmCollider->Set_Active(FALSE);

			m_pBodyCollider->Set_Active(TRUE);
		}

		if (150 == iCurFrame)
		{
			m_pBodyCollider->Set_Active(FALSE);
		}
	}
	else if (strCurAnimName == AnimNames_BL[ANIM_BL::ATK_BLADE_BL])
	{
		if (60 == iCurFrame)
		{
			m_pBodyCollider->Set_Active(TRUE);
		}

		if (110 == iCurFrame)
		{
			m_pBodyCollider->Set_Active(FALSE);
		}
	}
}

CState_Blader_Attack* CState_Blader_Attack::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Blader_Attack* pInstance = new CState_Blader_Attack();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Blader_Attack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Blader_Attack::Clone(void* pArg)
{
	return nullptr;
}

void CState_Blader_Attack::Free()
{
}
