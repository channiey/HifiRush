#include "..\Default\stdafx.h"
#include "..\Public\Chai.h"

#include "GameInstance.h"
#include "Weapon.h"

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

	if (FAILED(Ready_ChildObjects()))
		return E_FAIL;


	m_pModelCom->Set_Animation(5, TRUE);

	return S_OK;
}

void CChai::Tick(_float fTimeDelta)
{
	// << : Test 
	{
		if (GAME_INSTNACE->Key_Pressing('W'))
		{
			m_pTransformCom->Translate(m_pTransformCom->Get_Forward() * 10 * fTimeDelta);
		}
		else if (GAME_INSTNACE->Key_Pressing('S'))
		{
			m_pTransformCom->Translate(m_pTransformCom->Get_Backward() * 10 * fTimeDelta);
		}
		else if (GAME_INSTNACE->Key_Pressing('A'))
		{
			m_pTransformCom->Translate(m_pTransformCom->Get_Left() * 10 * fTimeDelta);
		}
		else if (GAME_INSTNACE->Key_Pressing('D'))
		{
			m_pTransformCom->Translate(m_pTransformCom->Get_Right() * 10 * fTimeDelta);
		}

		if (GAME_INSTNACE->Key_Down('Q'))
			m_pModelCom->Set_Animation(0, FALSE);

		if (GAME_INSTNACE->Key_Down('E'))
			m_pModelCom->Set_Animation(1, FALSE);



	
	}
	// >> : 

	/*if (FAILED(m_pStateMachineCom->Tick(fTimeDelta)))
		return;*/

	__super::Tick(fTimeDelta);
}

void CChai::LateTick(_float fTimeDelta)
{
	/*if (FAILED(m_pStateMachineCom->LateTick(fTimeDelta)))
		return;*/

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
	CCollider::COLLIDERDESC		ColliderDesc{1.f};
	CCollider_Sphere*			pCollider = nullptr;
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), (CComponent**)&pCollider, &ColliderDesc)))
		return E_FAIL;

	m_pColliderComs.push_back(pCollider);

	/* Com_StateMachine */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_StateMachine"),
		TEXT("Com_StateMachine"), (CComponent**)&m_pStateMachineCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CChai::Ready_ChildObjects()
{
	CWeapon* pChild = nullptr;
	
	pChild = dynamic_cast<CWeapon*>(GAME_INSTNACE->Add_GameObject(LV_STATIC, g_StrLayerID[LAYER_WEAPON], L"Weapon_Chai_Guitar_Explore"));
	{
		if (nullptr == pChild)
			return E_FAIL;

		Add_Child(pChild);
		pChild->Set_Socket(CModel::BONE_SOCKET_RIGHT);

	}

	return S_OK;
}

HRESULT CChai::Bind_ShaderResources()
{
	return S_OK;
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
