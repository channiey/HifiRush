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
	virtual const NODE_STATE Evaluate(const _float& fTimeDelta);

public:
	static CNode_Attack_Saber* Create(class CBlackboard* pBlackboard);
	virtual void Free();
};
END
