#include "..\Default\stdafx.h"
#include "..\Public\TriggerBattle.h"
#include "EngineInstance.h"

#include "Util_String.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CTriggerBattle::CTriggerBattle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CTriggerBattle::CTriggerBattle(const CTriggerBattle& rhs)
	: CGameObject(rhs)
{
}

HRESULT CTriggerBattle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTriggerBattle::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	CTriggerDummy::TRIGGER_DESC desc;

	memmove(&desc, pArg, sizeof(CTriggerDummy::TRIGGER_DESC));

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Chilren(desc)))
		return E_FAIL;

	return S_OK;
}

void CTriggerBattle::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CTriggerBattle::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}


HRESULT CTriggerBattle::Render()
{
	return S_OK;
}

HRESULT CTriggerBattle::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"),
		ComponentNames[COM_TRANSFORM], (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTriggerBattle::Ready_Chilren(CTriggerDummy::TRIGGER_DESC desc)
{
	CTriggerDummy* pTrigger = nullptr;
	{
		pTrigger = dynamic_cast<CTriggerDummy*>(ENGINE_INSTANCE->Add_GameObject(LV_PROTO, LayerNames[LAYER_TRIGGER], L"Trigger_Dummy", &desc));
		
		if (nullptr != pTrigger)
		{
			if (FAILED(Add_Child(pTrigger)))
				return E_FAIL;
		}
	}
	return S_OK;
}

void CTriggerBattle::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);

}
