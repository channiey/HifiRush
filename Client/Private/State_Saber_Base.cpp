#include "..\Default\stdafx.h"
#include "State_Saber_Base.h"

CState_Saber_Base::CState_Saber_Base()
{
}

CState_Saber_Base::CState_Saber_Base(const CState_Saber_Base& rhs)
{
}

HRESULT CState_Saber_Base::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName)))
		return E_FAIL;

	m_pSaber = dynamic_cast<CSaber*>(pOwner);

	if (nullptr == m_pSaber) 
		return E_FAIL;

	return S_OK;
}

CState* CState_Saber_Base::Clone(void* pArg)
{
	return nullptr;
}

void CState_Saber_Base::Free()
{
	__super::Free();
}
