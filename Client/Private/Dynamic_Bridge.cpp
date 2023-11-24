#include "..\Default\stdafx.h"
#include "Dynamic_Bridge.h"

#include "EngineInstance.h"
#include "Animation.h"

#include "Character.h"
#include "Projectile.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CDynamic_Bridge::CDynamic_Bridge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CDynamic(pDevice, pContext)
{
}

CDynamic_Bridge::CDynamic_Bridge(const CDynamic_Bridge& rhs)
	: CDynamic(rhs)
{
}

HRESULT CDynamic_Bridge::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDynamic_Bridge::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	CAnimation* pAnim = m_pModelCom->Get_Animation(m_strAnimName);

	if (nullptr == pAnim) return E_FAIL;

	m_pModelCom->Set_Animation(pAnim, pAnim->Get_TickPerFrame(), DF_TW_TIME);

	m_pModelCom->Stop_Animation(TRUE);

	return S_OK;
}

void CDynamic_Bridge::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CDynamic_Bridge::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (!m_pModelCom->Is_Tween() && 100 == m_pModelCom->Get_TweenDesc().cur.iCurFrame)
		m_pModelCom->Stop_Animation(TRUE);
}

HRESULT CDynamic_Bridge::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CDynamic_Bridge::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	if (L"Player_Chai_000" == pCollider->Get_Owner()->Get_Name())
	{
		Set_On();
	}
}

void CDynamic_Bridge::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
}

void CDynamic_Bridge::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
}

HRESULT CDynamic_Bridge::Set_On()
{
	m_pModelCom->Stop_Animation(FALSE);

	return S_OK;
}

HRESULT CDynamic_Bridge::Set_Off()
{
	m_pModelCom->Clear_Animation();

	CAnimation* pAnim = m_pModelCom->Get_Animation(m_strAnimName);

	if (nullptr == pAnim) return E_FAIL;

	m_pModelCom->Set_Animation(pAnim, pAnim->Get_TickPerFrame(), DF_TW_TIME);

	m_pModelCom->Stop_Animation(TRUE);

	return S_OK;
}

void CDynamic_Bridge::Check_Progress(_double fTimeDelta)
{
}

HRESULT CDynamic_Bridge::Ready_Components()
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Dynamic_Bridge"),
		ComponentNames[COM_MODEL], (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_Collider */
	CCollider::COLLIDERDESC	ColliderDesc(Vec3{ 0.f, 5.f, 0.f }, 5.f);
	{
		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			ComponentNames[COM_COLLIDER_SPHERE], (CComponent**)&m_pCollider, &ColliderDesc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CDynamic_Bridge::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::STATE_VIEW)))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::STATE_PROJ)))
		return E_FAIL;

	return S_OK;
}

CDynamic_Bridge* CDynamic_Bridge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDynamic_Bridge* pInstance = new CDynamic_Bridge(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDynamic_Bridge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CDynamic_Bridge* CDynamic_Bridge::Clone(void* pArg)
{
	CDynamic_Bridge* pInstance = new CDynamic_Bridge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDynamic_Bridge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDynamic_Bridge::Free()
{
	__super::Free();
}
