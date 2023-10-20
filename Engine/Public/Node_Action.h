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
	virtual HRESULT Initialize_Node();
	virtual const NODE_STATE Evaluate(const _float& fTimeDelta) PURE;

public:
	HRESULT Add_ChildNode(CNode* pChildNode) override { return E_FAIL; }
	virtual HRESULT Reset_Node() override;

public:
	virtual void Free();
};

END