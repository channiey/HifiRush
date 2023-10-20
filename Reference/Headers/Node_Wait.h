#pragma once

/* 개별 액션은 클라이언트에서 구현한다. */

#include "Node_Action.h"

BEGIN(Engine)

class ENGINE_DLL CNode_Wait final : public CNode_Action
{
private:
	CNode_Wait();
	CNode_Wait(const CNode_Wait& rhs);
	virtual ~CNode_Wait() = default;

public:
	HRESULT Initialize_Node(class CBlackboard* pBlackboard, const _float& fTime);
	virtual const NODE_STATE Evaluate(const _float& fTimeDelta);

public:
	/* 액션은 Leaf 노드로서 자식 노드를 갖지 않는다.*/
	HRESULT Add_ChildNode(CNode* pChildNode) override { return E_FAIL; }
	virtual HRESULT Reset_Node() override;

private:
	_float m_fAcc = { 0.f };
	_float m_fLimitAcc = { 0.f };

public:
	static CNode_Wait* Create(class CBlackboard* pBlackboard, const _float& fTime);
	virtual void Free();
};

END