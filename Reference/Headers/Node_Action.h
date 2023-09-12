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
	virtual const NODE_STATE Tick(const _float& fTimeDelta) PURE;
public:
	HRESULT Add_ChildNode(CNode* pChildNode) override { return E_FAIL; }

private:
	virtual CNode* Clone(void* pArg) PURE;
	virtual void Free();
};

END