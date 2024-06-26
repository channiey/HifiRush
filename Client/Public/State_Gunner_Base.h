#pragma once

#include "Client_Defines.h"

#include "Animation.h"

#include "EngineInstance.h"
#include "State.h"

#include "Gunner.h"

#include "BeatManager.h"
#include "BattleManager.h"

BEGIN(Engine)
class CGameObjet;
class CModel;
END

BEGIN(Client)

class CState_Gunner_Base abstract : public CState
{
protected:
	CState_Gunner_Base();
	CState_Gunner_Base(const CState_Gunner_Base& rhs);
	virtual ~CState_Gunner_Base() = default;

protected:
	virtual HRESULT		Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	virtual void		OnCollision_Enter(CGameObject* pGameObject) {};
	virtual void		OnCollision_Stay(CGameObject* pGameObject) {};
	virtual void		OnCollision_Exit(CGameObject* pGameObject) {};

protected:
	const wstring		Choice_NextState();
	const _float		Get_Distance();
	virtual void		Set_NextAnimation() {};
	void				Look_Target();

protected:
	CGunner*			m_pGunner = { nullptr };
	CModel*				m_pModel = { nullptr };

	_float				m_fTargetMinDist = 2.f;
	_float				m_fTargetMaxDist = 5.f;

	_double				m_fTimeLimit = 0.f;
	_double				m_fTimeAcc = 0.f;

	_float				m_fOriginHeight = 0.f;

public:
	virtual CState*		Clone(void* pArg) = 0;
	virtual void		Free();
};

END
