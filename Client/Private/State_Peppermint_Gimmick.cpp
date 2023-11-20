#include "..\Default\stdafx.h"
#include "State_Peppermint_Gimmick.h"

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
	m_pStateMachine->Set_State(StateNames_PE[STATE_PE::STATE_BATTLE_PE]);

	return S_OK;
}

const wstring CState_Peppermint_Gimmick::Tick(const _double& fTimeDelta)
{

	return m_strName;
}

const wstring CState_Peppermint_Gimmick::LateTick()
{
	return Check_Transition();
}

void CState_Peppermint_Gimmick::Exit()
{

}

const wstring CState_Peppermint_Gimmick::Check_Transition()
{
	return m_strName;
}

void CState_Peppermint_Gimmick::Check_Progress(const _double& fTimeDelta)
{
}

void CState_Peppermint_Gimmick::Set_Transform()
{
	
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
