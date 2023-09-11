#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CMonoBehaviour final : public CComponent
{
protected:
	CMonoBehaviour(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonoBehaviour(const CMonoBehaviour& rhs);
	virtual ~CMonoBehaviour() = default;

public:
	virtual HRESULT Send_Message() { return S_OK; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END