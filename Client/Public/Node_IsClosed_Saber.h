#pragma once

#include "Client_Defines.h"

#include "Node_Action_Saber.h"

BEGIN(Client)
class CNode_IsClosed_Saber final : public CNode_Action_Saber
{
private:
	CNode_IsClosed_Saber();
	CNode_IsClosed_Saber(const CNode_IsClosed_Saber& rhs);
	virtual ~CNode_IsClosed_Saber() = default;

public:
	HRESULT Initialize_Node(class CBlackboard* pBlackboard);
	virtual const NODE_STATE Evaluate(const _float& fTimeDelta);

private:
	void Move();

public:
	static CNode_IsClosed_Saber* Create(class CBlackboard* pBlackboard);
	virtual void Free();
};
END
