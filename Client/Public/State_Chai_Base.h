#pragma once

#include "Client_Defines.h"

#include "Animation.h"

#include "EngineInstance.h"
#include "State.h"

#include "Chai.h"
#include "Input.h"

#include "BeatManager.h"
#include "BattleManager.h"

#include "PlayerController.h"

BEGIN(Engine)
class CGameObjet;
END

BEGIN(Client)
class CEffect;

class CState_Chai_Base abstract : public CState
{
protected:
	CState_Chai_Base();
	CState_Chai_Base(const CState_Chai_Base& rhs);
	virtual ~CState_Chai_Base() = default;

protected:
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

protected:
	virtual HRESULT Ready_Effect()	{ return S_OK; }
	virtual HRESULT Set_Effect()	{ return S_OK; }

protected:
	CChai*		m_pChai = { nullptr };
	CModel*		m_pModel = { nullptr };
	CEffect*	m_pEffect = { nullptr };

public:
	virtual CState* Clone(void* pArg) = 0;
	virtual void Free();
};


END
