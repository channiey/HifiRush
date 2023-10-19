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
	virtual HRESULT Initialize_Node();
	virtual const NODE_STATE Evaluate(const _float& fTimeDelta);

public:
	HRESULT Add_ChildNode(CNode* pChildNode) override;

private:
	static CNode_Root* Create(void* pArg = nullptr);
	virtual void Free();
};

END		