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
	/* 여러 개의 자식노드를 가질 수 있다.*/
	SEQUENCE, /* Child중 하나라도 Failure or Running을 반환할 때까지 실행*/
	SELECTOR, /* Child중 하나라도 Success or Running을 반환할 때까지 실행*/
	PARALLEL, /* Child가 어떤 값을 반환하든, 모든 child를 순차적으로 실행한다. */


	/* Decorator */
	/* 자식 노드가 언제(조건) 어떻게 실행될지를 결정한다. */
	/* 단 하나의 자식 노드를 가지며 그 노드는 액션 노드이다. */
	IF,
	WHILE,
	SUCCEEDER,
	FAILURER,
	REPEATER,

	/* Action */
	ACTION, /* Lead Node로서 실제 행동을 담당한다. */

	// WAIT_TO_TIME (자주 사용되는 기능들은 미리 엔진에 정의해두는 편)
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
	/* TODO 이걸 꼭 틱과 레이트틱으로 나눠 두번이나 돌려야 할까?*/
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