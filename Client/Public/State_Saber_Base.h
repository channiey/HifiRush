#pragma once

#include "Client_Defines.h"

#include "Animation.h"

#include "EngineInstance.h"
#include "State.h"

#include "Saber.h"

#include "BeatManager.h"
#include "BattleManager.h"

BEGIN(Engine)
class CGameObjet;
END

BEGIN(Client)

class CState_Saber_Base abstract : public CState
{
protected:
	CState_Saber_Base();
	CState_Saber_Base(const CState_Saber_Base& rhs);
	virtual ~CState_Saber_Base() = default;

protected:
	virtual HRESULT		Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	virtual void		OnCollision_Enter(CGameObject* pGameObject) {};
	virtual void		OnCollision_Stay(CGameObject* pGameObject) {};
	virtual void		OnCollision_Exit(CGameObject* pGameObject) {};

protected:
	CSaber*				m_pSaber = { nullptr };

public:
	virtual CState*		Clone(void* pArg) = 0;
	virtual void		Free();
};


END
