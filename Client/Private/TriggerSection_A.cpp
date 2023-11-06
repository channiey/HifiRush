#include "..\Default\stdafx.h"
#include "..\Public\TriggerSection_A.h"

#include "EngineInstance.h"
#include "Util_String.h"

#include "BattleManager.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CTriggerSection_A::CTriggerSection_A(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTriggerBattle(pDevice, pContext)
{
}

CTriggerSection_A::CTriggerSection_A(const CTriggerSection_A& rhs)
	: CTriggerBattle(rhs)
	, m_fOriginFov(rhs.m_fOriginFov)
	, m_fBattleFov(rhs.m_fBattleFov)
{
}

HRESULT CTriggerSection_A::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTriggerSection_A::Initialize(void* pArg)
{
	CTriggerDummy::TRIGGER_DESC desc;
	{
		desc.eColType = CCollider::TYPE::SPHERE;
		desc.iIndexAsChild = -1;
		desc.tColDesc.vCenter = Vec3::Zero;
		desc.tColDesc.vSize = Vec3{ 10.f, 10.f, 10.f };
	}

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Chilren(desc)))
		return E_FAIL;

	if (FAILED(Ready_Pool()))
		return E_FAIL;

	return S_OK;
}

void CTriggerSection_A::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bBattle)
		Update_Battle(fTimeDelta);
}

void CTriggerSection_A::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}


HRESULT CTriggerSection_A::Render()
{
	return S_OK;
}

HRESULT CTriggerSection_A::Start_Battle()
{
	/* Set Camera */
	{
		CCamera* pCameraCom = ENGINE_INSTANCE->Get_CurCamera()->Get_Camera();

		if (nullptr == pCameraCom)
			return E_FAIL;

		m_fOriginFov = pCameraCom->Get_ProjDesc().fFovy;
	
		pCameraCom->Lerp_Fov(m_fOriginFov, m_fBattleFov, 1.f, LERP_MODE::SMOOTHER_STEP);
	}
	
	return S_OK;
}

HRESULT CTriggerSection_A::Update_Battle(_double fTimeDelta)
{
	return S_OK;
}

HRESULT CTriggerSection_A::Finish_Battle()
{
	/* Set Camera */
	{
		CCamera* pCameraCom = ENGINE_INSTANCE->Get_CurCamera()->Get_Camera();

		if (nullptr == pCameraCom)
			return E_FAIL;

		pCameraCom->Lerp_Fov(pCameraCom->Get_ProjDesc().fFovy, m_fOriginFov, 1.f, LERP_MODE::SMOOTHER_STEP);
	}

	return S_OK;
}

void CTriggerSection_A::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	CBattleManager::GetInstance()->OnTrigger_Enter(m_strTriggerTag);
}

void CTriggerSection_A::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
}

void CTriggerSection_A::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
}

HRESULT CTriggerSection_A::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"),
		ComponentNames[COM_TRANSFORM], (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTriggerSection_A::Ready_Chilren(CTriggerDummy::TRIGGER_DESC desc)
{
	CTriggerDummy* pTrigger = nullptr;
	{
		pTrigger = dynamic_cast<CTriggerDummy*>(ENGINE_INSTANCE->Add_GameObject(ENGINE_INSTANCE->Get_CurLevelIndex(), LayerNames[LAYER_TRIGGER], L"Trigger_Dummy", &desc));

		if (nullptr != pTrigger)
		{
			if (FAILED(Add_Child(pTrigger)))
				return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CTriggerSection_A::Ready_Pool()
{
	return S_OK;
}

CTriggerSection_A* CTriggerSection_A::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTriggerSection_A* pInstance = new CTriggerSection_A(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTriggerSection_A::Clone(void* pArg)
{
	CTriggerSection_A* pInstance = new CTriggerSection_A(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTriggerSection_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CTriggerSection_A::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);

}