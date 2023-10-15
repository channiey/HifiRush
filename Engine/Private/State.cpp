#include "State.h"

CState::CState()
{

}

CState::CState(const CState& rhs)
{
}

HRESULT CState::Initialize(CStateMachine* pStateMachine, const wstring& strStateName)
{
	if (nullptr == pStateMachine)
		return E_FAIL;

	m_pStateMachine = pStateMachine;

	m_strName = strStateName;

	return S_OK;
}


void CState::Free()
{
}
