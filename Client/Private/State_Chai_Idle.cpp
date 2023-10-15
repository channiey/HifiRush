#include "..\Default\stdafx.h"
#include "State_Chai_Idle.h"

CState_Chai_Idle::CState_Chai_Idle()
{
}

CState_Chai_Idle::CState_Chai_Idle(const CState_Chai_Idle& rhs)
{
}

HRESULT CState_Chai_Idle::Initialize()
{
	return E_NOTIMPL;
}

HRESULT CState_Chai_Idle::Enter()
{
	return S_OK;
}

const wstring& CState_Chai_Idle::Tick(const _float& fimeDelta)
{
	return m_pChai->m_StateNames[CChai::IDLE];
}

const wstring& CState_Chai_Idle::LateTick()
{
	return m_pChai->m_StateNames[CChai::IDLE];
}

void CState_Chai_Idle::Exit()
{
}

const wstring& CState_Chai_Idle::Check_Transition()
{
	return m_pChai->m_StateNames[CChai::NONE];
}

CState_Chai_Idle* CState_Chai_Idle::Create()
{
	CState_Chai_Idle* pInstance = new CState_Chai_Idle();

	if (FAILED(pInstance->Initialize()))
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
