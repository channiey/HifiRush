#pragma once

#include "Client_Defines.h"

#include "Blackboard.h"

#include "Saber.h"

BEGIN(Client)

class CBlackboard_Saber final : public CBlackboard
{
private:
	CBlackboard_Saber(class CGameObject* pGameObject);
	CBlackboard_Saber(const CBlackboard_Saber& rhs);
	virtual ~CBlackboard_Saber() = default;

public:
	 CSaber*	 m_pSaber			= { nullptr };
	
	const _float		 m_fCanBattleDist	= { 5.f };
	const _float		 m_fCanAttackDist	= { 1.f };

	CCharacter*  m_pTarget			= { nullptr };

public:
	static CBlackboard_Saber* Create(CGameObject* pGameObject);
	virtual void Free();
};

END
