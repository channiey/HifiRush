#include "..\Public\StateMachine.h"

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
