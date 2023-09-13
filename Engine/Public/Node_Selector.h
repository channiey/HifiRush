#pragma once

#include "Node.h"

BEGIN(Engine)

class ENGINE_DLL CNode_Selector final : public CNode
{

protected:
	CNode_Selector();
	CNode_Selector(const CNode_Selector& rhs);
	virtual ~CNode_Selector() = default;

public:
	virtual HRESULT Initialize_Node();
	virtual const NODE_STATE Evaluate(const _float& fTimeDelta);


public:
	HRESULT Add_ChildNode(CNode* pChildNode) override;

private:
	static CNode_Selector* Create(void* pArg = nullptr);
	virtual void Free();
};

END