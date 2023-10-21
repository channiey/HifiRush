#pragma once
#include "Character.h"

BEGIN(Engine)
class CBehaviourTree;
END

BEGIN(Client)

class CEnemy abstract : public CCharacter
{
protected:
	CEnemy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEnemy(const CEnemy& rhs);
	virtual ~CEnemy() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_float fTimeDelta);
	virtual void		LateTick(_float fTimeDelta);
	virtual HRESULT		Render();

public:
	virtual void		Set_State(const OBJ_STATE& eState) override;

protected:
	HRESULT				Ready_Components();
	virtual HRESULT		Ready_Chilren()			PURE;
	virtual HRESULT		Ready_BehavoiurTree()	PURE;

protected:
	HRESULT				Bind_ShaderResources();

protected:
	virtual void		OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild = -1) override;

protected:
	CBehaviourTree*		m_pBehaviourTreeCom = { nullptr };

public:
	virtual CEnemy*		Clone(void* pArg) PURE;
	virtual void		Free() override;
};
END
