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
	virtual HRESULT Initialize_Node();
	virtual const NODE_STATE Evaluate(const _double& fTimeDelta);

public:
	HRESULT Add_ChildNode(CNode* pChildNode) override;
	virtual HRESULT Reset_Node() override;

private:
	list<CNode*>::iterator m_iterRunning = m_ChildNodes.end();

public:
	static CNode_Sequence* Create();
	virtual void Free();
};

END