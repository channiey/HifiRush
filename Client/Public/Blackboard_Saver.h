#pragma once

#include "Client_Defines.h"

#include "Blackboard.h"

BEGIN(Client)

class CBlackboard_Saver final : public CBlackboard
{
private:
	CBlackboard_Saver(class CGameObject* pGameObject);
	CBlackboard_Saver(const CBlackboard_Saver& rhs);
	virtual ~CBlackboard_Saver() = default;


public:
	static CBlackboard_Saver* Create(class CGameObject* pGameObject);
	virtual void Free();
};

END
