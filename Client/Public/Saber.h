#pragma once
#include "Character.h"

BEGIN(Engine)
class CBehaviourTree;
END


enum ANIM_SA
{

	ANIM_END,
};

enum STATE_SA
{
	STATE_ONE,
	STATE_END
};

const wstring StateNames[STATE_SA::STATE_END]
{
	L"IDLE"
};

class CSaber final : public CCharacter
{
	enum CHILD_TYPE { SA_WEAPON_RIGHT, SA_END };

private:
	CSaber(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSaber(const CSaber& rhs);
	virtual ~CSaber() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_float fTimeDelta);
	virtual void		LateTick(_float fTimeDelta);
	virtual HRESULT		Render();

private:
	HRESULT				Ready_Components();
	HRESULT				Ready_Chilren();

private:
	HRESULT				Bind_ShaderResources();

private:
	virtual void		OnCollision_Enter(CGameObject* pGameObject) override;
	virtual void		OnCollision_Stay(CGameObject* pGameObject) override;
	virtual void		OnCollision_Exit(CGameObject* pGameObject) override;

private:
	CBehaviourTree* m_pBehaviourTreeCom = { nullptr };

public:
	static CSaber* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CSaber* Clone(void* pArg) override;
	virtual void Free() override;

private:
	
};

