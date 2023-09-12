#pragma once
   

#include "Base.h"
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CBlackboard abstract : public CBase
{

protected:
	CBlackboard();
	CBlackboard(const CBlackboard& rhs);
	virtual ~CBlackboard() = default;

private:
	virtual CBlackboard* Clone(void* pArg) PURE;
	virtual void Free();
};

END		