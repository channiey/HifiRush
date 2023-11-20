#include "..\Default\stdafx.h"
#include "State_Peppermint_Base.h"

CState_Peppermint_Base::CState_Peppermint_Base()
{
}

CState_Peppermint_Base::CState_Peppermint_Base(const CState_Peppermint_Base& rhs)
{
}

HRESULT CState_Peppermint_Base::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName)))
		return E_FAIL;

	m_pPeppermint = dynamic_cast<CPeppermint*>(pOwner);

	if (nullptr == m_pPeppermint)
		return E_FAIL;

	m_pModel = m_pPeppermint->Get_Model();

	if (nullptr == m_pModel)
		return E_FAIL;

	return S_OK;
}

CState* CState_Peppermint_Base::Clone(void* pArg)
{
	return nullptr;
}

void CState_Peppermint_Base::Free()
{
	__super::Free();
}
