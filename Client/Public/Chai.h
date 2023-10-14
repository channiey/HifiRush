#pragma once

#include "Character.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CChai final : public CCharacter
{
	//enum ChildType { CH_CAMERA, CH_WEAPON_LEFT, CH_WEAPON_RIGHT, CH_PET, CH_UI, CH_END };
	enum ChildType { CH_WEAPON_RIGHT, CH_END };

protected:
	CChai(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CChai(const CCharacter& rhs);
	virtual ~CChai() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_float fTimeDelta);
	virtual void		LateTick(_float fTimeDelta);
	virtual HRESULT		Render();

private:
	HRESULT				Ready_Components();
	HRESULT				Ready_ChildObjects();

private:
	HRESULT				Bind_ShaderResources();

private: 
	CStateMachine*		m_pStateMachineCom = { nullptr };
	
public:
	static CChai* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CCharacter* Clone(void* pArg) override; 
	virtual void Free() override;
};

END