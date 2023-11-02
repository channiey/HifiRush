#pragma once

#include "Client_Defines.h"

#include "Node_Action_Saber.h"

BEGIN(Client)
class CNode_Wait_Saber final : public CNode_Action_Saber
{
private:
	CNode_Wait_Saber();
	CNode_Wait_Saber(const CNode_Wait_Saber& rhs);
	virtual ~CNode_Wait_Saber() = default;

public:
	HRESULT Initialize_Node(class CBlackboard* pBlackboard);
	virtual const NODE_STATE Evaluate(const _double& fTimeDelta);

private:
	virtual const _bool Check_Condition(const _double& fTimeDelta) override;

private:
	void Wait(const _double& fTimeDelta);

public:
	static CNode_Wait_Saber* Create(class CBlackboard* pBlackboard);
	virtual void Free();
};
END
