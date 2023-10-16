#include "..\Default\stdafx.h"
#include "..\Public\Chai.h"

#include "GameInstance.h"

#include "Weapon.h"

/* Genral */
#include "State_Chai_Idle.h"

/* Movement */
#include "State_Chai_Run.h"
#include "State_Chai_Dash.h"
#include "State_Chai_Jump.h"
#include "State_Chai_DoubleJump.h"

/* Attack */
#include "State_Chai_Attack_1.h"
#include "State_Chai_Damaged.h"
#include "State_Chai_Parry.h"

CChai::CChai(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCharacter(pDevice, pContext)
{
}

CChai::CChai(const CCharacter& rhs)
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
	
	/* Com_StateMachine */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_StateMachine"),
		TEXT("Com_StateMachine"), (CComponent**)&m_pStateMachineCom)))
		return E_FAIL;
	{
		CState* pState = nullptr;

		/* General */
		{
			pState = CState_Chai_Idle::Create(m_pStateMachineCom, StateNames_CH[STATE_CH::IDLE], this);
			if (FAILED(m_pStateMachineCom->Add_State(pState)))
				return E_FAIL;
		}
		/* Movement */
		{
			pState = CState_Chai_Run::Create(m_pStateMachineCom, StateNames_CH[STATE_CH::RUN], this);
			if (FAILED(m_pStateMachineCom->Add_State(pState)))
				return E_FAIL;

			pState = CState_Chai_Dash::Create(m_pStateMachineCom, StateNames_CH[STATE_CH::DASH], this);
			if (FAILED(m_pStateMachineCom->Add_State(pState)))
				return E_FAIL;

			pState = CState_Chai_Jump::Create(m_pStateMachineCom, StateNames_CH[STATE_CH::JUMP], this);
			if (FAILED(m_pStateMachineCom->Add_State(pState)))
				return E_FAIL;

			pState = CState_Chai_DoubleJump::Create(m_pStateMachineCom, StateNames_CH[STATE_CH::DOUBLEJUMP], this);
			if (FAILED(m_pStateMachineCom->Add_State(pState)))
				return E_FAIL;
		}
		/* Action */
		{
			pState = CState_Chai_Attack_1::Create(m_pStateMachineCom, StateNames_CH[STATE_CH::ATTACK_1], this);
			if (FAILED(m_pStateMachineCom->Add_State(pState)))
				return E_FAIL;

			pState = CState_Chai_Damaged::Create(m_pStateMachineCom, StateNames_CH[STATE_CH::DAMAGED], this);
			if (FAILED(m_pStateMachineCom->Add_State(pState)))
				return E_FAIL;

			pState = CState_Chai_Damaged::Create(m_pStateMachineCom, StateNames_CH[STATE_CH::PARRY], this);
			if (FAILED(m_pStateMachineCom->Add_State(pState)))
				return E_FAIL;
		}
	}

	/* Com_Collider_Sphere */
	CCollider_Sphere* pCollider = nullptr;
	{
		CCollider::COLLIDERDESC	ColliderDesc{ 1.f };

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			TEXT("Com_Collider_Sphere"), (CComponent**)&pCollider, &ColliderDesc)))
			return E_FAIL;
		
		m_pColliderComs.push_back(pCollider);
	}

	return S_OK;
}

HRESULT CChai::Ready_Chilren()
{
	CWeapon* pChild = nullptr;
	
	pChild = dynamic_cast<CWeapon*>(GAME_INSTNACE->Add_GameObject(LV_STATIC, g_StrLayerID[LAYER_WEAPON], L"Weapon_Chai_Guitar_Explore"));
	{
		if (FAILED(Add_Child(pChild)))
			return E_FAIL;

		pChild->Set_Socket(CModel::BONE_SOCKET_RIGHT); 
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

CCharacter * CChai::Clone(void* pArg)
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
