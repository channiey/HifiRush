#pragma once

#include "Client_Defines.h"

#include "Node_Action_Saber.h"

BEGIN(Client)
class CNode_Damaged_Saber final : public CNode_Action_Saber
{
private:
	CNode_Damaged_Saber();
	CNode_Damaged_Saber(const CNode_Damaged_Saber& rhs);
	virtual ~CNode_Damaged_Saber() = default;

public:
	HRESULT Initialize_Node(class CBlackboard* pBlackboard);
	virtual const NODE_STATE Evaluate(const _float& fTimeDelta);

private:
	const _bool Check_Condition(const _float& fTimeDelta);


private:
	void Damaged();

public:
	static CNode_Damaged_Saber* Create(class CBlackboard* pBlackboard);
	virtual void Free();
};
END
