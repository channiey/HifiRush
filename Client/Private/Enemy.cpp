#include "..\Default\stdafx.h"
#include "Enemy.h"

#include "GameInstance.h"
#include "Animation.h"

#include "Weapon.h"
#include "TriggerDummy.h"

CEnemy::CEnemy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCharacter(pDevice, pContext)
{
}

CEnemy::CEnemy(const CEnemy& rhs)
	: CCharacter(rhs)
{
}

HRESULT CEnemy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEnemy::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Chilren()))
		return E_FAIL;

	if (FAILED(Ready_BehavoiurTree()))
		return E_FAIL;

	return S_OK;
}

void CEnemy::Tick(_float fTimeDelta)
{
	if (FAILED(m_pBehaviourTreeCom->Tick(fTimeDelta)))
		return;

	__super::Tick(fTimeDelta);
}

void CEnemy::LateTick(_float fTimeDelta)
{
	if (FAILED(m_pModelCom->Update(fTimeDelta)))
		return;

	if (FAILED(m_pBehaviourTreeCom->LateTick(fTimeDelta)))
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CEnemy::Render()
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

void CEnemy::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);

	if (OBJ_STATE::STATE_ACTIVE == eState)
	{
		CGameObject* pTarget = GAME_INSTNACE->Get_GameObject_InCurLevel_InLayerFirst(LayerNames[LAYER_PLAYER]);

		if (nullptr == pTarget)
			return;

		m_tFightDesc.pTarget = dynamic_cast<CCharacter*>(pTarget);
	}
}

HRESULT CEnemy::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Shader_VTF"),
		ComponentNames[COM_SHADER], (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_BehaviourTree*/
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_BehaviourTree"),
		ComponentNames[COM_BEHAVIOURTREE], (CComponent**)&m_pBehaviourTreeCom)))
		return E_FAIL;

	return S_OK;
}


HRESULT CEnemy::Bind_ShaderResources()
{
	return S_OK;
}

void CEnemy::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	CGameObject*	pGameObject = pCollider->Get_Owner();

	if (LayerNames[LAYER_WEAPON] == pGameObject->Get_LayerTag())
	{
		if (LayerNames[LAYER_PLAYER] == pGameObject->Get_Parent()->Get_LayerTag())
		{
			m_tFightDesc.bDamaged = TRUE;
			m_tFightDesc.pAttacker = dynamic_cast<CCharacter*>(pGameObject->Get_Parent());
		}
	}
}

void CEnemy::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
}

void CEnemy::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
}

void CEnemy::Free()
{
	__super::Free();

	Safe_Release(m_pBehaviourTreeCom);
}
