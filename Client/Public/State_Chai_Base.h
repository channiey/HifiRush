#pragma once

#include "Client_Defines.h"

#include "Animation.h"

#include "EngineInstance.h"
#include "State.h"

#include "Chai.h"
#include "Input.h"

#include "BeatManager.h"
#include "BattleManager.h"

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
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	virtual void		OnCollision_Enter(CGameObject* pGameObject) {};
	virtual void		OnCollision_Stay(CGameObject* pGameObject) {};
	virtual void		OnCollision_Exit(CGameObject* pGameObject) {};

protected:
	CChai* m_pChai = { nullptr };

public:
	virtual CState* Clone(void* pArg) = 0;
	virtual void Free();
};


END
