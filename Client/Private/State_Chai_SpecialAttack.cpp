#include "..\Default\stdafx.h"
#include "State_Chai_SpecialAttack.h"

#include "UiManager.h"
#include "Ui_SpecialAttack.h"

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
	if (FAILED(Set_UI()))
		return E_FAIL;

	CAnimation* pAnimation = nullptr;

	if (Input::LBtn())
	{
		m_eAtkType = SPC_ATK_TYPE::HIBIKI;
		pAnimation = m_pModel->Get_Animation(ANIM_CH::ATK_SPECIAL_00);
		m_pUI->Set_On(TRUE);

		ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_CHAI_HIBIKI, CHANNEL_ID::TALK_CHAI, 0.7f);
				
	}
	else if (Input::RBtn())
	{
		m_eAtkType = SPC_ATK_TYPE::POWER_CHORD;
		pAnimation = m_pModel->Get_Animation(ANIM_CH::ATK_HORIZONTAL_TOPBLADE_03);
		m_pUI->Set_On(FALSE);

		ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_CHAI_POWERCHORD, CHANNEL_ID::TALK_CHAI, 0.7f);
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

	Update_Sound();
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

	m_pChai->m_tChaiDesc.Clear_ReverbGuage();

	ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_CHAI_HIBIKI_FINISH_GUITAR, CHANNEL_ID::TALK_CHAI, 0.7f);


	m_bSoundHibiki = FALSE;
	m_bSoundPowerChord = FALSE;

	m_pUI->Set_Off();
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
		if (105 <= iCurFrame)
		{
			CGameObject* pChild = m_pChai->Get_Child(CChai::CHILD_TYPE::CH_WEAPON_RIGHT);

			if (pChild->Get_Collider_Sphere()->Is_Active())
			{
				pChild->Get_Collider_Sphere()->Set_Active(FALSE);
				ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_CHAI_HIBIHI_FINISH_BOOMB, CHANNEL_ID::PLAYER_CHAI, 0.8f);
			}
		}

		if(145 <= iCurFrame)
			return StateNames_CH[STATE_CH::STATE_IDLE_CH];
	}
		break;
	case CState_Chai_SpecialAttack::SPC_ATK_TYPE::POWER_CHORD:
	{
		if (80 <= iCurFrame)
		{
			CGameObject* pChild = m_pChai->Get_Child(CChai::CHILD_TYPE::CH_WEAPON_RIGHT);
			
			if (pChild->Get_Collider_Sphere()->Is_Active())
			{
				pChild->Get_Collider_Sphere()->Set_Active(FALSE);
				ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_CHAI_HIBIHI_FINISH_BOOMB, CHANNEL_ID::PLAYER_CHAI, 0.8f);
			}
		}

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
			ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_CHAI_HIBIKI_INTRO, CHANNEL_ID::PLAYER_CHAI, 0.8f);

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

void CState_Chai_SpecialAttack::Update_Sound()
{
	if (m_pModel->Is_Tween())
		return;

	const _int iCurFrame = m_pModel->Get_TweenDesc().cur.iCurFrame;

	if (SPC_ATK_TYPE::HIBIKI == m_eAtkType)
	{
		if (!m_bSoundHibiki && (15 == iCurFrame || 18 == iCurFrame || 22 == iCurFrame || 30 == iCurFrame || 35 == iCurFrame ||
			45 == iCurFrame || 49 == iCurFrame || 53 == iCurFrame || 55 == iCurFrame || 60 == iCurFrame))
		{
			ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_CHAI_ATTACK_00, CHANNEL_ID::PLAYER_CHAI, 0.5f);
			m_bSoundHibiki = TRUE;
		}
		else
			m_bSoundHibiki = FALSE;
	}
	else if (SPC_ATK_TYPE::POWER_CHORD == m_eAtkType)
	{
		if (!m_bSoundPowerChord && 30 == iCurFrame)
		{
			m_bSoundPowerChord = TRUE;
			ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_CHAI_POWERCHORD_GUITAR, CHANNEL_ID::PLAYER_CHAI, 0.8f);
		}
	}

	
}

HRESULT CState_Chai_SpecialAttack::Set_UI()
{
	if (nullptr == m_pUI)
	{
		CUi* pUI = CUiManager::GetInstance()->Get_UI(UI_ID::UI_SPECIALATTACK);
		
		if (nullptr == pUI)	return E_FAIL;
		
		m_pUI = dynamic_cast<CUi_SpecialAttack*>(pUI);

		if (nullptr == m_pUI)	return E_FAIL;
	}

	return S_OK;
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
