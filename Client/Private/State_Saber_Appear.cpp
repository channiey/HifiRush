#include "..\Default\stdafx.h"
#include "State_Saber_Appear.h"

CState_Saber_Appear::CState_Saber_Appear()
{
}

CState_Saber_Appear::CState_Saber_Appear(const CState_Saber_Appear& rhs)
{
}

HRESULT CState_Saber_Appear::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Saber_Appear::Enter()
{
	ANIM_SA			eAnimID = ANIM_SA::APPEAR_00_SA;
	CAnimation*		pAnimation = m_pModel->Get_Animation(eAnimID);
	const _double	fTimePerFrame = CBeatManager::GetInstance()->Get_SPB(4) / (_double)55.f;

	m_pModel->Set_Animation(eAnimID, fTimePerFrame, DF_TW_TIME);

	return S_OK;
}

const wstring& CState_Saber_Appear::Tick(const _double& fTimeDelta)
{
	return m_strName;
}

const wstring& CState_Saber_Appear::LateTick()
{
	return Check_Transition();
}

void CState_Saber_Appear::Exit()
{
}

const wstring& CState_Saber_Appear::Check_Transition()
{
	if (m_pModel->Is_Tween())
		return m_strName;

	if (55 <= m_pModel->Get_TweenDesc().cur.iCurFrame)
		return StateNames_SA[STATE_IDLE_SA];

	return m_strName;
}

CState_Saber_Appear* CState_Saber_Appear::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Saber_Appear* pInstance = new CState_Saber_Appear();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Saber_Appear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Saber_Appear::Clone(void* pArg)
{
	return nullptr;
}

void CState_Saber_Appear::Free()
{
}
