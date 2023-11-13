#include "..\Default\stdafx.h"
#include "State_Blader_Move.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CState_Blader_Move::CState_Blader_Move()
{
}

CState_Blader_Move::CState_Blader_Move(const CState_Blader_Move& rhs)
{
}

HRESULT CState_Blader_Move::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Blader_Move::Enter()
{
	if (FAILED(Set_Animation()))
		return E_FAIL;

	return S_OK;
}

const wstring CState_Blader_Move::Tick(const _double& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	return m_strName;
}

const wstring CState_Blader_Move::LateTick()
{
	return Check_Transition();
}

void CState_Blader_Move::Exit()
{
	m_fTimeAcc = 0.f;
	m_fTimeLimit = 0.f;
}

const wstring CState_Blader_Move::Check_Transition()
{
	if (m_pBlader->m_tFightDesc.bDamaged)
		return StateNames_BL[STATE_BL::STATE_DAMAGED_BL];

	if (!m_pModel->Is_Tween() && m_pModel->Is_ThreeFourths_Animation())
		return StateNames_BL[STATE_BL::STATE_IDLE_BL];

	return m_strName;
}

HRESULT CState_Blader_Move::Set_Animation()
{
	// RUN / ESCAPE / WALK

	CAnimation* pAnimation		= nullptr;
	_double		fTimePerFrame	= 0.f;
	_float		fTweenTime		= DF_TW_TIME;

	const _float fDistance = Get_Distance();

	if (fDistance >= m_fTargetMaxDist) // 타겟 최대 허용 거리보다 멂
	{
		pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::ESCAPE_FRONT_BL]);
		fTimePerFrame = pAnimation->Get_TickPerFrame();

	}
	else if (fDistance <= m_fTargetMinDist) // 타겟 최소 허용 거리보다 가까움
	{
		pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::ESCAPE_BACK_BL]);
		fTimePerFrame = pAnimation->Get_TickPerFrame();
	}
	else
	{
		const _int iRand = rand() % 2;

		if (0 == iRand)
		{
			pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::ESCAPE_RIGHT_BL]);
			fTimePerFrame = pAnimation->Get_TickPerFrame();
		}
		else
		{
			pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::ESCAPE_LEFT_BL]);
			fTimePerFrame = pAnimation->Get_TickPerFrame();
		}
	}

	m_pModel->Set_Animation(pAnimation, fTimePerFrame, fTweenTime);

	return S_OK;
}

CState_Blader_Move* CState_Blader_Move::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Blader_Move* pInstance = new CState_Blader_Move();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Blader_Move");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Blader_Move::Clone(void* pArg)
{
	return nullptr;
}

void CState_Blader_Move::Free()
{
}
