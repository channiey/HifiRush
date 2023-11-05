#pragma once

/* 상태들을 관리한다. */
/* 각 상태에서의 행동, 조건, 전이 등은 각 상태에 정의한다. */

#include "Component.h"

BEGIN(Engine)

class CState;

class ENGINE_DLL CStateMachine final : public CComponent
{
private:
	CStateMachine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CStateMachine(const CStateMachine& rhs);
	virtual ~CStateMachine() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Tick(const _double& fTimeDelta);
	virtual HRESULT LateTick(const _double& fTimeDelta);

public:
	CState*			Get_CurState() const { return m_pCurState; }
	HRESULT			Set_State(const wstring& strStateTag);
	HRESULT			Add_State(CState* pState);

private:
	const _bool		Has_State(const wstring& strStateTag);
	CState*			Find_State(const wstring& strStateTag);
	HRESULT			Change_State(const wstring& strStateTag);

private:
	map<const wstring, class CState*>	m_pStates;
	CState*								m_pCurState = { nullptr };

public:
	static CStateMachine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END