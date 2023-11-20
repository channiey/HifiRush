#include "..\Default\stdafx.h"
#include "State_Korsica_Battle.h"

CState_Korsica_Battle::CState_Korsica_Battle()
{
}

CState_Korsica_Battle::CState_Korsica_Battle(const CState_Korsica_Battle& rhs)
{
}

HRESULT CState_Korsica_Battle::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Korsica_Battle::Enter()
{

	return S_OK;
}

const wstring CState_Korsica_Battle::Tick(const _double& fTimeDelta)
{

	return m_strName;
}

const wstring CState_Korsica_Battle::LateTick()
{
	return Check_Transition();
}

void CState_Korsica_Battle::Exit()
{

}

const wstring CState_Korsica_Battle::Check_Transition()
{


	return m_strName;
}


CState_Korsica_Battle* CState_Korsica_Battle::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Korsica_Battle* pInstance = new CState_Korsica_Battle();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Korsica_Battle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Korsica_Battle::Clone(void* pArg)
{
	return nullptr;
}

void CState_Korsica_Battle::Free()
{
}
