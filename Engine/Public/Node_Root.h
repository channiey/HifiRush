#pragma once

#include "Node.h"

BEGIN(Engine)

/*
	원형을 클론에서 사용하는 루트, 컴포짓, 데코레이터는 원형과 클론을 컴포넌트처럼 만들어야 한다.

	액션은 그럴 필요 없음
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