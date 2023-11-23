#include "..\Default\stdafx.h"
#include "State_Peppermint_Gimmick.h"

#include "Camera_Follow.h"

#include "Peppermint_Bullet.h"

#include "Dynamic_Crane.h"

#include "UiManager.h"
#include "Ui.h"

CState_Peppermint_Gimmick::CState_Peppermint_Gimmick()
{
}

CState_Peppermint_Gimmick::CState_Peppermint_Gimmick(const CState_Peppermint_Gimmick& rhs)
{
}

HRESULT CState_Peppermint_Gimmick::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Peppermint_Gimmick::Enter()
{
	/* 페퍼민트 트랜스폼 설정 */
	Set_Transform();

	/* 카메라 보간 시작 */
	ENGINE_INSTANCE->Change_Camera(CAMERA_ID::CAM_PEPPERMINT_GIMMICK_AIM, 0.2f);

	/* UI */
	Set_UI(TRUE);

	/* Sound */
	Play_Sound();

	/* Animaion */
	CAnimation* pAnim = m_pModel->Get_Animation(AnimNames_PE[ANIM_PE::GIMMICK_AIM_PE]);
	{
		if (nullptr == pAnim) return E_FAIL;

		m_pModel->Clear_Animation();

		m_pModel->Set_Animation(pAnim, pAnim->Get_TickPerFrame() * 0.75f, DF_TW_TIME);
	}

	/* Progress */
	m_eProgressID = PROGRESS_ID::AIM;

	return S_OK;
}

const wstring CState_Peppermint_Gimmick::Tick(const _double& fTimeDelta)
{
	if (!CPlayerController::GetInstance()->Is_Controll(PLAYER_TYPE::PEPPERMINT))
		Exit();

	if (!ENGINE_INSTANCE->Is_LerpCam())
	{
		m_pPeppermint->Get_Transform()->Set_Look(ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_LOOK).ZeroY().Normalized());
	}

	Check_Progress(fTimeDelta);

	return m_strName;
}

const wstring CState_Peppermint_Gimmick::LateTick()
{
	if (!CPlayerController::GetInstance()->Is_Controll(PLAYER_TYPE::PEPPERMINT))
		Exit();

	return Check_Transition();
}

void CState_Peppermint_Gimmick::Exit()
{

	m_eProgressID = PROGRESS_ID::PROGRESS_END;
	
	m_fAcc = 0.f;

	CPlayerController::GetInstance()->SetOff_Player(PLAYER_TYPE::PEPPERMINT);

	/* UI */
	Set_UI(FALSE);

	/* 카메라 보간 시작 */
	{
		CGameObject*	pTarget	= ENGINE_INSTANCE->Get_GameObject_InCurLevel(LayerNames[LAYER_ID::LAYER_ENV_INTERACTALBE], L"Env_Dynamic_Crane_000");
		CDynamic_Crane* pCrane	= dynamic_cast<CDynamic_Crane*>(pTarget);

		if (nullptr == pCrane)
		{
			ENGINE_INSTANCE->Change_Camera(CAMERA_ID::CAM_FOLLOW, 0.4f);
	
			CCamera_Follow* pCam = dynamic_cast<CCamera_Follow*>(ENGINE_INSTANCE->Get_Camera(CAMERA_ID::CAM_FOLLOW));
			if (nullptr != pCam)
				pCam->Reset();
		}
		else
		{
			if (CDynamic_Crane::PROGRESS_TYPE::WAIT_CRANE != pCrane->Get_Progress()
				&& CDynamic_Crane::PROGRESS_TYPE::ACTIVE_CRANE != pCrane->Get_Progress())
			{
				ENGINE_INSTANCE->Change_Camera(CAMERA_ID::CAM_FOLLOW, 0.4f);

				CCamera_Follow* pCam = dynamic_cast<CCamera_Follow*>(ENGINE_INSTANCE->Get_Camera(CAMERA_ID::CAM_FOLLOW));
				if (nullptr != pCam)
					pCam->Reset();
			}

		}
	}
}

const wstring CState_Peppermint_Gimmick::Check_Transition()
{
	return m_strName;
}

void CState_Peppermint_Gimmick::Check_Progress(const _double& fTimeDelta)
{
	switch (m_eProgressID)
	{
	case AIM:
	{
		m_fAcc += fTimeDelta;
		if (m_fTimeLimit <= m_fAcc)
		{
			m_eProgressID = PROGRESS_ID::DISAPPEAR;

			CAnimation* pAnim = m_pModel->Get_Animation(AnimNames_PE[ANIM_PE::GIMMICK_DISAPPEAR_PE]);

			if (nullptr == pAnim) return;

			m_pModel->Set_Animation(pAnim, pAnim->Get_TickPerFrame() * 0.75f, DF_TW_TIME);
			
			/* UI */
			Set_UI(FALSE);

			ENGINE_INSTANCE->Play_Sound(EFC_PEPPERMINT_FAILED, CHANNEL_ID::PLAYER_PEPPERMINT, 0.8f);
		}

		if (ENGINE_INSTANCE->Key_Down(VK_LBUTTON))
		{
			Shoot();

			/* UI */
			Set_UI(FALSE);

			m_eProgressID = PROGRESS_ID::SHOOT;
		}
	}
	break;
	case SHOOT:
	{
		if (!m_pModel->Is_Tween())
		{
			CModel::TweenDesc tDesc = m_pModel->Get_TweenDesc();
			if (20 == tDesc.cur.iCurFrame)
			{
				m_eProgressID = PROGRESS_ID::DISAPPEAR;

				CAnimation* pAnim = m_pModel->Get_Animation(AnimNames_PE[ANIM_PE::GIMMICK_DISAPPEAR_PE]);

				if (nullptr == pAnim) return;

				m_pModel->Set_Animation(pAnim, pAnim->Get_TickPerFrame() * 0.75f, DF_TW_TIME);

				ENGINE_INSTANCE->Play_Sound(EFC_CHAI_JUMP, PLAYER_CHAI, EfcVolumeChai);
			}
		}
	}
		break;
	case DISAPPEAR:
	{
		if (!m_pModel->Is_Tween() && m_pModel->Is_TwoThirds_Animation())
		{
			Exit();
		}
	}
		break;
	default:
		break;
	}
}

void CState_Peppermint_Gimmick::Set_Transform()
{
	CTransform* pTransform_Peppermint = m_pPeppermint->Get_Transform();
	CTransform* pTrnasform_Chai = CPlayerController::GetInstance()->Get_Player(PLAYER_TYPE::CHAI)->Get_Transform();

	/* 네비 인덱스 설정 */
	//m_pPeppermint->Get_NavMeshAgent()->Set_CurIndex(CPlayerController::GetInstance()->Get_Player(PLAYER_TYPE::CHAI)->Get_NavMeshAgent()->Get_Index());

	/* 루트 포지션 초기화  */
	pTransform_Peppermint->Set_RootPos(Vec4::Zero, TRUE);

	/* 포지션 설정 (플레이어 기준 x축 -2떨어진 위치) */
	Vec4 vPlayerPos = pTrnasform_Chai->Get_FinalPosition();
	Vec4 vRelativePos = pTrnasform_Chai->Get_RelativePosition(vPeppermint_Gimmick_Relative_Pos).ZeroW();
	pTransform_Peppermint->Set_Position(vRelativePos + vPlayerPos, TRUE);

	/* 룩 설정 (카메라 포워드)*/
	Vec4 vCamDir = ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_LOOK).ZeroY().Normalized();

	pTransform_Peppermint->Set_Look(vCamDir);
}

void CState_Peppermint_Gimmick::Play_Sound()
{
	const _int iRand = rand() % 8;

	SOUND_FILE_ID eSoundID = SOUND_FILE_END;
	CHANNEL_ID	  eChannelID = CHANNEL_ID::ETC_PLAYER_CALL;

	switch (iRand)
	{
	case 0:
		eSoundID = SOUND_FILE_ID::EFC_PEPPERMINT_RESPAWN_00;
		break;
	case 1:
		eSoundID = SOUND_FILE_ID::EFC_PEPPERMINT_RESPAWN_01;
		break;
	case 2:
		eSoundID = SOUND_FILE_ID::EFC_PEPPERMINT_RESPAWN_02;
		break;
	case 3:
		eSoundID = SOUND_FILE_ID::EFC_PEPPERMINT_RESPAWN_03;
		break;
	case 4:
		eSoundID = SOUND_FILE_ID::EFC_PEPPERMINT_RESPAWN_04;
		break;
	case 5:
		eSoundID = SOUND_FILE_ID::EFC_PEPPERMINT_RESPAWN_05;
		break;
	case 6:
		eSoundID = SOUND_FILE_ID::EFC_PEPPERMINT_RESPAWN_06;
		break;
	case 7:
		eSoundID = SOUND_FILE_ID::EFC_PEPPERMINT_RESPAWN_07;
		break;
	default:
		break;
	}
	
	ENGINE_INSTANCE->Play_Sound(eSoundID, eChannelID, 0.8f);
}

HRESULT CState_Peppermint_Gimmick::Shoot()
{
	CAnimation* pAnim = m_pModel->Get_Animation(AnimNames_PE[ANIM_PE::GIMMICK_SHOOT_PE]);

	if (nullptr == pAnim) return E_FAIL;

	m_pModel->Set_Animation(pAnim, pAnim->Get_TickPerFrame() * 0.5f, 0.2f);

	// 사운드 재생 
	ENGINE_INSTANCE->Play_Sound(EFC_PEPPERMINT_SHOOT, PLAYER_PEPPERMINT, 0.8f);

	// 카메라 쉐이크 
	ENGINE_INSTANCE->Shake_Camera(0.1f, 10);

	// 총알 발사
	CGameObject* pClone = ENGINE_INSTANCE->Pop_Pool(ENGINE_INSTANCE->Get_CurLevelIndex(), L"Projectile_Peppermint_Bullet");

	if (nullptr == pClone) return E_FAIL;

	CProjectile* pProjectile = dynamic_cast<CProjectile*>(pClone);

	if (nullptr == pProjectile) return E_FAIL;

	CProjectile::PROJECTILE_DESC tDesc;
	{
		tDesc.pOwner = m_pPeppermint;

		tDesc.fSpeedPerSec = 60.f;

		CModel::BONE_TYPE		eBoneType = CModel::BONE_SOCKET_RIGHT;
		CPeppermint::CHILD_TYPE eChildType = CPeppermint::CHILD_TYPE::WP_RIGHT;

		Matrix matWorld = m_pPeppermint->Get_Model()->Get_SocketBoneMat(eBoneType) * m_pPeppermint->Get_Child((_uint)eChildType)->Get_Transform()->Get_FinalMat();

		tDesc.vDir = Vec4(matWorld.m[0]).Normalized();

		memcpy(&tDesc.vOrigin, matWorld.m[3], sizeof(Vec4));
	}

	return pProjectile->Shoot(tDesc);
}

void CState_Peppermint_Gimmick::Set_UI(const _bool& bActive)
{
	/* Aim */
	CUi* pAimUi = CUiManager::GetInstance()->Get_UI(UI_ID::UI_PEPPERMINT_AIM);

	if (nullptr == pAimUi) return;

	if(bActive)
		pAimUi->Set_State(CGameObject::OBJ_STATE::STATE_ACTIVE);
	else
		pAimUi->Set_State(CGameObject::OBJ_STATE::STATE_UNACTIVE);


	/* Target */
	//CUi* pTargetUi = CUiManager::GetInstance()->Get_UI(UI_ID::UI_PEPPERMINT_TARGET);

	//if (nullptr == pTargetUi) return;

	//if (bActive)
	//	pTargetUi->Set_State(CGameObject::OBJ_STATE::STATE_ACTIVE);
	//else
	//	pTargetUi->Set_State(CGameObject::OBJ_STATE::STATE_UNACTIVE);

}

CState_Peppermint_Gimmick* CState_Peppermint_Gimmick::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Peppermint_Gimmick* pInstance = new CState_Peppermint_Gimmick();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Peppermint_Gimmick");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Peppermint_Gimmick::Clone(void* pArg)
{
	return nullptr;
}

void CState_Peppermint_Gimmick::Free()
{
}
