#include "..\Default\stdafx.h"
#include "State_Chai_Idle.h"

CState_Chai_Idle::CState_Chai_Idle()
{
}

CState_Chai_Idle::CState_Chai_Idle(const CState_Chai_Idle& rhs)
{
}

HRESULT CState_Chai_Idle::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Chai_Idle::Enter()
{
	m_pChai->Get_Model()->Set_Animation(2, TRUE); /* TODO:: 애니메이션 다 적용하고 CChai에 있는 enum으로 사용 */

	return S_OK;
}

const wstring& CState_Chai_Idle::Tick(const _float& fTimeDelta)
{
	// ... 

	return Check_Transition();
}

const wstring& CState_Chai_Idle::LateTick()
{
	// ... 

	return Check_Transition();
}

void CState_Chai_Idle::Exit()
{
}

const wstring& CState_Chai_Idle::Check_Transition()
{
	return __super::Check_Transition();
}

CState_Chai_Idle* CState_Chai_Idle::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Chai_Idle* pInstance = new CState_Chai_Idle();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Chai_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Chai_Idle::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_Idle::Free()
{
}
