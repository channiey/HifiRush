#pragma once

#include "Client_Defines.h"

#include "Node_Action_Saber.h"

BEGIN(Client)
class CNode_Move_Saber final : public CNode_Action_Saber
{
private:
	CNode_Move_Saber();
	CNode_Move_Saber(const CNode_Move_Saber& rhs);
	virtual ~CNode_Move_Saber() = default;

public:
	HRESULT Initialize_Node(class CBlackboard* pBlackboard);
	virtual const NODE_STATE Evaluate(const _float& fTimeDelta);

private:
	virtual const _bool Check_Condition(const _float& fTimeDelta) override;

private:
	const _bool	Is_ReachTarget();
	void Move_ToTarget(const _float& fTimeDelta);
	void Wait(const _float& fTimeDelta);

public:
	static CNode_Move_Saber* Create(class CBlackboard* pBlackboard);
	virtual void Free();
};
END
