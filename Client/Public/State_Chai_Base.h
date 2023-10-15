#pragma once

#include "Client_Defines.h"

#include "GameInstance.h"
#include "State.h"

#include "Chai.h"
#include "Input.h"

BEGIN(Engine)
class CGameObjet;
END

BEGIN(Client)

class CState_Chai_Base abstract : public CState
{
protected:
	CState_Chai_Base();
	CState_Chai_Base(const CState_Chai_Base& rhs);
	virtual ~CState_Chai_Base() = default;

protected:
	CChai* m_pChai = { nullptr };

public:
	virtual CState* Clone(void* pArg) = 0;
	virtual void Free();
};


END
