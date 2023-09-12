#pragma once

#include "Node.h"

BEGIN(Engine)

class ENGINE_DLL CNode_Parallel final : public CNode
{

protected:
	CNode_Parallel();
	CNode_Parallel(const CNode_Parallel& rhs);
	virtual ~CNode_Parallel() = default;

public:
	virtual const NODE_STATE Evaluate(const _float& fTimeDelta);

public:
	HRESULT Add_ChildNode(CNode* pChildNode) override;

private:
	virtual CNode* Clone(void* pArg);
	virtual void Free();
};

END