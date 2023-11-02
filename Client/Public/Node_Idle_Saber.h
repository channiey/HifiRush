#pragma once

#include "Client_Defines.h"

#include "Node_Action_Saber.h"

BEGIN(Client)
class CNode_Idle_Saber final : public CNode_Action_Saber
{
private:
	CNode_Idle_Saber();
	CNode_Idle_Saber(const CNode_Idle_Saber& rhs);
	virtual ~CNode_Idle_Saber() = default;

public:
	HRESULT Initialize_Node(class CBlackboard* pBlackboard);
	virtual const NODE_STATE Evaluate(const _double& fTimeDelta);

private:
	virtual const _bool Check_Condition(const _double& fTimeDelta) override;

private:
	void Wait(const _double& fTimeDelta);

public:
	static CNode_Idle_Saber* Create(class CBlackboard* pBlackboard);
	virtual void Free();
};
END
