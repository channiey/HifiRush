#include "..\Default\stdafx.h"
#include "Blackboard_Saver.h"

#include "GameInstance.h"


CBlackboard_Saver::CBlackboard_Saver(class CGameObject* pGameObject)
	: CBlackboard(pGameObject)
{
}

CBlackboard_Saver::CBlackboard_Saver(const CBlackboard_Saver& rhs)
	: CBlackboard(rhs)
{
}

CBlackboard_Saver* CBlackboard_Saver::Create(class CGameObject* pGameObject)
{
	CBlackboard_Saver* pInstance = new CBlackboard_Saver(pGameObject);

	if (nullptr == pInstance)
	{
		MSG_BOX("Failed To Created : CBlackboard_Saver");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlackboard_Saver::Free()
{
}
