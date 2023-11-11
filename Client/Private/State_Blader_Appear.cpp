#include "..\Default\stdafx.h"
#include "State_Blader_Appear.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CState_Blader_Appear::CState_Blader_Appear()
{
}

CState_Blader_Appear::CState_Blader_Appear(const CState_Blader_Appear& rhs)
{
}

HRESULT CState_Blader_Appear::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Blader_Appear::Enter()
{
	CAnimation* pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::APPEAR_BL]);

	if (nullptr == pAnimation)
		return E_FAIL;

	_double	fTimePerFrame = 1.f / pAnimation->Get_TickPerSecond();

	m_pModel->Set_Animation(pAnimation, fTimePerFrame, DF_TW_TIME);

	return S_OK;
}

const wstring CState_Blader_Appear::Tick(const _double& fTimeDelta)
{
	Look_Target();

	return m_strName;
}

const wstring CState_Blader_Appear::LateTick()
{
	return Check_Transition();
}

void CState_Blader_Appear::Exit()
{

}

const wstring CState_Blader_Appear::Check_Transition()
{
	if (!m_pModel->Is_Tween() && 42 <= m_pModel->Get_TweenDesc().cur.iCurFrame)
		return StateNames_BL[STATE_IDLE_BL];

	return m_strName;
}

CState_Blader_Appear* CState_Blader_Appear::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Blader_Appear* pInstance = new CState_Blader_Appear();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Blader_Appear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Blader_Appear::Clone(void* pArg)
{
	return nullptr;
}

void CState_Blader_Appear::Free()
{
}
