#pragma once

/* 개별 액션은 클라이언트에서 구현한다. */

#include "Node.h"

BEGIN(Engine)

class ENGINE_DLL CNode_Action abstract : public CNode
{
protected:
	CNode_Action();
	CNode_Action(const CNode_Action& rhs);
	virtual ~CNode_Action() = default;

public:
	virtual HRESULT Initialize_Node(class CBlackboard* pBlackboard);
	virtual const NODE_STATE Evaluate(const _float& fTimeDelta) PURE;

public:
	/* 액션은 Leaf 노드로서 자식 노드를 갖지 않는다.*/
	HRESULT Add_ChildNode(CNode* pChildNode) override { return E_FAIL; }
	virtual HRESULT Reset_Node() override;

private:
	virtual void Free();
};

END