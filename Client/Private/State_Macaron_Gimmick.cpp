#include "..\Default\stdafx.h"
#include "State_Macaron_Gimmick.h"

#include "Camera_Follow.h"

#include "UiManager.h"

#include "Dynamic.h"

CState_Macaron_Gimmick::CState_Macaron_Gimmick()
{
}

CState_Macaron_Gimmick::CState_Macaron_Gimmick(const CState_Macaron_Gimmick& rhs)
{
}

HRESULT CState_Macaron_Gimmick::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Macaron_Gimmick::Enter()
{
	/* 트랜스폼 설정 */
	Set_Transform();

	/* UI */
	Set_UI(TRUE);

	/* Sound */
	Play_Sound();

	/* Animaion */
	CAnimation* pAnim = m_pModel->Get_Animation(AnimNames_MA[ANIM_MA::GIMMICK_APPEAR_MA]);
	{
		if (nullptr == pAnim) return E_FAIL;

		m_pModel->Clear_Animation();

		m_pModel->Set_Animation(pAnim, pAnim->Get_TickPerFrame() * 0.75f, DF_TW_TIME);
	}

	/* Progress */
	m_eProgressID = PROGRESS_ID::APPEAR;

	return S_OK;
}

const wstring CState_Macaron_Gimmick::Tick(const _double& fTimeDelta)
{
	if (!CPlayerController::GetInstance()->Is_Controll(PLAYER_TYPE::MACARON))
		Exit();

	Check_Progress(fTimeDelta);

	return m_strName;
}

const wstring CState_Macaron_Gimmick::LateTick()
{
	if (!CPlayerController::GetInstance()->Is_Controll(PLAYER_TYPE::MACARON))
		Exit();

	return Check_Transition();
}

void CState_Macaron_Gimmick::Exit()
{
	m_fAcc = 0.f;

	m_eProgressID = PROGRESS_ID::PROGRESS_END;

	CPlayerController::GetInstance()->SetOff_Player(PLAYER_TYPE::MACARON);

	Set_UI(FALSE);

	ENGINE_INSTANCE->Change_Camera(CAMERA_ID::CAM_FOLLOW);

	CCamera_Follow* pCam = dynamic_cast<CCamera_Follow*>(ENGINE_INSTANCE->Get_Camera(CAMERA_ID::CAM_FOLLOW));
	if (nullptr != pCam)
		pCam->Reset();

	if (RESULT_TYPE::SUCCESS == m_eResultType)
	{
		CUiManager::GetInstance()->On_Dialouge(2, L"언제든지 불러줘 차이!");
		ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_MACARON_SUCCESS, CHANNEL_ID::PLAYER_MACARON, 0.8f);

	}

	m_eResultType = RESULT_TYPE::NONE;
}

const wstring CState_Macaron_Gimmick::Check_Transition()
{
	return m_strName;
}

void CState_Macaron_Gimmick::Check_Progress(const _double& fTimeDelta)
{
	CModel::TweenDesc tDesc = m_pModel->Get_TweenDesc();

	switch (m_eProgressID)
	{
	case CState_Macaron_Gimmick::PROGRESS_ID::APPEAR: 
	{
		if (!m_pModel->Is_Tween() && 20 == tDesc.cur.iCurFrame)
		{
			CAnimation* pAnim = m_pModel->Get_Animation(AnimNames_MA[ANIM_MA::GIMMICK_TIMMING_MA]);
			{
				if (nullptr == pAnim) return;

				m_pModel->Set_Animation(pAnim, pAnim->Get_TickPerFrame(), DF_TW_TIME);

				ENGINE_INSTANCE->Change_Camera(CAMERA_ID::CAM_MACARON_GIMMICK_WALL);
			}

			m_eProgressID = CState_Macaron_Gimmick::PROGRESS_ID::TIMMING;
		}

	}
		break;
	case CState_Macaron_Gimmick::PROGRESS_ID::TIMMING:
	{
		if (!m_pModel->Is_Tween())
		{
			/* 타임 아웃 */
			m_fAcc += fTimeDelta;
			if (m_fTimeLimit <= m_fAcc)
			{
				CAnimation* pAnim = m_pModel->Get_Animation(AnimNames_MA[ANIM_MA::GIMMICK_FAILURE_DISAPPEAR_MA]);
				{
					if (nullptr == pAnim) return;

					m_pModel->Set_Animation(pAnim, pAnim->Get_TickPerFrame(), DF_TW_TIME);	
				}
				m_eProgressID = CState_Macaron_Gimmick::PROGRESS_ID::FAILURE_DISAPPEAR;

				m_eResultType = RESULT_TYPE::FAILURE;
			}

			/* 슛 */
			if (ENGINE_INSTANCE->Key_Down(VK_LBUTTON))
			{
				CAnimation* pAnim = m_pModel->Get_Animation(AnimNames_MA[ANIM_MA::GIMMICK_SUCCESS_DISAPPEAR_MA]);
				{
					if (nullptr == pAnim) return;

					m_pModel->Clear_Animation();

					m_pModel->Set_Animation(pAnim, pAnim->Get_TickPerFrame(), DF_TW_TIME);

					/* 카메라 FOV 보간 시작 */
					ENGINE_INSTANCE->Get_CurCamera()->Get_Camera()->Lerp_Fov(
						CamFov_MacaronGimmick * 0.85f,
						pAnim->Get_TickPerFrame() * 10 + DF_TW_TIME,
						LERP_MODE::SMOOTHER_STEP
					);

					m_eResultType = RESULT_TYPE::SUCCESS;
				}

				m_eProgressID = CState_Macaron_Gimmick::PROGRESS_ID::SUCCESS_DISAPPEAR;
			}
		}
	}
		break;
	case CState_Macaron_Gimmick::PROGRESS_ID::SUCCESS_DISAPPEAR:
	{
		if (!m_pModel->Is_Tween() && tDesc.cur.iCurFrame <= 10)
		{
			/* 카메라 회전 */
			CGameObject* pCam = ENGINE_INSTANCE->Get_CurCamera();
			if (nullptr != pCam)
			{
				pCam->Get_Transform()->Rotate(
					pCam->Get_Transform()->Get_State(CTransform::STATE_LOOK),
					DEG2RAD(1.f)
				);
			}
		}

		/* Boomb */
		if (!m_pModel->Is_Tween() && 18 == tDesc.cur.iCurFrame)
		{
			ENGINE_INSTANCE->Shake_Camera(0.7f, 15);

			CGameObject* pTarget = ENGINE_INSTANCE->Get_GameObject_InCurLevel(LayerNames[LAYER_ENV_INTERACTALBE], L"Env_Dynamic_Macaron_Wall_000");
			if (nullptr != pTarget)
			{
				if (Vec4(m_pMacaron->Get_Transform()->Get_FinalPosition() - pTarget->Get_Transform()->Get_FinalPosition()).OneW().Length() <= 5.f)
				{
					CDynamic* pDynamic = dynamic_cast<CDynamic*>(pTarget);
					if (nullptr != pDynamic)
					{
						pDynamic->Set_On();
						ENGINE_INSTANCE->Play_Sound(EFC_MACARON_BREAK_WALL, CHANNEL_ID::PLAYER_MACARON, 0.8f);
					}
				}
			}
		}

		if (!m_pModel->Is_Tween() && 40 == tDesc.cur.iCurFrame)
		{
			/* 카메라 FOV 보간 시작 */
			ENGINE_INSTANCE->Get_CurCamera()->Get_Camera()->Lerp_Fov(
				CamFov_MacaronGimmick,
				0.5f,
				LERP_MODE::SMOOTHER_STEP
			);
		}

		if (!m_pModel->Is_Tween() && 40 < tDesc.cur.iCurFrame && 45 < tDesc.cur.iCurFrame)
		{
			/* 카메라 회전 */
			CGameObject* pCam = ENGINE_INSTANCE->Get_CurCamera();
			if (nullptr != pCam)
			{
				pCam->Get_Transform()->Rotate(
					pCam->Get_Transform()->Get_State(CTransform::STATE_LOOK),
					DEG2RAD(-0.4f)
				);
			}
		}

		if (!m_pModel->Is_Tween() && 70 == tDesc.cur.iCurFrame)
		{
			Exit();
		}
	}
		break;
	case CState_Macaron_Gimmick::PROGRESS_ID::FAILURE_DISAPPEAR:
	{
		if (!m_pModel->Is_Tween() && 20 == tDesc.cur.iCurFrame)
		{
			CUiManager::GetInstance()->On_Dialouge(2, L"이런 부수지 못했군");
			ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_MACARON_FAILED, CHANNEL_ID::PLAYER_MACARON, 0.8f);
		}
		

		if (!m_pModel->Is_Tween() && 65 == tDesc.cur.iCurFrame)
		{
			Exit();
		}
	}
		break;
	default:
		break;
	}
}

void CState_Macaron_Gimmick::Set_Transform()
{
	CTransform* pTransform_Macaron = m_pMacaron->Get_Transform();
	CTransform* pTrnasform_Chai = CPlayerController::GetInstance()->Get_Player(PLAYER_TYPE::CHAI)->Get_Transform();

	/* 루트 포지션 초기화  */
	pTransform_Macaron->Set_RootPos(Vec4::Zero, TRUE);

	/* 포지션 설정 (플레이어 기준 x축 -2떨어진 위치) */
	Vec4 vPlayerPos = pTrnasform_Chai->Get_FinalPosition();
	Vec4 vRelativePos = pTrnasform_Chai->Get_RelativePosition(vMacaron_Gimmick_Relative_Pos).ZeroW();
	pTransform_Macaron->Set_Position(vRelativePos + vPlayerPos, TRUE);

	/* 룩 설정 (카메라 포워드)*/
	Vec4 vCamDir = ENGINE_INSTANCE->Get_CurCamera_State(CTransform::STATE_LOOK).ZeroY().Normalized();

	pTransform_Macaron->Set_Look(vCamDir);
}

void CState_Macaron_Gimmick::Play_Sound()
{

	SOUND_FILE_ID eSoundID		= SOUND_FILE_END;
	CHANNEL_ID	  eChannelID	= CHANNEL_ID::ETC_PLAYER_CALL;
	eSoundID = SOUND_FILE_ID::EFC_MACARON_RESPAWN_00;

	/*const _int iRand = rand() % 7;
	switch (iRand)
	{
	case 0:
		eSoundID = SOUND_FILE_ID::EFC_MACARON_RESPAWN_00;
		break;
	case 1:
		eSoundID = SOUND_FILE_ID::EFC_MACARON_RESPAWN_01;
		break;
	case 2:
		eSoundID = SOUND_FILE_ID::EFC_MACARON_RESPAWN_02;
		break;
	case 3:
		eSoundID = SOUND_FILE_ID::EFC_MACARON_RESPAWN_03;
		break;
	case 4:
		eSoundID = SOUND_FILE_ID::EFC_MACARON_RESPAWN_04;
		break;
	case 5:
		eSoundID = SOUND_FILE_ID::EFC_MACARON_RESPAWN_05;
		break;
	case 6:
		eSoundID = SOUND_FILE_ID::EFC_MACARON_RESPAWN_06;
		break;
	default:
		eSoundID = SOUND_FILE_ID::EFC_MACARON_RESPAWN_00;
		break;
	}*/

	ENGINE_INSTANCE->Play_Sound(eSoundID, eChannelID, 0.8f);
}

void CState_Macaron_Gimmick::Set_UI(const _bool& bActive)
{
}


CState_Macaron_Gimmick* CState_Macaron_Gimmick::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Macaron_Gimmick* pInstance = new CState_Macaron_Gimmick();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Macaron_Gimmick");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Macaron_Gimmick::Clone(void* pArg)
{
	return nullptr;
}

void CState_Macaron_Gimmick::Free()
{
}
