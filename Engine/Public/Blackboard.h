#pragma once
   
#include "Base.h"
#include "Engine_Defines.h"

BEGIN(Engine)
class CGameObject;

class ENGINE_DLL CBlackboard abstract : public CBase
{

protected:
	CBlackboard();
	//CBlackboard(CGameObject* pGameObject);
	CBlackboard(const CBlackboard& rhs);
	virtual ~CBlackboard() = default;

public:
	//CGameObject* Get_Owner() const { return m_pOwner; }

protected:
	//CGameObject* m_pOwner;

public:
	virtual void Free();
};

END		