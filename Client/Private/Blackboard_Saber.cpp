#include "..\Default\stdafx.h"
#include "Blackboard_Saber.h"

#include "GameInstance.h"


CBlackboard_Saber::CBlackboard_Saber(class CGameObject* pGameObject)
{
	CSaber* pSaber = dynamic_cast<CSaber*>(pGameObject);

	if (nullptr != pSaber)
		m_pSaber = pSaber;
}

CBlackboard_Saber::CBlackboard_Saber(const CBlackboard_Saber& rhs)
	: CBlackboard(rhs)
{
}

CBlackboard_Saber* CBlackboard_Saber::Create(class CGameObject* pGameObject)
{
	CBlackboard_Saber* pInstance = new CBlackboard_Saber(pGameObject);

	if (nullptr == pInstance)
	{
		MSG_BOX("Failed To Created : CBlackboard_Saber");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlackboard_Saber::Free()
{
}
