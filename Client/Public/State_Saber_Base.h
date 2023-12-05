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
class CModel;
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
	const wstring			Choice_NextState();
	const _float			Get_Distance(); 
	virtual void			Set_NextAnimation() {};
	void					Look_Target();

protected:
	CSaber*				m_pSaber = { nullptr };
	CModel*				m_pModel = { nullptr };

	_float				m_fTargetMinDist = 3.f;
	_float				m_fTargetMaxDist = 7.f;

	_double				m_fTimeLimit = 0.f;
	_double				m_fTimeAcc = 0.f;

public:
	virtual CState*		Clone(void* pArg) = 0;
	virtual void		Free();
};

END
