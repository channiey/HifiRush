#include "..\Default\stdafx.h"
#include "Saber.h"

#include "EngineInstance.h"
#include "Animation.h"

#include "Weapon.h"
#include "TriggerDummy.h"

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

	/* 임시 코드 */
	m_pModelCom->Set_Animation(ANIM_SA::IDLE_ATTACK);

	return S_OK;
}

void CSaber::Tick(_double fTimeDelta)
{
	/* 임시 코드 */
	if (nullptr == m_tFightDesc.pTarget)
	{
		CGameObject* pTarget = ENGINE_INSTANCE->Get_GameObject_InCurLevel_InLayerFirst(LayerNames[LAYER_PLAYER]);

		if (nullptr != pTarget)
			m_tFightDesc.pTarget = dynamic_cast<CCharacter*>(pTarget);
	}

	__super::Tick(fTimeDelta);
}

void CSaber::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
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

	/*CTriggerDummy* pTrigger = nullptr;
	{
		CCollider::COLLIDERDESC		ColliderDesc{ Vec3(0, 0, 0), 5.f };
		CTriggerDummy::TRIGGER_DESC TriggerDesc(ColliderDesc, CCollider::SPHERE, CHILD_TYPE::SA_TRIGGER_TRACKED);
		pTrigger = dynamic_cast<CTriggerDummy*>(ENGINE_INSTANCE->Add_GameObject(LV_PROTO, LayerNames[LAYER_TRIGGER], L"Trigger_Dummy", &TriggerDesc));
		
		if (FAILED(Add_Child(pTrigger)))
			return E_FAIL;
	}*/
	return S_OK;
}

HRESULT CSaber::Ready_StateMachine()
{
	return E_NOTIMPL;
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
