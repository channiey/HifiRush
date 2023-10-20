#include "..\Default\stdafx.h"
#include "Saber.h"

#include "GameInstance.h"
#include "Animation.h"

#include "Weapon.h"

#include "Blackboard_Saver.h"

CSaber::CSaber(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCharacter(pDevice, pContext)
{
}

CSaber::CSaber(const CSaber& rhs)
	: CCharacter(rhs)
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

	if (FAILED(Ready_BehavoiurTree()))
		return E_FAIL;

	// << : 임시 코드 
	{
		Vec3 vPos{ 2.5f, 0.f, -1.5f };
		m_pTransformCom->Set_Position(vPos);
		m_pTransformCom->Rotate(m_pTransformCom->Get_Up(), DEG2RAD(180.f));
	}
	// >> 

	return S_OK;
}

void CSaber::Tick(_float fTimeDelta)
{
	if (FAILED(m_pBehaviourTreeCom->Tick(fTimeDelta)))
		return;

	__super::Tick(fTimeDelta);
}

void CSaber::LateTick(_float fTimeDelta)
{
	if (FAILED(m_pModelCom->Update(fTimeDelta)))
		return;

	if (FAILED(m_pBehaviourTreeCom->LateTick(fTimeDelta)))
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CSaber::Render()
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

HRESULT CSaber::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Shader_VTF"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Saber"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Comp_BehaviourTree*/
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_BehaviourTree"),
		TEXT("Com_BehaviourTree"), (CComponent**)&m_pBehaviourTreeCom)))
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

	return S_OK;
}

HRESULT CSaber::Ready_BehavoiurTree()
{
	/* Blackboard */
	CBlackboard* pBlackboard = CBlackboard_Saver::Create(this);
	{
		if (nullptr == pBlackboard)
			return E_FAIL;
	}

	/* 00.Root */
	CNode* pRootNode = CNode_Root::Create(pBlackboard);
	{
		if (nullptr == pRootNode)
			return E_FAIL;

		if (FAILED(m_pBehaviourTreeCom->Set_RootNode(pRootNode)))
			return E_FAIL;
	}

	/* 01. Sequence */
	CNode* pSequenceNode = CNode_Sequence::Create(pBlackboard);
	{
		if (nullptr == pSequenceNode)
			return E_FAIL;

		if (FAILED(pRootNode->Add_ChildNode(pSequenceNode)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CSaber::Ready_Chilren()
{
	CWeapon* pChild = nullptr;

	pChild = dynamic_cast<CWeapon*>(GAME_INSTNACE->Add_GameObject(LV_PROTO, g_strLayerID[LAYER_WEAPON], L"Weapon_Saber_Sword"));
	{
		if (FAILED(Add_Child(pChild)))
			return E_FAIL;

		pChild->Set_Socket(CModel::BONE_SOCKET_RIGHT);

		CCollider::COLLIDERDESC		ColliderDesc{ Vec3(95, 0, 0), 30.f };

		pChild->Get_Collider_Sphere()->Set_ColliderDesc(ColliderDesc);

	}
	return S_OK;
}

HRESULT CSaber::Bind_ShaderResources()
{
	return S_OK;
}

void CSaber::OnCollision_Enter(CGameObject* pGameObject)
{
	const wstring& strLayer = pGameObject->Get_LayerTag();

	if (strLayer == g_strLayerID[LAYER_WEAPON])
	{
		if (g_strLayerID[LAYER_PLAYER] == pGameObject->Get_Parent()->Get_LayerTag())
		{
			/* 플레이어 무기에 맞음 */
		}
	}
}

void CSaber::OnCollision_Stay(CGameObject* pGameObject)
{
}

void CSaber::OnCollision_Exit(CGameObject* pGameObject)
{
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

	Safe_Release(m_pBehaviourTreeCom);
}
