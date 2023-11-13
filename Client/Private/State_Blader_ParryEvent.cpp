#include "..\Default\stdafx.h"
#include "State_Blader_ParryEvent.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CState_Blader_ParryEvent::CState_Blader_ParryEvent()
{
}

CState_Blader_ParryEvent::CState_Blader_ParryEvent(const CState_Blader_ParryEvent& rhs)
{
}

HRESULT CState_Blader_ParryEvent::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Blader_ParryEvent::Enter()
{
	CAnimation* pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::PARRY_EVENT_START_BL]);

	if (nullptr == pAnimation) return E_FAIL;

	m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame(), DF_TW_TIME);

	/* Set Look */
	{
		Vec4 vDir = m_pBlader->m_tFightDesc.pTarget->Get_Transform()->Get_FinalPosition()
					- m_pBlader->Get_Transform()->Get_FinalPosition();

		m_pBlader->Get_Transform()->Set_Look(vDir.ZeroY().Normalized());
	}

	return S_OK;
}

const wstring CState_Blader_ParryEvent::Tick(const _double& fTimeDelta)
{

	return m_strName;
}

const wstring CState_Blader_ParryEvent::LateTick()
{
	return Check_Transition();
}

void CState_Blader_ParryEvent::Exit()
{
	m_fTimeAcc = 0.f;
	m_fTimeLimit = 0.f;
}

const wstring CState_Blader_ParryEvent::Check_Transition()
{
	CAnimation* pAnimation = m_pModel->Get_CurAnimation();
	CModel::TweenDesc desc = m_pModel->Get_TweenDesc();

	const string strCurAnimName = pAnimation->Get_Name();

	if (-1 == desc.next.iAnimIndex)
	{
		if (AnimNames_BL[ANIM_BL::PARRY_EVENT_START_BL] == strCurAnimName &&
			55 == desc.cur.iCurFrame)
		{
			CAnimation* pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::PARRY_EVENT_ING_BL]);

			if(nullptr != pAnimation)
				m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame(), DF_TW_TIME);
		}
		else if (AnimNames_BL[ANIM_BL::PARRY_EVENT_ING_BL] == strCurAnimName &&
			185 == desc.cur.iCurFrame)
		{
			CAnimation* pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::PARRY_EVENT_FINISH_BL]);
		
			if (nullptr != pAnimation)
				m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame(), DF_TW_TIME);
		}
		else if (AnimNames_BL[ANIM_BL::PARRY_EVENT_FINISH_BL] == strCurAnimName &&
			45 == desc.cur.iCurFrame)
		{
			return StateNames_BL[STATE_BL::STATE_IDLE_BL];
		}
	}

	return m_strName;
}

CState_Blader_ParryEvent* CState_Blader_ParryEvent::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Blader_ParryEvent* pInstance = new CState_Blader_ParryEvent();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Blader_ParryEvent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Blader_ParryEvent::Clone(void* pArg)
{
	return nullptr;
}

void CState_Blader_ParryEvent::Free()
{
}
