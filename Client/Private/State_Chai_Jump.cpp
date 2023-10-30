#include "..\Default\stdafx.h"
#include "State_Chai_Jump.h"
#include "State_Chai_Run.h"
CState_Chai_Jump::CState_Chai_Jump()
{
}

CState_Chai_Jump::CState_Chai_Jump(const CState_Chai_Jump& rhs)
{
}

HRESULT CState_Chai_Jump::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Chai_Jump::Enter()
{
	m_pChai->Get_Model()->Set_RootAnimation(FALSE);
	m_pChai->Get_Model()->Set_Animation(ANIM_CH::JUMP, 3.f, 0.1f);

	m_pChai->m_tPhysicsDesc.bGround = FALSE;
	m_pChai->m_tPhysicsDesc.bJump = TRUE;

	m_pChai->Get_NavMeshAgent()->Set_AirState(TRUE);

	Jump();

	return S_OK;
}

const wstring& CState_Chai_Jump::Tick(const _float& fTimeDelta)
{
	Move(fTimeDelta);

	return m_strName;
}

const wstring& CState_Chai_Jump::LateTick()
{
	/* Run에서 Fall 체크 필요 */
	CModel* pModel = m_pChai->Get_Model();
	CModel::TweenDesc desc = pModel->Get_TweenDesc();

	/* Fall Speed */
	Set_FallSpeed();

	/* Land */
	{
		if (!m_pChai->m_tPhysicsDesc.bLanding && Check_Land())
		{
			m_pChai->m_tPhysicsDesc.bLanding = TRUE;
			return Check_Transition();
		}
	}
	
	/* Fall */
	if (!m_pChai->m_tPhysicsDesc.bFall && Check_Fall())
	{
		if (m_pChai->m_tPhysicsDesc.bJump)
			pModel->Set_Animation(ANIM_CH::FALL_JUMP);
		else if (m_pChai->m_tPhysicsDesc.bDoubleJump)
			pModel->Set_Animation(ANIM_CH::FALL_DOUBLE_JUMP);

		m_pChai->m_tPhysicsDesc.bFall = TRUE;
	}

	/* Double Jump */
	if (!m_pChai->m_tPhysicsDesc.bFall && Input::Jump())
	{
		if (m_pChai->m_tPhysicsDesc.bJump && !m_pChai->m_tPhysicsDesc.bDoubleJump)
		{
			m_pChai->m_tPhysicsDesc.bJump = FALSE;
			m_pChai->m_tPhysicsDesc.bDoubleJump = TRUE;
			m_pChai->m_tPhysicsDesc.bFall = FALSE;

			pModel->Set_Animation(ANIM_CH::DOUBLE_JUMP, DF_PL_TIME, 0.1f);

			Jump();
		}
	}

	return Check_Transition();
}

void CState_Chai_Jump::Exit()
{
	m_pChai->Get_Rigidbody()->Clear_NetPower();
	m_pChai->Get_Rigidbody()->Set_UseGravity(FALSE);

	m_pChai->Get_NavMeshAgent()->Set_AirState(FALSE);

	m_pChai->m_tPhysicsDesc.bJump		= FALSE;
	m_pChai->m_tPhysicsDesc.bDoubleJump = FALSE;
	m_pChai->m_tPhysicsDesc.bFall		= FALSE;
	m_pChai->m_tPhysicsDesc.bLanding	= FALSE;

	m_pChai->m_tPhysicsDesc.bGround		= TRUE;

	m_bSetFallSpeed_InJump				= FALSE;
	m_bSetFallSpeed_InDoubleJump		= FALSE;
}

const wstring& CState_Chai_Jump::Check_Transition()
{
	CModel* pModel = m_pChai->Get_Model();

	if (m_pChai->m_tPhysicsDesc.bLanding)
	{
		if(Input::Move())
			return StateNames[STATE_RUN];
		else
			return StateNames[STATE_IDLE];
	}

	return m_strName;
}


const _bool CState_Chai_Jump::Check_Land()
{
	const Vec3	 vPos = m_pChai->Get_Transform()->Get_FinalPosition().xyz();

	const _float fLandThreshold = 0.f;
	const _float fGroundHeight	= m_pChai->Get_NavMeshAgent()->Get_Height(vPos);

	if ((vPos.y - fGroundHeight) <= fLandThreshold)
	{
		m_pChai->Get_Transform()->Set_Position(Vec3(vPos.x, fGroundHeight, vPos.z));
		return TRUE;
	}

	return FALSE;
}

const _bool CState_Chai_Jump::Check_Fall()
{
	CModel* pModel = m_pChai->Get_Model();

	if (ANIM_CH::JUMP == pModel->Get_CurAnimationIndex() && -1 == pModel->Get_TweenDesc().next.iAnimIndex && pModel->Is_TwoThirds_Animation())
		return TRUE;
	else if (ANIM_CH::DOUBLE_JUMP == pModel->Get_CurAnimationIndex() && pModel->Is_TwoThirds_Animation())
		return TRUE;

	return FALSE;
}

void CState_Chai_Jump::Set_FallSpeed()
{
	CModel::TweenDesc desc = m_pChai->Get_Model()->Get_TweenDesc();

	if (!m_bSetFallSpeed_InJump)
	{
		if (ANIM_CH::JUMP == desc.cur.iAnimIndex && ANIM_CH::DOUBLE_JUMP != desc.next.iAnimIndex && m_pChai->Get_Model()->Is_Half_Animation())
		{
			m_pChai->Get_Rigidbody()->Set_Gravity(m_pChai->m_tPhysicsDesc.fFallGravity);
			m_bSetFallSpeed_InJump = TRUE;
		}
	}

	if (!m_bSetFallSpeed_InDoubleJump)
	{
		if (ANIM_CH::DOUBLE_JUMP == desc.cur.iAnimIndex && m_pChai->Get_Model()->Is_TwoThirds_Animation())
		{
			m_pChai->Get_Rigidbody()->Set_Gravity(m_pChai->m_tPhysicsDesc.fFallGravity * 2.f);
			m_bSetFallSpeed_InDoubleJump = TRUE;
		}
	}
}

void CState_Chai_Jump::Move(const _float& fTimeDelta)
{
	CTransform* pTranform = m_pChai->Get_Transform();

	Vec4 vDir, vRotDir, vLook;
	vLook = pTranform->Get_State(CTransform::STATE_LOOK).Normalized();

	const _float fRotConstMax = 66.f;
	const _float fRotConstNormal = 20.f;

	if (Input::Up() && !Input::Left() && !Input::Right()) // Up
	{
		vDir = ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_LOOK).ZeroY().Normalized();

		if (3.f < acos(vDir.Dot((vLook))))
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

void CState_Chai_Jump::Jump()
{
	//m_pChai->Get_Rigidbody()->Set_Kinematic(FALSE);

	m_pChai->Get_Rigidbody()->Clear_NetPower();
	m_pChai->Get_Rigidbody()->Set_UseGravity(TRUE);

	m_pChai->Get_Rigidbody()->Set_Gravity(m_pChai->m_tPhysicsDesc.fJumpGravity);
	m_pChai->Get_Rigidbody()->Add_Force(Vec3::Up * m_pChai->m_tPhysicsDesc.fJumpPower, CRigidbody::FORCE_MODE::IMPULSE);
}

CState_Chai_Jump* CState_Chai_Jump::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Chai_Jump* pInstance = new CState_Chai_Jump();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Chai_Jump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Chai_Jump::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_Jump::Free()
{
}
