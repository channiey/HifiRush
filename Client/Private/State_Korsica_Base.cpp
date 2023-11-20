#include "..\Default\stdafx.h"
#include "State_Korsica_Base.h"

CState_Korsica_Base::CState_Korsica_Base()
{
}

CState_Korsica_Base::CState_Korsica_Base(const CState_Korsica_Base& rhs)
{
}

HRESULT CState_Korsica_Base::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName)))
		return E_FAIL;

	m_pKorsica = dynamic_cast<CKorsica*>(pOwner);

	if (nullptr == m_pKorsica)
		return E_FAIL;

	m_pModel = m_pKorsica->Get_Model();

	if (nullptr == m_pModel)
		return E_FAIL;

	return S_OK;
}

CState* CState_Korsica_Base::Clone(void* pArg)
{
	return nullptr;
}

void CState_Korsica_Base::Free()
{
	__super::Free();
}
