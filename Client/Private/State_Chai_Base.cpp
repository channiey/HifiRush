#include "..\Default\stdafx.h"
#include "State_Chai_Base.h"

CState_Chai_Base::CState_Chai_Base()
{
}

CState_Chai_Base::CState_Chai_Base(const CState_Chai_Base& rhs)
{
}

HRESULT CState_Chai_Base::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName)))
		return E_FAIL;

	m_pChai = dynamic_cast<CChai*>(pOwner);

	if (nullptr == m_pChai) 
		return E_FAIL;

	m_pModel = m_pChai->Get_Model();

	if (nullptr == m_pModel)
		return E_FAIL;

	return S_OK;
}

CState* CState_Chai_Base::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_Base::Free()
{
	__super::Free();
}
