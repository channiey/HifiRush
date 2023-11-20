#include "..\Default\stdafx.h"
#include "State_Macaron_Battle.h"

CState_Macaron_Battle::CState_Macaron_Battle()
{
}

CState_Macaron_Battle::CState_Macaron_Battle(const CState_Macaron_Battle& rhs)
{
}

HRESULT CState_Macaron_Battle::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Macaron_Battle::Enter()
{
	
	return S_OK;
}

const wstring CState_Macaron_Battle::Tick(const _double& fTimeDelta)
{
	
	return m_strName;
}

const wstring CState_Macaron_Battle::LateTick()
{
	return Check_Transition();
}

void CState_Macaron_Battle::Exit()
{
	
}

const wstring CState_Macaron_Battle::Check_Transition()
{


	return m_strName;
}


CState_Macaron_Battle* CState_Macaron_Battle::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Macaron_Battle* pInstance = new CState_Macaron_Battle();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Macaron_Battle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Macaron_Battle::Clone(void* pArg)
{
	return nullptr;
}

void CState_Macaron_Battle::Free()
{
}
