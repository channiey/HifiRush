#include "Blackboard.h"

#include "GameObject.h"
CBlackboard::CBlackboard( CGameObject* pGameObject)
	: m_pOwner(pGameObject)
{
	Safe_AddRef(m_pOwner);
}

CBlackboard::CBlackboard(const CBlackboard& rhs)
{
}


void CBlackboard::Free()
{
	Safe_Release(m_pOwner);
}
