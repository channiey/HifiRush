#include "..\Default\stdafx.h"
#include "State_Chai_SpecialAttack.h"

CState_Chai_SpecialAttack::CState_Chai_SpecialAttack()
{
}

CState_Chai_SpecialAttack::CState_Chai_SpecialAttack(const CState_Chai_SpecialAttack& rhs)
{
}

HRESULT CState_Chai_SpecialAttack::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Chai_SpecialAttack::Enter()
{
	CAnimation* pAnimation = nullptr;

	if (Input::LBtn())
	{
		m_eAtkType = SPC_ATK_TYPE::HIBIKI;
		pAnimation = m_pModel->Get_Animation(ANIM_CH::ATK_SPECIAL_00);
	}
	else if (Input::RBtn())
	{
		m_eAtkType = SPC_ATK_TYPE::POWER_CHORD;
		pAnimation = m_pModel->Get_Animation(ANIM_CH::ATK_HORIZONTAL_TOPBLADE_03);
	}


	if (nullptr != pAnimation)
	{
		m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame());
		m_pChai->Get_Child(CChai::CHILD_TYPE::CH_WEAPON_RIGHT)->Get_Collider_Sphere()->Set_Active(TRUE);
	}
	else
		m_pStateMachine->Set_State(StateNames_CH[STATE_CH::STATE_IDLE_CH]);


	
	return S_OK;
}

const wstring CState_Chai_SpecialAttack::Tick(const _double& fTimeDelta)
{
	Update_Camera(fTimeDelta);
	
	return m_strName;
}

const wstring CState_Chai_SpecialAttack::LateTick()
{
	return Check_Transition();
}

void CState_Chai_SpecialAttack::Exit()
{
	m_pChai->Get_Child(CChai::CHILD_TYPE::CH_WEAPON_RIGHT)->Get_Collider_Sphere()->Set_Active(FALSE);

	m_eAtkType = SPC_ATK_TYPE::TYPEEND;

	m_pChai->Get_ChaiDesc().Clear_ReverbGuage();
}

const wstring CState_Chai_SpecialAttack::Check_Transition()
{
	if (m_pModel->Is_Tween())
		return m_strName;

	const _int iCurFrame = m_pModel->Get_TweenDesc().cur.iCurFrame;

	switch (m_eAtkType)
	{
	case CState_Chai_SpecialAttack::SPC_ATK_TYPE::HIBIKI:
	{
		if(120 == iCurFrame)
			m_pChai->Get_Child(CChai::CHILD_TYPE::CH_WEAPON_RIGHT)->Get_Collider_Sphere()->Set_Active(FALSE);

		if(145 <= iCurFrame)
			return StateNames_CH[STATE_CH::STATE_IDLE_CH];
	}
		break;
	case CState_Chai_SpecialAttack::SPC_ATK_TYPE::POWER_CHORD:
	{
		if (80 <= iCurFrame)
			m_pChai->Get_Child(CChai::CHILD_TYPE::CH_WEAPON_RIGHT)->Get_Collider_Sphere()->Set_Active(FALSE);

		if (115 <= iCurFrame)
			return StateNames_CH[STATE_CH::STATE_IDLE_CH];
	}	
		break;
	default:
		return StateNames_CH[STATE_CH::STATE_IDLE_CH];
		break;
	}

	return m_strName;
}

void CState_Chai_SpecialAttack::KnockBack(CCharacter* pTarget)
{
	/* 플레이어 바라보게 */
	Vec4 vLook = m_pChai->Get_Transform()->Get_FinalPosition() - pTarget->Get_Transform()->Get_FinalPosition();
	pTarget->Get_Transform()->Set_Look(vLook.ZeroY().Normalized());

	/* 넉백 힘 적용 */
	Vec3 vDir = pTarget->Get_Transform()->Get_FinalPosition().xyz() - m_pChai->Get_Transform()->Get_FinalPosition().xyz();

	Vec3 vForce = vDir.ZeroY().Normalized() * m_pChai->m_tPhysicsDesc.fKnockBackPower;
	if (2 == m_pChai->m_tFightDesc.iStep)
		vForce *= 10.f;

	pTarget->Get_Rigidbody()->Add_Force(vForce, CRigidbody::FORCE_MODE::IMPULSE);
}

void CState_Chai_SpecialAttack::Update_Camera(const _double& fTimeDelta)
{
	if (m_pModel->Is_Tween())
		return;

	const _int iCurFrame = m_pModel->Get_TweenDesc().cur.iCurFrame;

	CCamera* pCameraCom = ENGINE_INSTANCE->Get_Camera(CAMERA_ID::CAM_FOLLOW)->Get_Camera();

	switch (m_eAtkType)
	{
	case CState_Chai_SpecialAttack::SPC_ATK_TYPE::HIBIKI:
	{
		if (70 == iCurFrame)
		{
			const _float fLerpTime = m_pModel->Get_CurAnimation()->Get_SecondPerFrame() * 25.f;
			pCameraCom->Lerp_Fov(CamFov_Follow_Attack_Wide, fLerpTime, LERP_MODE::EASE_OUT);
			pCameraCom->Lerp_Dist(CamDist_Follow_Attack_Wide, fLerpTime, LERP_MODE::EASE_OUT);
		}

		if (95 == iCurFrame)
		{

			const _float fLerpTime = m_pModel->Get_CurAnimation()->Get_SecondPerFrame() * 10.f;

			pCameraCom->Lerp_Fov(CamFov_Follow_Default, fLerpTime, LERP_MODE::SMOOTHER_STEP);
			pCameraCom->Lerp_Dist(CamDist_Follow_Default, fLerpTime, LERP_MODE::EASE_OUT);
			//pCameraCom->Lerp_Fov(CamFov_Follow_Attack_Narrow, fLerpTime, LERP_MODE::EASE_IN);
		}

		//if (105 == iCurFrame)
			//ENGINE_INSTANCE->Shake_Camera(0.15f, 10);

		/*if (140 == iCurFrame)
		{
			pCameraCom->Lerp_Fov(CamFov_Follow_Default, 0.5f, LERP_MODE::SMOOTHER_STEP);
			pCameraCom->Lerp_Dist(CamDist_Follow_Default, 0.5f, LERP_MODE::EASE_OUT);
		}*/
	}
		break;
	case CState_Chai_SpecialAttack::SPC_ATK_TYPE::POWER_CHORD:
	{

		if (8 == iCurFrame)
		{
			const _float fLerpTime = m_pModel->Get_CurAnimation()->Get_SecondPerFrame() * 20.f;
			pCameraCom->Lerp_Fov(CamFov_Follow_Attack_Wide, fLerpTime, LERP_MODE::EASE_OUT);
			pCameraCom->Lerp_Dist(CamDist_Follow_Attack_Wide, fLerpTime, LERP_MODE::EASE_OUT);
		}

		//if (70 == iCurFrame)
		//{
		//	const _float fLerpTime = m_pModel->Get_CurAnimation()->Get_SecondPerFrame() * 10.f;
		//	//pCameraCom->Lerp_Fov(CamFov_Follow_Attack_Narrow, fLerpTime, LERP_MODE::EASE_IN);

		//	pCameraCom->Lerp_Fov(CamFov_Follow_Default, fLerpTime, LERP_MODE::SMOOTHER_STEP);
		//	pCameraCom->Lerp_Dist(CamDist_Follow_Default, fLerpTime, LERP_MODE::EASE_OUT);
		//}

		//if (80 <= iCurFrame)
			//ENGINE_INSTANCE->Shake_Camera(0.15f, 10);
	
		if (110 == iCurFrame)
		{
			pCameraCom->Lerp_Fov(CamFov_Follow_Default, 0.5f, LERP_MODE::SMOOTHER_STEP);
			pCameraCom->Lerp_Dist(CamDist_Follow_Default, 0.5f, LERP_MODE::EASE_OUT);

		}
	}
		break;
	default:
		break;
	}
}

CState_Chai_SpecialAttack* CState_Chai_SpecialAttack::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Chai_SpecialAttack* pInstance = new CState_Chai_SpecialAttack();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Chai_SpecialAttack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Chai_SpecialAttack::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_SpecialAttack::Free()
{
}
