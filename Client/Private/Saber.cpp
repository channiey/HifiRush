#include "..\Default\stdafx.h"
#include "Saber.h"

#include "EngineInstance.h"
#include "Animation.h"

#include "Weapon.h"

#include "State_Saber_Base.h"
#include "ImGui_Manager.h"

CSaber::CSaber(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEnemy(pDevice, pContext)
{
}

CSaber::CSaber(const CSaber& rhs)
	: CEnemy(rhs)
{
}

HRESULT CSaber::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSaber::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Chilren()))
		return E_FAIL;

	if (FAILED(Ready_StateMachine()))
		return E_FAIL;

	_uint			eAnimID = 0;
	CAnimation*		pAnimation = m_pModelCom->Get_Animation(eAnimID);
	const _double	fTimePerFrame = CBeatManager::GetInstance()->Get_AnimTimePerFrame(pAnimation);

	m_pModelCom->Set_Animation(eAnimID, fTimePerFrame * (_double)2.f, DF_TW_TIME);

	return S_OK;
}

void CSaber::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CSaber::LateTick(_double fTimeDelta)
{
	// << : Test 

	if (!CImGui_Manager::GetInstance()->Is_DebugCam())
	{
		m_pTransformCom->Translate(m_pTransformCom->Get_Forward() * m_tPhysicsDesc.fMaxForwardSpeed * 0.1f * fTimeDelta);

		if (FAILED(m_pModelCom->Update(fTimeDelta)))
			return;

	}

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
	// >> : 
	// 
	// 
	//__super::LateTick(fTimeDelta);
}

HRESULT CSaber::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CSaber::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);
}

HRESULT CSaber::Ready_Components()
{
	/* Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Saber"),
		ComponentNames[COM_MODEL], (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_Collider */
	CCollider_Sphere* pCollider = nullptr;
	{
		CCollider::COLLIDERDESC	ColliderDesc(Vec3{ 0.f, 0.9f, 0.f }, 0.9f);

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			ComponentNames[COM_COLLIDER_SPHERE], (CComponent**)&pCollider, &ColliderDesc)))
			return E_FAIL;

		m_pColliderComs.push_back(pCollider);
	}

	return S_OK;
}

HRESULT CSaber::Ready_Chilren()
{
	CWeapon* pWeapon = nullptr;
	{
		pWeapon = dynamic_cast<CWeapon*>(ENGINE_INSTANCE->Add_GameObject(LV_PROTO, LayerNames[LAYER_WEAPON], L"Weapon_Saber_Sword"));
	
		if (FAILED(Add_Child(pWeapon)))
			return E_FAIL;

		pWeapon->Set_Socket(CModel::BONE_SOCKET_RIGHT);
		pWeapon->Set_IndexAsChild(CHILD_TYPE::SA_WEAPON_RIGHT);
	}

	return S_OK;
}

HRESULT CSaber::Ready_StateMachine()
{
	CState* pState = nullptr;

	/* General */
	{
		/*pState = CState_Chai_Idle::Create(m_pStateMachineCom, StateNames_CH[STATE_IDLE], this);
		if (FAILED(m_pStateMachineCom->Add_State(pState)))
			return E_FAIL;*/
	}

	return S_OK;
}

void CSaber::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	__super::OnCollision_Enter(pCollider, iIndexAsChild);

	CGameObject* pGameObject = pCollider->Get_Owner();

	if (iIndexAsChild == SA_WEAPON_RIGHT)
	{
		if (LayerNames[LAYER_PLAYER] == pGameObject->Get_LayerTag())
			KnockBack(pGameObject);
	}

}

void CSaber::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
	__super::OnCollision_Stay(pCollider, iIndexAsChild);
}

void CSaber::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
	__super::OnCollision_Exit(pCollider, iIndexAsChild);
}

CSaber* CSaber::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSaber* pInstance = new CSaber(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSaber");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CSaber* CSaber::Clone(void* pArg)
{
	CSaber* pInstance = new CSaber(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSaber");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSaber::Free()
{
	__super::Free();
}
