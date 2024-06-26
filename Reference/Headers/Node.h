#pragma once
   
/* 행동트리에서 사용하는 노드의 최상위 추상 클래스이다. */

#include "Base.h"
#include "Engine_Defines.h"
#include "BehaviourTree.h"
#include "Blackboard.h"

BEGIN(Engine)

class ENGINE_DLL CNode abstract : public CBase
{

protected:
	CNode();
	CNode(const CNode& rhs);
	virtual ~CNode() = default;

public:
	virtual HRESULT Initialize_Node();
	/* 자식 노드를 순차적으로 실행하고 결과를 반환한다. (행동 트리에서의 Tick 역할) */
	virtual const NODE_STATE Evaluate(const _double& fTimeDelta) PURE;

public:
	CNode*				Get_ParentNode() const { return m_pParentNode; }
	const NODE_STATE&	Get_State() const { return m_eState; }

public:
	void Set_ParentNode(CNode* pNode) { m_pParentNode = pNode; }

public:
	virtual HRESULT Add_ChildNode(CNode* pChildNode) PURE;
	virtual HRESULT Reset_Node();

protected:
	NODE_TYPE		m_eType		= { NODE_TYPE::TYPEEND };
	NODE_STATE		m_eState	= { NODE_STATE::TYPEEND };

	CNode*			m_pParentNode = { nullptr };
	list<CNode*>	m_ChildNodes;

//	CBlackboard*	m_pBlackboard = { nullptr };

protected:
	const _bool		Is_ChildNode() const { return !m_ChildNodes.empty(); }

public:
	virtual void Free();
};

END		