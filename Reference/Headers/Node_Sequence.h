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
	virtual HRESULT Initialize_Node(class CBlackboard* pBlackboard);
	virtual const NODE_STATE Evaluate(const _float& fTimeDelta);

public:
	HRESULT Add_ChildNode(CNode* pChildNode) override;
	virtual HRESULT Reset_Node() override;

public:
	static CNode_Sequence* Create(class CBlackboard* pBlackboard);
	virtual void Free();
};

END