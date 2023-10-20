#pragma once

#include "Client_Defines.h"

#include "Blackboard.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CBlackboard_Saver final : public CBlackboard
{
private:
	CBlackboard_Saver(class CGameObject* pGameObject);
	CBlackboard_Saver(const CBlackboard_Saver& rhs);
	virtual ~CBlackboard_Saver() = default;

private:
	CGameObject* pTarget = nullptr;

public:
	static CBlackboard_Saver* Create(CGameObject* pGameObject);
	virtual void Free();
};

END
