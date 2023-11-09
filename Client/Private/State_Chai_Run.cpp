#include "..\Default\stdafx.h"
#include "State_Chai_Run.h"

CState_Chai_Run::CState_Chai_Run()
{
}

CState_Chai_Run::CState_Chai_Run(const CState_Chai_Run& rhs)
{
}

HRESULT CState_Chai_Run::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Chai_Run::Enter()
{
	ANIM_CH			eAnimID			= ANIM_CH::RUN;
	CAnimation*		pAnimation		= m_pChai->Get_Model()->Get_Animation(eAnimID);
	const _double	fTimePerFrame	= CBeatManager::GetInstance()->Get_AnimTimePerFrame(pAnimation);

	m_pChai->Get_Model()->Set_Animation(eAnimID, (_double)fTimePerFrame * (_double)2.f, DF_TW_TIME);

	return S_OK;
}

const wstring CState_Chai_Run::Tick(const _double& fTimeDelta)
{
	Move(fTimeDelta);

	return m_strName;
}

const wstring CState_Chai_Run::LateTick()
{
	return Check_Transition();
}

void CState_Chai_Run::Exit()
{
}

const wstring CState_Chai_Run::Check_Transition()
{
	if (m_pChai->m_tFightDesc.bDamaged)
		return StateNames_CH[STATE_IDLE_CH];

	/* Movement */
	if (Input::Jump())
	{
		if (!m_pChai->m_tPhysicsDesc.bJump)
			return StateNames_CH[STATE_JUMP_CH];
	}
	else if (Input::Shift())
	{
		if (!m_pChai->m_tPhysicsDesc.bDash)
		{
			return StateNames_CH[STATE_DASH_CH];
		}
	}
	else if (Input::Attack())
	{
		return StateNames_CH[STATE_ATTACK_CH];
	}
	else if (Input::Parry())
	{
		if (m_pChai->m_tPhysicsDesc.bGround) 
		{
			return StateNames_CH[STATE_PARRY_CH];
		}
	}
	else if (Input::Move())
	{
		if (m_pChai->m_tPhysicsDesc.bGround)
		{
			return StateNames_CH[STATE_RUN_CH];
		}
	}



	if (CBeatManager::GetInstance()->Is_HalfBeat())
		return StateNames_CH[STATE_IDLE_CH];
	else
		return StateNames_CH[STATE_RUN_CH];
}

void CState_Chai_Run::Move(const _double& fTimeDelta)
{
	CTransform* pTranform = m_pChai->Get_Transform();

	Vec4 vDir, vRotDir, vLook;
	vLook = pTranform->Get_State(CTransform::STATE_LOOK).Normalized();

	const _float fRotConstMax = 66.f;
	const _float fRotConstNormal = 20.f;

	if (Input::Up() && !Input::Left() && !Input::Right()) // Up
	{
		vDir = ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_LOOK).ZeroY().Normalized();
		
		if(3.f < acos(vDir.Dot((vLook))))
			vRotDir = Vec4::Lerp(vLook, vDir, fRotConstMax);
		else
			vRotDir = Vec4::Lerp(vLook, vDir, fTimeDelta * fRotConstNormal);

		pTranform->Set_Look(vRotDir);
		pTranform->Translate(vDir * m_pChai->m_tPhysicsDesc.fMaxForwardSpeed * fTimeDelta);
	}
	else if (Input::Down() && !Input::Left() && !Input::Right()) // Down
	{
		vDir = ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_LOOK).ZeroY().Normalized().Inverse();

		if (3.f < acos(vDir.Dot((vLook))))
			vRotDir = Vec4::Lerp(vLook, vDir, fRotConstMax);
		else
			vRotDir = Vec4::Lerp(vLook, vDir, fTimeDelta * fRotConstNormal);

		pTranform->Set_Look(vRotDir);
		pTranform->Translate(vDir * m_pChai->m_tPhysicsDesc.fMaxForwardSpeed * fTimeDelta);
	}
	else if (Input::Left() && !Input::Up() && !Input::Down()) // Left 
	{
		vDir = ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_RIGHT).ZeroY().Normalized().Inverse();

		if (3.f < acos(vDir.Dot((vLook))))
			vRotDir = Vec4::Lerp(vLook, vDir, fRotConstMax);
		else
			vRotDir = Vec4::Lerp(vLook, vDir, fTimeDelta * fRotConstNormal);

		pTranform->Set_Look(vRotDir);
		pTranform->Translate(vDir * m_pChai->m_tPhysicsDesc.fMaxForwardSpeed * fTimeDelta);
	}
	else if (Input::Right() && !Input::Up() && !Input::Down()) // Right 
	{
		vDir = ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_RIGHT).ZeroY().Normalized();

		if (3.f < acos(vDir.Dot((vLook))))
			vRotDir = Vec4::Lerp(vLook, vDir, fRotConstMax);
		else
			vRotDir = Vec4::Lerp(vLook, vDir, fTimeDelta * fRotConstNormal);

		pTranform->Set_Look(vRotDir);
		pTranform->Translate(vDir * m_pChai->m_tPhysicsDesc.fMaxForwardSpeed * fTimeDelta);
	}
	else if (Input::Up() && Input::Left() && !Input::Right()) // Up + Left
	{
		vDir = ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_LOOK).ZeroY()
			+ ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_RIGHT).ZeroY().Inverse();

		vDir.Normalize();
		vRotDir = Vec4::Lerp(vLook, vDir, fTimeDelta * fRotConstNormal);
		pTranform->Set_Look(vRotDir);
		pTranform->Translate(vDir * m_pChai->m_tPhysicsDesc.fMaxForwardSpeed * fTimeDelta);
	}
	else if (Input::Up() && !Input::Left() && Input::Right()) // Up + Right
	{
		vDir = ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_LOOK).ZeroY()
			+ ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_RIGHT).ZeroY();

		vDir.Normalize();

		vRotDir = Vec4::Lerp(vLook, vDir, fTimeDelta * fRotConstNormal);
		pTranform->Set_Look(vRotDir);
		pTranform->Translate(vDir * m_pChai->m_tPhysicsDesc.fMaxForwardSpeed * fTimeDelta);
	}
	else if (Input::Down() && Input::Left() && !Input::Right()) // Down + Left
	{
		vDir = ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_LOOK).ZeroY().Inverse()
			+ ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_RIGHT).ZeroY().Inverse();

		vDir.Normalize();
		vRotDir = Vec4::Lerp(vLook, vDir, fTimeDelta * fRotConstNormal);
		pTranform->Set_Look(vRotDir);
		pTranform->Translate(vDir * m_pChai->m_tPhysicsDesc.fMaxForwardSpeed * fTimeDelta);
	}
	else if (Input::Down() && !Input::Left() && Input::Right()) // Down + Right
	{
		vDir = ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_LOOK).ZeroY().Inverse()
			+ ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_RIGHT).ZeroY();

		vDir.Normalize();
		vRotDir = Vec4::Lerp(vLook, vDir, fTimeDelta * fRotConstNormal);
		pTranform->Set_Look(vRotDir);
		pTranform->Translate(vDir * m_pChai->m_tPhysicsDesc.fMaxForwardSpeed * fTimeDelta);
	}

	if (Vec3::Zero == pTranform->Get_Scale())
		pTranform->Set_Scale(m_vScale);
}

CState_Chai_Run* CState_Chai_Run::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Chai_Run* pInstance = new CState_Chai_Run();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Chai_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Chai_Run::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_Run::Free()
{
}
