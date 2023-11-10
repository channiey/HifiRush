#include "..\Default\stdafx.h"
#include "Blader.h"

#include "ImGui_Manager.h"

#include "EngineInstance.h"
#include "Animation.h"

CBlader::CBlader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEnemy(pDevice, pContext)
{
}

CBlader::CBlader(const CBlader& rhs)
	: CEnemy(rhs)
{
}

HRESULT CBlader::Initialize_Prototype()
{
	m_eEnemyType = ENEMY_TYPE::NORMAL;

	return S_OK;
}

HRESULT CBlader::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Chilren()))
		return E_FAIL;

	if (FAILED(Ready_StateMachine()))
		return E_FAIL;

	m_pModelCom->Set_Animation(5, (1.f / 24.f), DF_TW_TIME);

	return S_OK;
}

void CBlader::Tick(_double fTimeDelta)
{
	//__super::Tick(fTimeDelta);
}

void CBlader::LateTick(_double fTimeDelta)
{
	if (!CImGui_Manager::GetInstance()->Is_DebugCam())
	{
		if (FAILED(m_pModelCom->Update(fTimeDelta)))
			return;
	}
	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;

	//__super::LateTick(fTimeDelta);
}

HRESULT CBlader::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CBlader::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);

	if (OBJ_STATE::STATE_ACTIVE == eState)
	{
		//m_pStateMachineCom->Set_State(StateNames_SA[STATE_APPEAR_SA]);
	}
}

HRESULT CBlader::Ready_Components()
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Blader"),
		ComponentNames[COM_MODEL], (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_Collider */
	CCollider_Sphere* pCollider = nullptr;
	{
		CCollider::COLLIDERDESC	ColliderDesc(Vec3{ 0.f, 1.f, 0.f }, 1.f);

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			ComponentNames[COM_COLLIDER_SPHERE], (CComponent**)&pCollider, &ColliderDesc)))
			return E_FAIL;

		m_pColliderComs.push_back(pCollider);
	}

	return S_OK;
}

HRESULT CBlader::Ready_Chilren()
{
	return S_OK;
}

HRESULT CBlader::Ready_StateMachine()
{
	CState* pState = nullptr;

	return S_OK;
}

void CBlader::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	__super::OnCollision_Enter(pCollider, iIndexAsChild);
}

void CBlader::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
	__super::OnCollision_Stay(pCollider, iIndexAsChild);
}

void CBlader::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
	__super::OnCollision_Exit(pCollider, iIndexAsChild);
}

CBlader* CBlader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBlader* pInstance = new CBlader(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBlader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBlader* CBlader::Clone(void* pArg)
{
	CBlader* pInstance = new CBlader(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBlader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlader::Free()
{
	__super::Free();
}
