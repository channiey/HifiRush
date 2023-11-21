#pragma once

#include "Client_Defines.h"

#include "Animation.h"

#include "EngineInstance.h"
#include "State.h"

#include "Peppermint.h"
#include "Input.h"

#include "PlayerController.h"

BEGIN(Client)

class CState_Peppermint_Base abstract : public CState
{
protected:
	CState_Peppermint_Base();
	CState_Peppermint_Base(const CState_Peppermint_Base& rhs);
	virtual ~CState_Peppermint_Base() = default;

protected:
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

protected:
	virtual void	Check_Progress(const _double& fTimeDelta) {};
	virtual void	Set_Transform() {};
	virtual void	Play_Sound() {};

protected:
	CPeppermint*	m_pPeppermint = { nullptr };
	CModel*			m_pModel = { nullptr };

public:
	virtual CState* Clone(void* pArg) = 0;
	virtual void	Free();
};


END
