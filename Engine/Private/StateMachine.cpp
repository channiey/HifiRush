#include "..\Public\StateMachine.h"

#include "State.h"

CStateMachine::CStateMachine(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CStateMachine::CStateMachine(const CStateMachine & rhs)
	: CComponent(rhs)
{

}

HRESULT CStateMachine::Initialize_Prototype()
{
	

	return S_OK;
}

HRESULT CStateMachine::Initialize(void * pArg)
{
	return S_OK;
}

CState* CStateMachine::Tick(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pCurState, nullptr);

	CState* pState = m_pCurState->Tick(fTimeDelta);

	if (pState != m_pCurState)
		Change_State(pState);

	/* 현재 상태 유지*/

	return nullptr;
}

CState* CStateMachine::LateTick()
{
	NULL_CHECK_RETURN(m_pCurState, nullptr);

	if (m_pCurState->LateTick() != m_pCurState)
	{
		/* 상태의 변환 */
	}

	/* 현재 상태 유지*/

	return nullptr;
}

HRESULT CStateMachine::Add_State(const wstring& strStateTag, CState* pState)
{
	if (nullptr == pState || Has_State(strStateTag))
		return E_FAIL;
	
	m_pStates.emplace(strStateTag, pState);

	return S_OK;
}

const _bool& CStateMachine::Has_State(const wstring& strStateTag)
{
	auto iter = m_pStates.find(strStateTag);

	if (m_pStates.end() == iter)
		return false;

	return true;
}

HRESULT CStateMachine::Change_State(CState* pState)
{
	return S_OK;
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
}
