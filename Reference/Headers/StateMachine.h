#pragma once

/* ���µ��� �����Ѵ�. */
/* �� ���¿����� �ൿ, ����, ���� ���� �� ���¿� �����Ѵ�. */

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CStateMachine final : public CComponent
{
private:
	CStateMachine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CStateMachine(const CStateMachine& rhs);
	virtual ~CStateMachine() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	class CState* Tick(const _float& fTimeDelta);
	class CState* LateTick();

public:
	HRESULT Add_State(const wstring& strStateTag, class CState* pState);

	/*
	
		������Ʈ�� � �����̳ʷ� ��� �������̸�

		��� ������Ʈ �ϰ�, ��ȯ���� ��� ����� ������
	

		Ŭ���̾�Ʈ�� enum�� enum string���� �� ������Ʈ�� ���¸� �����Ѵ�.
		������ ������Ʈ �ӽſ����� map(��Ʈ��, ������Ʈ)�� ������Ʈ�� �����Ѵ�.

		Ȯ���� �� ��Ʈ�� ������

	*/

private:
	map<const wstring, class CState*> m_pStates;
	class CState* m_pCurState = { nullptr };

private:
	const _bool& Has_State(const wstring& strStateTag);
	HRESULT Change_State(class CState* pState);


public:
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END