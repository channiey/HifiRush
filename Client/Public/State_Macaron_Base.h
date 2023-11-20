#pragma once

#include "Client_Defines.h"

#include "Animation.h"

#include "EngineInstance.h"
#include "State.h"

#include "Macaron.h"
#include "Input.h"

#include "PlayerController.h"

BEGIN(Client)

class CState_Macaron_Base abstract : public CState
{
protected:
	CState_Macaron_Base();
	CState_Macaron_Base(const CState_Macaron_Base& rhs);
	virtual ~CState_Macaron_Base() = default;

protected:
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

protected:
	virtual void	Check_Progress(const _double& fTimeDelta) {};
	virtual void	Set_Transform() {};
	virtual void	Play_Sound() {};

protected:
	CMacaron*	m_pMacaron = { nullptr };
	CModel*		m_pModel = { nullptr };

public:
	virtual CState* Clone(void* pArg) = 0;
	virtual void	Free();
};


END
