#pragma once

#include "Node.h"

BEGIN(Engine)

/*
	������ Ŭ�п��� ����ϴ� ��Ʈ, ������, ���ڷ����ʹ� ������ Ŭ���� ������Ʈó�� ������ �Ѵ�.

	�׼��� �׷� �ʿ� ����
*/

class ENGINE_DLL CNode_Root final : public CNode
{

protected:
	CNode_Root();
	CNode_Root(const CNode_Root& rhs);
	virtual ~CNode_Root() = default;

public:
	virtual HRESULT Initialize_Node(class CBlackboard* pBlackboard);
	virtual const NODE_STATE Evaluate(const _float& fTimeDelta);

public:
	HRESULT Add_ChildNode(CNode* pChildNode) override;

public:
	static CNode_Root* Create(class CBlackboard* pBlackboard);
	virtual void Free();
};

END		