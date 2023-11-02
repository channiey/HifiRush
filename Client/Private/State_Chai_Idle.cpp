#include "..\Default\stdafx.h"
#include "State_Chai_Idle.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CState_Chai_Idle::CState_Chai_Idle()
{
}

CState_Chai_Idle::CState_Chai_Idle(const CState_Chai_Idle& rhs)
{
}

HRESULT CState_Chai_Idle::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Chai_Idle::Enter()
{
	ANIM_CH			eAnimID = ANIM_CH::IDLE;
	CAnimation*		pAnimation = m_pChai->Get_Model()->Get_Animation(eAnimID);
	const _double	fTimePerFrame = CBeatManager::GetInstance()->Get_AnimTimePerFrame(pAnimation);

	m_pChai->Get_Model()->Set_Animation(eAnimID, fTimePerFrame * (_double)2.f, DF_TW_TIME);
	m_pChai->Get_Model()->Set_RootAnimation(FALSE);

	return S_OK;
}

const wstring& CState_Chai_Idle::Tick(const _double& fTimeDelta)
{

	return m_strName;
}

const wstring& CState_Chai_Idle::LateTick()
{

	return Check_Transition();
}

void CState_Chai_Idle::Exit()
{
}

const wstring& CState_Chai_Idle::Check_Transition()
{
	if(!CBeatManager::GetInstance()->Is_Hit())
		return m_strName;

	if (m_pChai->Get_Model()->Is_Tween())
		return m_strName;

	if (m_pChai->m_tFightDesc.bDamaged)
	{
		return StateNames[STATE_DAMAGED];
	}

	if (Input::Move())
	{
		if (m_pChai->m_tPhysicsDesc.bGround) 
		{
			return StateNames[STATE_RUN];
		}
	}
	else if (Input::Shift())
	{
		if (!m_pChai->m_tPhysicsDesc.bDash)
		{
			return StateNames[STATE_DASH];
		}
	}
	else if (Input::Attack()) 
	{
#ifdef _DEBUG
if(!CImGui_Manager::GetInstance()->Is_ClickedWindow())
	return StateNames[STATE_ATTACK];	
#else
	return StateNames[STATE_ATTACK];
#endif // _DEBUG
	}
	else if (Input::Parry())
	{
		return StateNames[STATE_PARRY];
	}
	else if (Input::Jump())
	{
		return StateNames[STATE_JUMP];
	}

	return m_strName;
}

CState_Chai_Idle* CState_Chai_Idle::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Chai_Idle* pInstance = new CState_Chai_Idle();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Chai_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Chai_Idle::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_Idle::Free()
{
}
