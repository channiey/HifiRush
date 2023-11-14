#include "..\Default\stdafx.h"
#include "State_Blader_ParryEvent.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG
#include "Camera_Parry.h"

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
	m_pParryCam = dynamic_cast<CCamera_Parry*>(ENGINE_INSTANCE->Get_Camera(CAMERA_ID::CAM_PARRY));

	if (nullptr == m_pParryCam)
		return E_FAIL;

	CAnimation* pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::PARRY_EVENT_START_BL]);

	if (nullptr == pAnimation) return E_FAIL;

	m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame() * 0.5f, DF_TW_TIME);

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
	CAnimation* pAnimation			= m_pModel->Get_CurAnimation();
	CModel::TweenDesc desc			= m_pModel->Get_TweenDesc();
	const string strCurAnimName		= pAnimation->Get_Name();
	CCamera* pCurCameraCom			= ENGINE_INSTANCE->Get_CurCamera()->Get_Camera();

	if (PROGRESS_ID::PR_ZERO == m_eProgressID)
	{
		if (AnimNames_BL[ANIM_BL::PARRY_EVENT_ING_BL] == strCurAnimName && 10 == desc.cur.iCurFrame)
		{
			/* �÷��̾� ���� ��ġ ��� */
			Calculate_Player_Transform();

			/* �÷��̾� �� ���� */
			Set_Player_Look();

			/* �и� ī�޶� Ʈ������ ���� */
			Set_Camera_Transform();
			cout << "�и� ī�޶� Ʈ������ ����\n";

			/* ī�޶� ���� (�ȷο� -> �и�) */
			ENGINE_INSTANCE->Change_Camera(CAMERA_ID::CAM_PARRY, 0.3f, LERP_MODE::SMOOTHER_STEP);
			cout << "ī�޶� ���� ���� ���� \n";
			m_eProgressID = PR_SET_CAM;
		}

	}

	if (PROGRESS_ID::PR_SET_CAM == m_eProgressID)
	{
		if (!ENGINE_INSTANCE->Is_LerpCam())
		{
			cout << "ī�޶� ���� ���� ���� \n";
			/* ���� */
			Zoom_In();
			cout << "���� ���� \n";
			m_eProgressID = PROGRESS_ID::PR_ZOOM_IN;
		}
	}

	if (PROGRESS_ID::PR_ZOOM_IN == m_eProgressID)
	{
		if (!pCurCameraCom->Is_Lerp_Dist())
		{
			cout << "���� ���� \n";

			m_fTimeAcc += fTimeDelta;
			if (0.3f <= m_fTimeAcc)
			{
				/* �÷��̾� ������ ���� */
				Set_Player_Transform();
				m_eProgressID = PR_SET_PLAYER;

				cout << "�÷��̾� ������ ���� \n";
			}
		}
	}

	if (PROGRESS_ID::PR_SET_PLAYER == m_eProgressID)
	{
		if (45 == desc.cur.iCurFrame)
		{
			Zoom_Out();
			m_eProgressID = PROGRESS_ID::PR_SET_PLAYER;
		}
	}


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

	m_eProgressID = PR_ZERO;
}

const wstring CState_Blader_ParryEvent::Check_Transition()
{
	CAnimation* pAnimation = m_pModel->Get_CurAnimation();
	CModel::TweenDesc desc = m_pModel->Get_TweenDesc();

	const string strCurAnimName = pAnimation->Get_Name();

	if (-1 == desc.next.iAnimIndex)
	{
		if (AnimNames_BL[ANIM_BL::PARRY_EVENT_START_BL] == strCurAnimName && 30 == desc.cur.iCurFrame)
		{
			CAnimation* pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::PARRY_EVENT_ING_BL]);

			if(nullptr != pAnimation)
				m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame(), DF_TW_TIME);
		}
		else if (AnimNames_BL[ANIM_BL::PARRY_EVENT_ING_BL] == strCurAnimName && 185 == desc.cur.iCurFrame)
		{
			CAnimation* pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::PARRY_EVENT_FINISH_BL]);
		
			if (nullptr != pAnimation)
				m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame(), DF_TW_TIME, FALSE);
		}
		else if (AnimNames_BL[ANIM_BL::PARRY_EVENT_FINISH_BL] == strCurAnimName && 45 == desc.cur.iCurFrame)
		{
			ENGINE_INSTANCE->Change_Camera(CAMERA_ID::CAM_FOLLOW);
			return StateNames_BL[STATE_BL::STATE_IDLE_BL];
		}
	}

	return m_strName;
}

void CState_Blader_ParryEvent::Calculate_Player_Transform()
{
	Vec4 vLook = m_pBlader->Get_Transform()->Get_State(CTransform::STATE_LOOK);

	m_vPlayerNewPos = m_pBlader->Get_Transform()->Get_FinalPosition() + (vLook.Normalized() * m_fDistance);
}

void CState_Blader_ParryEvent::Set_Camera_Transform()
{
	m_pParryCam->Set_CamTransform(m_pBlader->Get_Transform(), m_pBlader->m_tFightDesc.pTarget->Get_Transform(), m_vPlayerNewPos);
}

void CState_Blader_ParryEvent::Set_Player_Look()
{
	Vec4 vDir = m_pBlader->Get_Transform()->Get_FinalPosition() - m_pBlader->m_tFightDesc.pTarget->Get_Transform()->Get_FinalPosition();

	m_pBlader->m_tFightDesc.pTarget->Get_Transform()->Set_Look(vDir.ZeroY().Normalized());
}

void CState_Blader_ParryEvent::Zoom_In()
{
	m_pParryCam->Zoom_In();
}

void CState_Blader_ParryEvent::Set_Player_Transform()
{
	m_pBlader->m_tFightDesc.pTarget->Get_Transform()->Set_Position(m_vPlayerNewPos);
}

void CState_Blader_ParryEvent::Zoom_Out()
{
	m_pParryCam->Zoom_Out();
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
