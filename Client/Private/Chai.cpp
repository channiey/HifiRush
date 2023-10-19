#include "..\Default\stdafx.h"
#include "..\Public\Chai.h"

#include "GameInstance.h"
#include "Animation.h"

#include "Weapon.h"

/* Genral */
#include "State_Chai_Idle.h"

/* Movement */
#include "State_Chai_Run.h"
#include "State_Chai_Dash.h"
#include "State_Chai_Jump.h"

/* Attack */
#include "State_Chai_Attack.h"
#include "State_Chai_Damaged.h"
#include "State_Chai_Parry.h"


CChai::CChai(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacter(pDevice, pContext)
{
}

CChai::CChai(const CChai& rhs)
	: CCharacter(rhs)
{
}

HRESULT CChai::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CChai::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Chilren()))
		return E_FAIL;
	
	return S_OK;
}

void CChai::Tick(_float fTimeDelta)
{
	if (FAILED(m_pStateMachineCom->Tick(fTimeDelta)))
		return;

	__super::Tick(fTimeDelta);
}

void CChai::LateTick(_float fTimeDelta)
{
	if (FAILED(m_pModelCom->Update(fTimeDelta)))
		return;

	if (FAILED(m_pStateMachineCom->LateTick(fTimeDelta)))
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CChai::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	for (_uint i = 0; i < m_pModelCom->Get_MeshCount(); ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		//if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
		//	return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CChai::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Shader_VTF"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Chai"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
	
	/* Com_Collider_Sphere */
	CCollider_Sphere* pCollider = nullptr;
	{
		CCollider::COLLIDERDESC	ColliderDesc(Vec3{ 0.f, 0.9f, 0.f }, 0.9f);

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			TEXT("Com_Collider_Sphere"), (CComponent**)&pCollider, &ColliderDesc)))
			return E_FAIL;

		m_pColliderComs.push_back(pCollider);
	}

	/* Com_StateMachine */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_StateMachine"),
		TEXT("Com_StateMachine"), (CComponent**)&m_pStateMachineCom)))
		return E_FAIL;
	{	
		CState* pState = nullptr;

		/* General */
		{
			pState = CState_Chai_Idle::Create(m_pStateMachineCom, StateNames[STATE_IDLE], this);
			if (FAILED(m_pStateMachineCom->Add_State(pState)))
				return E_FAIL;
		}
		/* Movement */
		{
			pState = CState_Chai_Run::Create(m_pStateMachineCom, StateNames[STATE_RUN], this);
			if (FAILED(m_pStateMachineCom->Add_State(pState)))
				return E_FAIL;

			pState = CState_Chai_Dash::Create(m_pStateMachineCom, StateNames[STATE_DASH], this);
			if (FAILED(m_pStateMachineCom->Add_State(pState)))
				return E_FAIL;

			pState = CState_Chai_Jump::Create(m_pStateMachineCom, StateNames[STATE_JUMP], this);
			if (FAILED(m_pStateMachineCom->Add_State(pState)))
				return E_FAIL;
		}
		/* Action */
		{
			pState = CState_Chai_Attack::Create(m_pStateMachineCom, StateNames[STATE_ATTACK], this);
			if (FAILED(m_pStateMachineCom->Add_State(pState)))
				return E_FAIL;

			pState = CState_Chai_Damaged::Create(m_pStateMachineCom, StateNames[STATE_DAMAGED], this);
			if (FAILED(m_pStateMachineCom->Add_State(pState)))
				return E_FAIL;

			pState = CState_Chai_Parry::Create(m_pStateMachineCom, StateNames[STATE_PARRY], this);
			if (FAILED(m_pStateMachineCom->Add_State(pState)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CChai::Ready_Chilren()
{
	CWeapon* pChild = nullptr;
	
	pChild = dynamic_cast<CWeapon*>(GAME_INSTNACE->Add_GameObject(LV_PROTO, g_strLayerID[LAYER_WEAPON], L"Weapon_Chai_Guitar_Explore"));
	{
		if (FAILED(Add_Child(pChild)))
			return E_FAIL;

		pChild->Set_Socket(CModel::BONE_SOCKET_RIGHT); 

		CCollider::COLLIDERDESC		ColliderDesc{ Vec3(0, 0, -75), 30.f };

		pChild->Get_Collider_Sphere()->Set_ColliderDesc(ColliderDesc);
	}
	return S_OK;
}

HRESULT CChai::Bind_ShaderResources()
{
	return S_OK;
}

void CChai::OnCollision_Enter(CGameObject* pGameObject)
{
}

void CChai::OnCollision_Stay(CGameObject* pGameObject)
{
}

void CChai::OnCollision_Exit(CGameObject* pGameObject)
{
}

CChai * CChai::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CChai*	pInstance = new CChai(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CChai");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CChai* CChai::Clone(void* pArg)
{
	CChai*	pInstance = new CChai(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CChai");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChai::Free()
{
	__super::Free();

	Safe_Release(m_pStateMachineCom);
}
