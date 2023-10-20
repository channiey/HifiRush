#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "Collider.h"

BEGIN(Engine)
class CCollider;
class CRenderer;
class CTransform;
END

BEGIN(Client)

class CTriggerDummy final : public CGameObject
{
public:
	typedef struct tagTriggerDesc
	{
		CCollider::COLLIDERDESC	 tColDesc = {};

		CCollider::TYPE			 eColType = CCollider::TYPE_END;

		_int					 iIndexAsChild = -1;

		tagTriggerDesc() {};
		tagTriggerDesc(const CCollider::COLLIDERDESC& _tColDesc, const CCollider::TYPE& _eColType, const _int& _iIndexAsChild)
			: tColDesc(_tColDesc), eColType(_eColType), iIndexAsChild(_iIndexAsChild) {};

	}TRIGGER_DESC;

private:
	CTriggerDummy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTriggerDummy(const CTriggerDummy& rhs);
	virtual ~CTriggerDummy() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_float fTimeDelta);
	virtual void		LateTick(_float fTimeDelta);
	virtual HRESULT		Render();

private:
	HRESULT				Ready_Components();

private:
	CTransform*			m_pTransformCom = nullptr;
	CCollider*			m_pCollider = nullptr;
	CRenderer*			m_pRendererCom = { nullptr };

	TRIGGER_DESC		m_tDesc = {};

public:
	static CTriggerDummy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END