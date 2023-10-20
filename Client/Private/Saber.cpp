#include "..\Default\stdafx.h"
#include "Saber.h"

#include "GameInstance.h"
#include "Animation.h"

#include "Weapon.h"
#include "TriggerDummy.h"

#include "Blackboard_Saber.h"
#include "Node_Damaged_Saber.h"
#include "Node_Tracked_Saber.h"

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
		m_pModelCom->Set_Animation(ANIM_SA::IDLE_ATTACK, TRUE, DF_PL_TIME, DF_TW_TIME);
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
		ComponentNames[COM_SHADER], (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Saber"),
		ComponentNames[COM_MODEL], (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* Com_BehaviourTree*/
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_BehaviourTree"),
		ComponentNames[COM_BEHAVIOURTREE], (CComponent**)&m_pBehaviourTreeCom)))
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

HRESULT CSaber::Ready_BehavoiurTree()
{
	CBlackboard_Saber* pBlackboard	= nullptr;
	CNode* pRootNode			= nullptr;
	CNode* pSequenceNode		= nullptr;
	CNode* pSelectorNode		= nullptr;
	CNode* pNode				= nullptr;


	/* Blackboard */
	pBlackboard = CBlackboard_Saber::Create(this);
	{
		if (nullptr == pBlackboard)
			return E_FAIL;
	}

	/* 00.Root */
	pRootNode = CNode_Root::Create();
	{
		if (nullptr == pRootNode)
			return E_FAIL;

		if (FAILED(m_pBehaviourTreeCom->Set_RootNode(pRootNode)))
			return E_FAIL;
	}

	/* 01. Selector */
	pSelectorNode = CNode_Selector::Create();
	{
		if (nullptr == pSelectorNode)
			return E_FAIL;

		if (FAILED(pRootNode->Add_ChildNode(pSelectorNode)))
			return E_FAIL;
	}

	/* 01-00. Damaged */
	pNode = CNode_Damaged_Saber::Create(pBlackboard);
	{
		if (nullptr == pNode)
			return E_FAIL;

		if (FAILED(pSelectorNode->Add_ChildNode(pNode)))
			return E_FAIL;
	}

	/* 01-01. Sequence */
	pSequenceNode = CNode_Sequence::Create();
	{
		if (nullptr == pNode)
			return E_FAIL;

		if (FAILED(pSelectorNode->Add_ChildNode(pSequenceNode)))
			return E_FAIL;
	}

	/* 01-01-00 Tracked */
	pNode = CNode_Tracked_Saber::Create(pBlackboard);
	{
		if (nullptr == pNode)
			return E_FAIL;

		if (FAILED(pSequenceNode->Add_ChildNode(pNode)))
			return E_FAIL;
	}
	
	return S_OK;
}

HRESULT CSaber::Ready_Chilren()
{
	CWeapon* pWeapon = nullptr;

	pWeapon = dynamic_cast<CWeapon*>(GAME_INSTNACE->Add_GameObject(LV_PROTO, LayerNames[LAYER_WEAPON], L"Weapon_Saber_Sword"));
	{
		if (FAILED(Add_Child(pWeapon)))
			return E_FAIL;

		pWeapon->Set_Socket(CModel::BONE_SOCKET_RIGHT);

		CCollider::COLLIDERDESC		ColliderDesc{ Vec3(95, 0, 0), 30.f };

		pWeapon->Get_Collider_Sphere()->Set_ColliderDesc(ColliderDesc);
		pWeapon->Set_IndexAsChild(CHILD_TYPE::SA_WEAPON_RIGHT);
	}

	CTriggerDummy* pTrigger = nullptr;
	{
		CCollider::COLLIDERDESC		ColliderDesc{ Vec3(0, 0, 0), 5.f };
		CTriggerDummy::TRIGGER_DESC TriggerDesc(ColliderDesc, CCollider::SPHERE, CHILD_TYPE::SA_TRIGGER_TRACKED);
		pTrigger = dynamic_cast<CTriggerDummy*>(GAME_INSTNACE->Add_GameObject(LV_PROTO, LayerNames[LAYER_TRIGGER], L"Trigger_Dummy", &TriggerDesc));
		
		if (FAILED(Add_Child(pTrigger)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CSaber::Bind_ShaderResources()
{
	return S_OK;
}

void CSaber::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	CGameObject* pGameObject = pCollider->Get_Owner();
	const wstring& strLayer = pGameObject->Get_LayerTag();

	if (strLayer == LayerNames[LAYER_WEAPON])
	{
		if (LayerNames[LAYER_PLAYER] == pGameObject->Get_Parent()->Get_LayerTag())
		{
			m_tFightDesc.bDamaged = TRUE;
		}
	}
}

void CSaber::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
}

void CSaber::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
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
