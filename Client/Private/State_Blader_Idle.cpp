#include "..\Default\stdafx.h"
#include "State_Blader_Idle.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CState_Blader_Idle::CState_Blader_Idle()
{
}

CState_Blader_Idle::CState_Blader_Idle(const CState_Blader_Idle& rhs)
{
}

HRESULT CState_Blader_Idle::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Blader_Idle::Enter()
{
	/* 최초 입장 */
	CAnimation* pAnimation		= nullptr;
	_double		fTimePerFrame	= 0.f;
	_float		fTweenTime		= DF_TW_TIME;

	if (StateNames_BL[STATE_BL::STATE_APPEAR_BL] == m_pStateMachine->Get_PrevState()->Get_Name())
	{
		pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::IDLE_BL]);
	
		if (nullptr == pAnimation)
			return E_FAIL;
		
		fTimePerFrame = pAnimation->Get_TickPerFrame();

		m_fTimeLimit = 2.f + ((rand() % 5) * 0.1f);
	}
	else
	{
		const _int iRand = rand() % 2;

		switch (iRand)
		{
		case 0:
		{
			pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::IDLE_BL]);

			if (nullptr == pAnimation) return E_FAIL;

			fTimePerFrame = pAnimation->Get_TickPerFrame();

			m_fTimeLimit = 1.f + ((rand() % 5) * 0.1f);
		}
			break;
		case 1:
		{
			pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::IDLE_SHORT_00_BL]);

			if (nullptr == pAnimation) return E_FAIL;

			m_fTimeLimit =  CBeatManager::GetInstance()->Get_SPB(4);

			fTimePerFrame = m_fTimeLimit / 55.f;
		}
			break;
		default:
		{
			pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::IDLE_SHORT_00_BL]);

			if (nullptr == pAnimation) return E_FAIL;

			m_fTimeLimit = CBeatManager::GetInstance()->Get_SPB(4);

			fTimePerFrame = m_fTimeLimit / 55.f;
		}
			break;
		}
	}

	m_pModel->Set_Animation(pAnimation, fTimePerFrame, fTweenTime);

	return S_OK;
}

const wstring CState_Blader_Idle::Tick(const _double& fTimeDelta)
{
	Look_Target();

	m_fTimeAcc += fTimeDelta;

	if (!m_bPlaySound && AnimNames_BL[ANIM_BL::IDLE_SHORT_00_BL] == m_pModel->Get_CurAnimation()->Get_Name())
	{
		if (20 == m_pModel->Get_CurAnimationFrame())
		{
			ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_BLADER_PARRY_SWING, CHANNEL_ID::ENEMY_BLADER, 0.5f);
			m_bPlaySound = TRUE;
		}
	}

	return m_strName;
}

const wstring CState_Blader_Idle::LateTick()
{
	return Check_Transition();
}

void CState_Blader_Idle::Exit()
{
	m_fTimeAcc = 0.f;
	m_fTimeLimit = 0.f;

	m_bPlaySound = FALSE;
}

const wstring CState_Blader_Idle::Check_Transition()
{
	if (m_pBlader->m_tFightDesc.bDamaged)
		return StateNames_BL[STATE_BL::STATE_DAMAGED_BL];

	if (m_pModel->Is_Tween() || m_fTimeAcc < m_fTimeLimit)
		return m_strName;

	return Choose_NextState();
}

const wstring CState_Blader_Idle::Choose_NextState()
{
	/* 이전 스테이트가 등장 스테이트였다면 바로 공격으로 전환 */
	if (StateNames_BL[STATE_BL::STATE_APPEAR_BL] == m_pStateMachine->Get_PrevState()->Get_Name())
		return StateNames_BL[STATE_BL::STATE_ATTACK_BL];

	/* 가깝다면 바로 공격 */
	if (Get_Distance() <= m_fTargetMinDist) 
		return StateNames_BL[STATE_BL::STATE_ATTACK_BL];

	vector<_float> Probabilities = { 0.7f, 0.3f }; // move idle attack

	random_device			rd;
	mt19937					gen(rd());
	discrete_distribution<> distribution(Probabilities.begin(), Probabilities.end());

	const _int iChoiceIndex = distribution(gen);
	
	switch (iChoiceIndex)
	{
	case 0 : 
		return StateNames_BL[STATE_BL::STATE_ATTACK_BL];
		break;
	default :
		return StateNames_BL[STATE_BL::STATE_MOVE_BL];
		break;
	}
	// Parry 

	return m_strName;
}

CState_Blader_Idle* CState_Blader_Idle::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Blader_Idle* pInstance = new CState_Blader_Idle();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Blader_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Blader_Idle::Clone(void* pArg)
{
	return nullptr;
}

void CState_Blader_Idle::Free()
{
}
