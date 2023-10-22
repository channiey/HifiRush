#pragma once

#include "Client_Defines.h"

#include "Node_Action.h"

#include "Blackboard_Saber.h"

BEGIN(Client)
class CNode_Action_Saber abstract : public CNode_Action
{
protected:
	CNode_Action_Saber();
	CNode_Action_Saber(const CNode_Action_Saber& rhs);
	virtual ~CNode_Action_Saber() = default;

public:
	HRESULT Initialize_Node(class CBlackboard* pBlackboard);

public:
	HRESULT Add_ChildNode(CNode* pChildNode) override { return E_FAIL; }

protected:
	const _bool Is_Playing_Animation();

protected:
	CBlackboard_Saber* m_pBlackboard_Saber = { nullptr };

	ANIM_SA				m_eCurAnim = ANIM_SA::TYPEEND;

public:
	virtual void Free();
};
END
