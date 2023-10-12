#include "..\Public\StateMachine.h"

#include "State.h"

CStateMachine::CStateMachine(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CStateMachine::CStateMachine(const CStateMachine & rhs)
	: CComponent(rhs)
	, m_pStates(rhs.m_pStates)
	, m_pCurState(rhs.m_pCurState)
{
	for (auto& Pair : m_pStates)
	{
		Safe_AddRef(Pair.second);
	}

	Safe_AddRef(m_pCurState);
}

HRESULT CStateMachine::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStateMachine::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CStateMachine::Tick(const _float& fTimeDelta)
{
	if (nullptr == m_pCurState)
		return E_FAIL;

	const wstring& strStateName = m_pCurState->Tick(fTimeDelta);
	
	if (strStateName != m_pCurState->Get_Name())
		return Change_State(strStateName);

	return S_OK;
}

HRESULT CStateMachine::LateTick(const _float& fTimeDelta)
{
	if (nullptr == m_pCurState)
		return E_FAIL;
	const wstring& strStateName = m_pCurState->LateTick();

	if (strStateName != m_pCurState->Get_Name())
		return Change_State(strStateName);

	return S_OK;
}

HRESULT CStateMachine::Set_State(const wstring& strStateTag)
{
	CState* pState = Find_State(strStateTag);

	if (nullptr == pState)
		return E_FAIL;

	return (Change_State(pState->Get_Name()));
}

HRESULT CStateMachine::Add_State(const wstring& strStateTag, CState* pState)
{
	if (nullptr == pState || Has_State(strStateTag))
		return E_FAIL;
	
	pState->Set_Name(strStateTag);

	m_pStates.emplace(strStateTag, pState);
	
	if (nullptr == m_pCurState)
		m_pCurState = pState;

	Safe_AddRef(m_pCurState);

	return S_OK;
}

const _bool CStateMachine::Has_State(const wstring& strStateTag)
{
	auto iter = m_pStates.find(strStateTag);

	if (m_pStates.end() == iter)
		return false;

	return true;
}

CState* CStateMachine::Find_State(const wstring& strStateTag)
{
	auto iter = m_pStates.find(strStateTag);

	if (m_pStates.end() == iter)
		return nullptr;

	return iter->second;
}

HRESULT CStateMachine::Change_State(const wstring& strStateTag)
{
	CState* pNextState = Find_State(strStateTag);

	NULL_CHECK_RETURN(pNextState, E_FAIL);

	m_pCurState->Exit();
	Safe_Release(m_pCurState);

	m_pCurState = pNextState;
	Safe_AddRef(m_pCurState);

	m_pCurState->Enter();

	return S_OK;
}

CStateMachine* CStateMachine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStateMachine* pInstance = new CStateMachine(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CStateMachine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CStateMachine::Clone(void * pArg)
{
	CStateMachine*	pInstance = new CStateMachine(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CStateMachine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateMachine::Free()
{
	__super::Free();

	Safe_Release(m_pCurState);

	for (auto& Pair : m_pStates)
	{
		Safe_Release(Pair.second);
	}
	m_pStates.clear();
}
