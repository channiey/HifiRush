#pragma once

/* ���� �׼��� Ŭ���̾�Ʈ���� �����Ѵ�. */

#include "Node.h"

BEGIN(Engine)

class ENGINE_DLL CNode_Action abstract : public CNode
{
protected:
	CNode_Action();
	CNode_Action(const CNode_Action& rhs);
	virtual ~CNode_Action() = default;

public:
	virtual const NODE_STATE Evaluate(const _float& fTimeDelta) PURE;
public:
	/* �׼��� Leaf ���μ� �ڽ� ��带 ���� �ʴ´�.*/
	HRESULT Add_ChildNode(CNode* pChildNode) override { return E_FAIL; }

private:
	virtual CNode* Clone(void* pArg) PURE;
	virtual void Free();
};

END