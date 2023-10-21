#pragma once

#include "Client_Defines.h"

#include "Node_Action_Saber.h"

BEGIN(Client)
class CNode_IsAttack_Saber final : public CNode_Action_Saber
{
private:
	CNode_IsAttack_Saber();
	CNode_IsAttack_Saber(const CNode_IsAttack_Saber& rhs);
	virtual ~CNode_IsAttack_Saber() = default;

public:
	HRESULT Initialize_Node(class CBlackboard* pBlackboard);
	virtual const NODE_STATE Evaluate(const _float& fTimeDelta);

private:
	void Attack();
	void Escape();

private:
	const ANIM_SA Change_AttackAnim();
	const ANIM_SA Change_EscapeAnim();

public:
	static CNode_IsAttack_Saber* Create(class CBlackboard* pBlackboard);
	virtual void Free();
};
END
