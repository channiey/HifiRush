#pragma once

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

	/*
	
		������Ʈ�� � �����̳ʷ� ��� �������̸�

		��� ������Ʈ �ϰ�, ��ȯ���� ��� ����� ������
	

		Ŭ���̾�Ʈ�� enum�� enum string���� �� ������Ʈ�� ���¸� �����Ѵ�.
		������ ������Ʈ �ӽſ����� map(��Ʈ��, ������Ʈ)�� ������Ʈ�� �����Ѵ�.

		Ȯ���� �� ��Ʈ�� ������

	*/


public:
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END