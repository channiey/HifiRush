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
	
		스테이트를 어떤 컨테이너로 들고 있을것이며

		어떻게 업데이트 하고, 반환값은 어떻게 사용할 것인지
	

		클라이언트에 enum과 enum string으로 각 오브젝트별 상태를 정의한다.
		엔진의 스테이트 머신에서는 map(스트링, 스테이트)로 스테이트를 관리한다.

		확인할 것 스트링 컨버팅

	*/


public:
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END