#pragma once

#include "Component.h"

BEGIN(Engine)

enum class NODE_STATE
{
	SUCCESS,
	FAILURE,
	RUNNING, 
	TYPEEND

	/* RUNNING 
	* 
		������ �� Ȥ�� �ڽ� ������ �򰡰� �������� �ȿ��� ó���� �� ���� ��� ������ �帧�� �������� �ʰ�

		���� �����ӿ��� �ٽ� üũ�ϱ� ���� ���ȴ�. 
	*/
	
};

enum class NODE_TYPE
{
	/* Root */
	/* �ڽ� ���� �� �Ѱ��� ������(�ַ� ������)�� ���´�. */
	ROOT,

	/* Composit */
	/* ���� ���� �ڽ� ���(Action, Composit, Decorator)�� ���� �� �ִ�.*/
	SEQUENCE,	/* Child�� �ϳ��� Failure or Running�� ��ȯ�� ������ �ݺ��ؼ� ���� */
				/* �ټ��� �۾��� ���ʴ�� �����ϵ� ���� �ϳ��� �۵����� ������ �۾� ��ü�� ��� �� �� */
	SELECTOR,	/* Child�� �ϳ��� Success or Running�� ��ȯ�� ������ �ݺ��ؼ� ���� -> �� �ϳ��� ���� */
				/* � �� �ʿ����� ������ ���� ������ �۾��̳� �귣ġ�� ������ �� */
	PARALLEL,	/* Child�� � ���� ��ȯ�ϵ�, ��� child�� ���������� �����Ѵ�. */


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
	WAIT,

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
	virtual HRESULT LateTick(const _float& fTimeDelta);

public:
	const NODE_STATE&	Get_State() const { return m_eState; }
	class CNode* 		Get_RootNode() const { return m_pRootNode; }
	class CBlackboard* 	Get_Blackboard() const { return m_pBlackboard; }

public:
	HRESULT				Set_RootNode(class CNode* const pRootNode);
	HRESULT				Set_Blackboard(class CBlackboard* const pBlackboard);

	virtual HRESULT		Set_Active(const _bool& bActive) override;

private:
	NODE_STATE			m_eState = { NODE_STATE::RUNNING };
	class CNode*		m_pRootNode = { nullptr };
	class CBlackboard*	m_pBlackboard = { nullptr };

public:
	static CBehaviourTree* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END