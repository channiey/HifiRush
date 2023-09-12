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
	virtual const NODE_STATE Evaluate(const _float& fTimeDelta);

public:
	HRESULT Add_ChildNode(CNode* pChildNode) override;

private:
	virtual CNode* Clone(void* pArg);
	virtual void Free();
};

END		