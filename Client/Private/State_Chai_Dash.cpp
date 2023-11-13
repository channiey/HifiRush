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
	CTransform* pTransform = m_pChai->Get_Transform();
	Vec3 vScale = pTransform->Get_Scale();

	ANIM_CH			eAnimID			= ANIM_CH::DASH;
	CAnimation*		pAnimation		= m_pChai->Get_Model()->Get_Animation(eAnimID);
	const _double	fTimePerFrame = (1 / pAnimation->Get_TickPerSecond()) * 0.5f;

	m_pChai->Get_Model()->Set_Animation(eAnimID, fTimePerFrame, DF_TW_TIME);

	Vec4 vDir;
	if (Input::Up() && !Input::Left() && !Input::Right()) // Up
		vDir = ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_LOOK).ZeroY().Normalized();
	else if (Input::Down() && !Input::Left() && !Input::Right()) // Down
		vDir = ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_LOOK).ZeroY().Normalized() * -1.f;
	else if (Input::Left() && !Input::Up() && !Input::Down()) // Left 
		vDir = ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_RIGHT).ZeroY().Normalized() * -1.f;
	else if (Input::Right() && !Input::Up() && !Input::Down()) // Right 
		vDir = ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_RIGHT).ZeroY().Normalized();
	else if (Input::Up() && Input::Left() && !Input::Right()) // Up + Left
	{
		vDir = ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_LOOK).ZeroY()
			+ ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_RIGHT).ZeroY() * -1.f;

		vDir.Normalize();
	}
	else if (Input::Up() && !Input::Left() && Input::Right()) // Up + Right
	{
		vDir = ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_LOOK).ZeroY()
			+ ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_RIGHT).ZeroY();

		vDir.Normalize();
	}
	else if (Input::Down() && Input::Left() && !Input::Right()) // Down + Left
	{
		vDir = ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_LOOK).ZeroY() * -1.f
			+ ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_RIGHT).ZeroY() * -1.f;

		vDir.Normalize();
	}
	else if (Input::Down() && !Input::Left() && Input::Right()) // Down + Right
	{
		vDir = ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_LOOK).ZeroY() * -1.f
			+ ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_RIGHT).ZeroY();

		vDir.Normalize();
	}
	
	m_pChai->Get_Transform()->Set_Look(vDir);
	m_pChai->Get_Rigidbody()->Add_Force(m_pChai->Get_Transform()->Get_Forward().xyz() * m_pChai->m_tPhysicsDesc.fDashPower, CRigidbody::FORCE_MODE::IMPULSE);
	ENGINE_INSTANCE->Play_Sound(EFC_CHAI_DASH, PLAYER_CHAI, EfcVolumeChai);

	m_pChai->m_tPhysicsDesc.bDash = TRUE;

	if (Vec3::Zero == pTransform->Get_Scale())
		pTransform->Set_Scale(vScale);

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
