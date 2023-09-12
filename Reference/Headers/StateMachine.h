#pragma once

/* 상태들을 관리한다. */
/* 각 상태에서의 행동, 조건, 전이 등은 각 상태에 정의한다. */

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
	
		스테이트를 어떤 컨테이너로 들고 있을것이며

		어떻게 업데이트 하고, 반환값은 어떻게 사용할 것인지
	

		클라이언트에 enum과 enum string으로 각 오브젝트별 상태를 정의한다.
		엔진의 스테이트 머신에서는 map(스트링, 스테이트)로 스테이트를 관리한다.

		확인할 것 스트링 컨버팅

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