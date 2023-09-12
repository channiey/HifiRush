#pragma once

#include "Component.h"

BEGIN(Engine)

enum class NODE_STATE
{
	SUCCESS,
	FAILURE,
	RUNNING,
	TYPEEND
};

enum class NODE_TYPE
{
	/* Root */
	ROOT,

	/* Composit */
	/* ���� ���� �ڽĳ�带 ���� �� �ִ�.*/
	SEQUENCE, /* Child�� �ϳ��� Failure or Running�� ��ȯ�� ������ ����*/
	SELECTOR, /* Child�� �ϳ��� Success or Running�� ��ȯ�� ������ ����*/
	PARALLEL, /* Child�� � ���� ��ȯ�ϵ�, ��� child�� ���������� �����Ѵ�. */


	/* Decorator */
	/* �ڽ� ��尡 ����(����) ��� ��������� �����Ѵ�. */
	/* �� �ϳ��� �ڽ� ��带 ������ �� ���� �׼� ����̴�. */
	IF,
	WHILE,
	SUCCEEDER,
	FAILURER,
	REPEATER,

	/* Action */
	ACTION, /* Lead Node�μ� ���� �ൿ�� ����Ѵ�. */

	// WAIT_TO_TIME (���� ���Ǵ� ��ɵ��� �̸� ������ �����صδ� ��)
	// MOVETO

	TYPEEND
};

class ENGINE_DLL CBehaviourTree final : public CComponent
{
private:
	CBehaviourTree(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CBehaviourTree(const CBehaviourTree& rhs);
	virtual ~CBehaviourTree() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Tick(const _float& fTimeDelta);
	/* TODO �̰� �� ƽ�� ����Ʈƽ���� ���� �ι��̳� ������ �ұ�?*/
	virtual HRESULT LateTick(const _float& fTimeDelta);

public:
	const NODE_STATE& Get_State() const { return m_eState; }
	class CNode* const Get_RootNode() const { return m_pRootNode; }
	class CBlackboard* const Get_Blackboard() const { return m_pBlackboard; }

public:
	HRESULT Set_RootNode(class CNode* const pRootNode);
	HRESULT Set_Blackboard(class CBlackboard* const pBlackboard);

	virtual HRESULT Set_Active(const _bool& bActive) override;

private:
	NODE_STATE m_eState = { NODE_STATE::RUNNING };
	class CNode* m_pRootNode = { nullptr };
	class CBlackboard* m_pBlackboard = { nullptr };

public:
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END