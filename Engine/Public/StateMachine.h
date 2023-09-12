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
	virtual HRESULT Tick(const _float& fTimeDelta);
	virtual HRESULT LateTick();

public:
	const CState* const Get_CurState() const { return m_pCurState; }
	const wstring& Get_CurStateName() const { return m_pCurState->Get_Name(); }

public:
	/* TODO ������Ʈ �ӽ� �ܺο��� ������Ʈ�� ������ �� �ְ� �ϴ� ���� �´� �ɱ�?*/
	HRESULT Set_State(const wstring& strStateTag);

public:
	HRESULT Add_State(const wstring& strStateTag, class CState* pState);

private:
	map<const wstring, class CState*> m_pStates;
	class CState* m_pCurState = { nullptr };

private:
	const _bool& Has_State(const wstring& strStateTag);
	CState* Find_State(const wstring& strStateTag);
	HRESULT Change_State(const wstring& strStateTag);

public:
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END