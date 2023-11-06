#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "Collider.h"

#include "TriggerDummy.h"

BEGIN(Client)

class CTriggerBattle abstract : public CGameObject
{
protected:
	CTriggerBattle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTriggerBattle(const CTriggerBattle& rhs);
	virtual ~CTriggerBattle() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_double fTimeDelta);
	virtual void		LateTick(_double fTimeDelta);
	virtual HRESULT		Render();

public:
	virtual HRESULT		Start_Battle() PURE;
	virtual HRESULT		Update_Battle(_double fTimeDelta) PURE;
	virtual HRESULT		Finish_Battle() PURE;

public:
	const wstring&		Get_TriggerTag() const { return m_strTriggerTag; }

	void				Set_TriggerTag(const wstring strTriggerTag) { m_strTriggerTag = strTriggerTag; }

protected:
	HRESULT				Ready_Components();
	HRESULT				Ready_Chilren(CTriggerDummy::TRIGGER_DESC desc);
	virtual HRESULT		Ready_Pool() PURE;

protected:
	CTransform*			m_pTransformCom = { nullptr };

protected:
	wstring				m_strTriggerTag = {};
	_bool				m_bBattle		= FALSE;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END