#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;
class CRenderer;
class CTransform;
END

BEGIN(Client)

class CTriggerBattle abstract : public CGameObject
{
public:
	typedef struct  tagCloneDesc
	{
		_uint	iFlowLevel		= { 0 };
		wstring strPrototypeTag = {};
		_uint	iCellIndex		= { 0 };
		Matrix	matWorld		= {};

	}CLONE_DESC;

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

	const _bool&		Is_StartBattle() const { return m_bStartBattle; }

public: /* 캐시 데이터 */
	HRESULT				Add_Clone(const _uint iFlowLevel, CGameObject* pObject);
	HRESULT				Clear_Clone();

public: /* 정규 데이터 */
	HRESULT				Set_TriggerData();
	HRESULT				Clear_TriggerData();

public:
	HRESULT				Save();
	HRESULT				Load();

protected:
	HRESULT				Ready_Components();

private:
	CGameObject*		Find_Object(CGameObject* pObject);

protected:
	CTransform*			m_pTransformCom	= { nullptr };
	CCollider_Sphere*	m_pCollider		= { nullptr };
	CRenderer*			m_pRendererCom	= { nullptr };

protected:
	wstring				m_strTriggerTag = {};
	_bool				m_bBattle		= FALSE;
	_bool				m_bStartBattle	= FALSE;

	map<_uint, list<CGameObject*>>	m_CacheFlows;
	map<_uint, list<CLONE_DESC>>	m_Flows;

	vector<vector<CGameObject*>>	m_Clones;
	_int							m_iCurFlow = 0;
	
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END