#include "..\Default\stdafx.h"
#include "State_Macaron_Base.h"

CState_Macaron_Base::CState_Macaron_Base()
{
}

CState_Macaron_Base::CState_Macaron_Base(const CState_Macaron_Base& rhs)
{
}

HRESULT CState_Macaron_Base::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName)))
		return E_FAIL;

	m_pMacaron = dynamic_cast<CMacaron*>(pOwner);

	if (nullptr == m_pMacaron)
		return E_FAIL;

	m_pModel = m_pMacaron->Get_Model();

	if (nullptr == m_pModel)
		return E_FAIL;

	return S_OK;
}

CState* CState_Macaron_Base::Clone(void* pArg)
{
	return nullptr;
}

void CState_Macaron_Base::Free()
{
	__super::Free();
}
