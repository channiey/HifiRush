#pragma once
   
/* �ൿƮ������ ����ϴ� ����� �ֻ��� �߻� Ŭ�����̴�. */

#include "Base.h"
#include "Engine_Defines.h"
#include "BehaviourTree.h"

BEGIN(Engine)

class ENGINE_DLL CNode abstract : public CBase
{

protected:
	CNode();
	CNode(const CNode& rhs);
	virtual ~CNode() = default;

public:
	/* �ڽ� ��带 ���������� �����ϰ� ����� ��ȯ�Ѵ�. (�ൿ Ʈ�������� Tick ����) */
	virtual const NODE_STATE Evaluate(const _float& fTimeDelta) PURE;

public:
	void Set_Name(const wstring& strName) { m_strName = strName; }

public:
	virtual HRESULT Add_ChildNode(CNode* pChildNode) PURE;

protected:
	NODE_STATE m_eState = { NODE_STATE::TYPEEND };
	NODE_TYPE m_eType = { NODE_TYPE::TYPEEND };
	list<CNode*> m_pChildNodes;
	wstring m_strName = {};

protected:
	const _bool Is_ChildNode() const { return m_pChildNodes.empty(); }

public:
	virtual void Free();
};

END		