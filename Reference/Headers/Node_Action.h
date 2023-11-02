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
	virtual HRESULT Initialize_Node();
	virtual const NODE_STATE Evaluate(const _double& fTimeDelta) PURE;

public:
	HRESULT Add_ChildNode(CNode* pChildNode) override { return E_FAIL; }
	virtual HRESULT Reset_Node() override;

protected:
	virtual const _bool Check_Condition(const _double& fTimeDelta) { return TRUE; }

public:
	virtual void Free();
};

END