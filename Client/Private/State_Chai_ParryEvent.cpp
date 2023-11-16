#include "..\Default\stdafx.h"
#include "State_Chai_ParryEvent.h"

CState_Chai_ParryEvent::CState_Chai_ParryEvent()
{
}

CState_Chai_ParryEvent::CState_Chai_ParryEvent(const CState_Chai_ParryEvent& rhs)
{
}

HRESULT CState_Chai_ParryEvent::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Chai_ParryEvent::Enter()
{
	ANIM_CH			eAnimID = ANIM_CH::ATK_WAIT;
	CAnimation*		pAnimation = m_pChai->Get_Model()->Get_Animation(eAnimID);

	m_pChai->Get_Model()->Set_Animation(eAnimID, pAnimation->Get_TickPerFrame(), DF_TW_TIME, FALSE);

	return S_OK;
}

const wstring CState_Chai_ParryEvent::Tick(const _double& fTimeDelta)
{
	if (!m_bFinalAttack && !m_bParried && !m_bDamaged && Input::Parry())
		Parry();

	if (m_bCanFinalAttack)
		FinalAttack();

	return m_strName;
}

const wstring CState_Chai_ParryEvent::LateTick()
{
	Check_Animation();

	if(!m_bFinalAttack)
		m_pChai->Get_Transform()->Set_RootPos(Vec4::Zero);

	return Check_Transition();
}

void CState_Chai_ParryEvent::Exit()
{
	m_bParried = FALSE;
	m_bDamaged = FALSE;
	m_bSuccess = TRUE;
	m_bCanFinalAttack = FALSE;
	m_bFinalAttack = FALSE;
}

void CState_Chai_ParryEvent::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	CCharacter* pCharacter = dynamic_cast<CCharacter*>(pCollider->Get_Owner()->Get_Parent());

	if (nullptr != pCharacter && LayerNames[LAYER_ENEMY] == pCharacter->Get_LayerTag())
	{
		if (m_bParried)
		{
			ENGINE_INSTANCE->Shake_Camera(0.3f, 10);
		}
		else
		{
			Damaged();
			ENGINE_INSTANCE->Shake_Camera(0.3f, 10);
			m_bSuccess = FALSE;
		}
	}
}

const wstring CState_Chai_ParryEvent::Check_Transition()
{
	if (ANIM_CH::ATK_FINAL == m_pModel->Get_CurAnimationIndex() && 70 <= m_pModel->Get_TweenDesc().cur.iCurFrame)
		return StateNames_CH[STATE_CH::STATE_IDLE_CH];

	return m_strName;
}

void CState_Chai_ParryEvent::Parry()
{
	CAnimation*		pAnimation = m_pChai->Get_Model()->Get_Animation(ANIM_CH::PARRY_00);

	if (nullptr == pAnimation) return;
	
	m_pChai->Get_Model()->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame() * 0.3f, 0.05f, FALSE);

	m_bParried = TRUE;
}

void CState_Chai_ParryEvent::Damaged()
{
	CAnimation* pAnimation = m_pChai->Get_Model()->Get_Animation(ANIM_CH::DMG_LIGHT);

	if (nullptr == pAnimation) return;

	m_pChai->Get_Model()->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame() * 0.5f, 0.075f, FALSE);

	m_bDamaged = TRUE;
}

void CState_Chai_ParryEvent::FinalAttack()
{
	CAnimation* pAnimation = m_pChai->Get_Model()->Get_Animation(ANIM_CH::ATK_FINAL);

	if (nullptr == pAnimation) return;

	m_pChai->Get_Model()->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame(), 0.05f);

	m_bCanFinalAttack = FALSE;
	m_bFinalAttack = TRUE;
}

void CState_Chai_ParryEvent::Check_Animation()
{
	if (m_bParried || m_bDamaged)
	{
		if (!m_pModel->Is_Tween() && m_pModel->Is_ThreeFourths_Animation())
		{
			CAnimation* pAnimation = m_pChai->Get_Model()->Get_Animation(ANIM_CH::ATK_WAIT);

			m_pChai->Get_Model()->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame(), DF_TW_TIME, FALSE);

			m_bParried = FALSE;
			m_bDamaged = FALSE;
		}
	}
}

CState_Chai_ParryEvent* CState_Chai_ParryEvent::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Chai_ParryEvent* pInstance = new CState_Chai_ParryEvent();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Chai_ParryEvent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Chai_ParryEvent::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_ParryEvent::Free()
{
}
