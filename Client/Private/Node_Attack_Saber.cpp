#include "..\Default\stdafx.h"

#include "GameInstance.h"
#include "Node_Attack_Saber.h"


CNode_Attack_Saber::CNode_Attack_Saber()
{
}

CNode_Attack_Saber::CNode_Attack_Saber(const CNode_Attack_Saber& rhs)
{
}

HRESULT CNode_Attack_Saber::Initialize_Node(CBlackboard* pBlackboard)
{
	if (FAILED(__super::Initialize_Node(pBlackboard)))
		return E_FAIL;

	return S_OK;
}

const NODE_STATE CNode_Attack_Saber::Evaluate(const _float& fTimeDelta)
{
	return NODE_STATE::FAILURE;
}

CNode_Attack_Saber* CNode_Attack_Saber::Create(CBlackboard* pBlackboard)
{
	CNode_Attack_Saber* pInstance = new CNode_Attack_Saber();

	if (FAILED(pInstance->Initialize_Node(pBlackboard)))
	{
		MSG_BOX("Failed to Created : CNode_Attack_Saber");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNode_Attack_Saber::Free()
{
	__super::Free();
}
