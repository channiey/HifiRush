#include "..\Default\stdafx.h"
#include "State_Macaron_Gimmick.h"

CState_Macaron_Gimmick::CState_Macaron_Gimmick()
{
}

CState_Macaron_Gimmick::CState_Macaron_Gimmick(const CState_Macaron_Gimmick& rhs)
{
}

HRESULT CState_Macaron_Gimmick::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Macaron_Gimmick::Enter()
{

	return S_OK;
}

const wstring CState_Macaron_Gimmick::Tick(const _double& fTimeDelta)
{

	return m_strName;
}

const wstring CState_Macaron_Gimmick::LateTick()
{
	return Check_Transition();
}

void CState_Macaron_Gimmick::Exit()
{

}

const wstring CState_Macaron_Gimmick::Check_Transition()
{


	return m_strName;
}


CState_Macaron_Gimmick* CState_Macaron_Gimmick::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Macaron_Gimmick* pInstance = new CState_Macaron_Gimmick();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Macaron_Gimmick");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Macaron_Gimmick::Clone(void* pArg)
{
	return nullptr;
}

void CState_Macaron_Gimmick::Free()
{
}
