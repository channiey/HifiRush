#include "..\Default\stdafx.h"
#include "State_Chai_Dash.h"

CState_Chai_Dash::CState_Chai_Dash()
{
}

CState_Chai_Dash::CState_Chai_Dash(const CState_Chai_Dash& rhs)
{
}

HRESULT CState_Chai_Dash::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Chai_Dash::Enter()
{
	ANIM_CH			eAnimID			= ANIM_CH::DASH;
	CAnimation*		pAnimation		= m_pChai->Get_Model()->Get_Animation(eAnimID);
	const _double	fTimePerFrame = (1 / pAnimation->Get_TickPerSecond()) * 0.5f;

	m_pChai->Get_Model()->Set_Animation(eAnimID, fTimePerFrame, DF_TW_TIME);

	m_pChai->Get_Rigidbody()->Add_Force(m_pChai->Get_Transform()->Get_Forward().xyz() * m_pChai->m_tPhysicsDesc.fDashPower, CRigidbody::FORCE_MODE::IMPULSE);
	ENGINE_INSTANCE->Play_Sound(EFC_CHAI_DASH, PLAYER_CHAI, EfcVolumeChai);

	m_pChai->m_tPhysicsDesc.bDash = TRUE;

	return S_OK;
}

const wstring CState_Chai_Dash::Tick(const _double& fTimeDelta)
{
	if(!m_pModel->Is_Tween() && m_pModel->Is_OneThirds_Animation()  && Input::Shift())
	{
		ANIM_CH			eAnimID = ANIM_CH::DASH;
		CAnimation* pAnimation = m_pChai->Get_Model()->Get_Animation(eAnimID);
		const _double	fTimePerFrame = (1 / pAnimation->Get_TickPerSecond()) * 0.5f;

		m_pChai->Get_Model()->Set_Animation(eAnimID, fTimePerFrame, DF_TW_TIME);

		m_pChai->Get_Rigidbody()->Add_Force(m_pChai->Get_Transform()->Get_Forward().xyz() * m_pChai->m_tPhysicsDesc.fDashPower, CRigidbody::FORCE_MODE::IMPULSE);
		ENGINE_INSTANCE->Play_Sound(EFC_CHAI_DASH, PLAYER_CHAI, EfcVolumeChai);

		return m_strName;
	}

	return m_strName;
}

const wstring CState_Chai_Dash::LateTick()
{
	return Check_Transition();
}

void CState_Chai_Dash::Exit()
{
	m_pChai->m_tPhysicsDesc.bDash = FALSE;

	m_pChai->Get_Rigidbody()->Clear_NetPower();
}

const wstring CState_Chai_Dash::Check_Transition()
{			
	if (m_pChai->Get_Model()->Is_Tween())
		return m_strName;

	if (m_pModel->Is_Half_Animation())
	{		
		return StateNames_CH[STATE_IDLE_CH];
	}

	return m_strName;
}

CState_Chai_Dash* CState_Chai_Dash::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Chai_Dash* pInstance = new CState_Chai_Dash();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Chai_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Chai_Dash::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_Dash::Free()
{
}
