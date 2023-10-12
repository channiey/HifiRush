#pragma once

#include "Character.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class Peppermint final : public CCharacter
{
protected:
	Peppermint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Peppermint(const CCharacter& rhs);

	virtual ~Peppermint() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_float fTimeDelta);
	virtual void		LateTick(_float fTimeDelta);
	virtual HRESULT		Render();

private:
	CStateMachine* m_pStateMachineCom = { nullptr };
	
private:
	HRESULT				Ready_Components();
	HRESULT				Bind_ShaderResources();

public:
	static Peppermint* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CCharacter* Clone(void* pArg) override; 
	virtual void Free() override;
};

END