#pragma once
   
/* �ൿƮ������ ����ϴ� ����� �ֻ��� �߻� Ŭ�����̴�. */

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
	/* �ڽ� ��带 ���������� �����ϰ� ����� ��ȯ�Ѵ�. (�ൿ Ʈ�������� Tick ����) */
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