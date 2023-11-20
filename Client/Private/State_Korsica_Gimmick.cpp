#include "..\Default\stdafx.h"
#include "State_Korsica_Gimmick.h"

CState_Korsica_Gimmick::CState_Korsica_Gimmick()
{
}

CState_Korsica_Gimmick::CState_Korsica_Gimmick(const CState_Korsica_Gimmick& rhs)
{
}

HRESULT CState_Korsica_Gimmick::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Korsica_Gimmick::Enter()
{

	return S_OK;
}

const wstring CState_Korsica_Gimmick::Tick(const _double& fTimeDelta)
{

	return m_strName;
}

const wstring CState_Korsica_Gimmick::LateTick()
{
	return Check_Transition();
}

void CState_Korsica_Gimmick::Exit()
{

}

const wstring CState_Korsica_Gimmick::Check_Transition()
{


	return m_strName;
}


CState_Korsica_Gimmick* CState_Korsica_Gimmick::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Korsica_Gimmick* pInstance = new CState_Korsica_Gimmick();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Korsica_Gimmick");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Korsica_Gimmick::Clone(void* pArg)
{
	return nullptr;
}

void CState_Korsica_Gimmick::Free()
{
}
