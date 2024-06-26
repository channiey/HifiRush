#pragma once

#include "Client_Defines.h"

#include "Animation.h"

#include "EngineInstance.h"
#include "State.h"

#include "Blader.h"

#include "BeatManager.h"
#include "BattleManager.h"

BEGIN(Engine)
class CGameObjet;
class CModel;
END

BEGIN(Client)

class CState_Blader_Base abstract : public CState
{
protected:
	CState_Blader_Base();
	CState_Blader_Base(const CState_Blader_Base& rhs);
	virtual ~CState_Blader_Base() = default;

protected:
	virtual HRESULT		Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	virtual void		OnCollision_Enter(CGameObject* pGameObject) {};
	virtual void		OnCollision_Stay(CGameObject* pGameObject) {};
	virtual void		OnCollision_Exit(CGameObject* pGameObject) {};

protected:
	const _float		Get_Distance();
	void				Turn_ToTarget();
	void				Look_Target();
	const _bool			Is_On_Right(class CTransform* pTargetTransform);
	const _bool			Is_On_Front(class CTransform* pTargetTransform);

protected:
	virtual HRESULT			Set_Animation() { return S_OK; }
	virtual const wstring	Choose_NextState() { return wstring(); }



protected:
	CBlader*			m_pBlader = { nullptr };
	CModel*				m_pModel = { nullptr };

	_float				m_fTargetMinDist = 5.f;
	_float				m_fTargetMaxDist = 20.f;

	_double				m_fTimeLimit = 0.f;
	_double				m_fTimeAcc = 0.f;

public:
	virtual CState* Clone(void* pArg) = 0;
	virtual void		Free();
};

END
