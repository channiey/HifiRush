#pragma once

#include "Node.h"

BEGIN(Engine)

class ENGINE_DLL CNode_Sequence final : public CNode
{

protected:
	CNode_Sequence();
	CNode_Sequence(const CNode_Sequence& rhs);
	virtual ~CNode_Sequence() = default;

public:
	virtual const NODE_STATE Tick(const _float& fTimeDelta);

public:
	HRESULT Add_ChildNode(CNode* pChildNode) override;

private:
	virtual CNode* Clone(void* pArg);
	virtual void Free();
};

END