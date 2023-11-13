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

	return S_OK;
}

HRESULT CState_Blader_Attack::Enter()
{
	CAnimation* pAnimation = nullptr;

	const _int iRand = rand() % 3;
	
	switch (iRand)
	{
	case 0 :
	{
		pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::ATK_JUMP_STRKIE_01_BL]);
	}
	break;
	case 1 :
	{
		pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::ATK_COMBO_BL]);
	}
	break;
	default:
	{
		pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::ATK_BLADE_BL]);
	}
		break;
	}
	if (nullptr == pAnimation)
		return E_FAIL;

	m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame() * 0.75f, DF_TW_TIME);

	return S_OK;
}

const wstring CState_Blader_Attack::Tick(const _double& fTimeDelta)
{
	return m_strName;
}

const wstring CState_Blader_Attack::LateTick()
{
	return Check_Transition();
}

void CState_Blader_Attack::Exit()
{
	m_fTimeAcc = 0.f;
	m_fTimeLimit = 0.f;
}

const wstring CState_Blader_Attack::Check_Transition()
{
	if (m_pBlader->m_tFightDesc.bDamaged)
		return StateNames_BL[STATE_BL::STATE_DAMAGED_BL];

	const string strCurAnimName = m_pModel->Get_CurAnimation()->Get_Name();
	CModel::TweenDesc desc = m_pModel->Get_TweenDesc();

	if (!m_pModel->Is_Tween())
	{
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
