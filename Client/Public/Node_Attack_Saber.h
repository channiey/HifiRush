#pragma once

#include "Client_Defines.h"

#include "Node_Action_Saber.h"

BEGIN(Client)
class CNode_Attack_Saber final : public CNode_Action_Saber
{
private:
	CNode_Attack_Saber();
	CNode_Attack_Saber(const CNode_Attack_Saber& rhs);
	virtual ~CNode_Attack_Saber() = default;

public:
	HRESULT Initialize_Node(class CBlackboard* pBlackboard);
	virtual const NODE_STATE Evaluate(const _double& fTimeDelta);

private:
	virtual const _bool Check_Condition(const _double& fTimeDelta) override;

private:
	void Attack();
	void Reset_BT();

public:
	static CNode_Attack_Saber* Create(class CBlackboard* pBlackboard);
	virtual void Free();
};
END
