#include "..\Default\stdafx.h"
#include "..\Public\Player.h"

#include "GameInstance.h"

CPlayer::CPlayer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CPlayer::CPlayer(const CGameObject & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* Temporary Initaial Setting */
	{
		//m_pModelCom->Set_Animation(rand() % 6);
	}

	return S_OK;
}

void CPlayer::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	/* Test Animation */
	CGameInstance* pGameInst = GET_INSTANCE(CGameInstance);
	{
		/*if (pGameInst->Key_Down(VK_UP))
		{
			_int  iCurAnim = (m_pModelCom->Get_CurAnimationIndex() + 1) % m_pModelCom->Get_AnimationCount();

			m_pModelCom->Set_Animation(iCurAnim);
		}
		else if (pGameInst->Key_Down(VK_DOWN))
		{
			_int iCurAnim = m_pModelCom->Get_CurAnimationIndex() - 1;
			if (iCurAnim < 0) 
				iCurAnim = 0;

			m_pModelCom->Set_Animation(iCurAnim);
		}*/

		//m_pModelCom->Set_Animation(1);

	}
	RELEASE_INSTANCE(CGameInstance);

	/* Update Colliders */
	for (auto& pCollider : m_pColliderComs)
	{
		if (nullptr != pCollider)
			pCollider->Update(m_pTransformCom->Get_WorldMat());
	}
}

void CPlayer::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	//m_pModelCom->Update_Anim(fTimeDelta);
	m_pModelCom->Update_VTFAnim(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);	

	for (auto& pCollider : m_pColliderComs)
	{
		if (nullptr != pCollider)
			m_pRendererCom->Add_DebugGroup(pCollider);
	}
}

HRESULT CPlayer::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Ready_Components()
{
	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Shader */
	//if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
	//	TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
	//	return E_FAIL;
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Shader_VTF"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Chai"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
	
	/* Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* Com_Collider_Sphere */
	CCollider::COLLIDERDESC		ColliderDesc;
	{
		ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));

		ColliderDesc.vSize		= _float3(1.f, 1.f, 1.f);
		ColliderDesc.vCenter	= _float3(0.f, ColliderDesc.vSize.y * 0.5f, 0.f);
		ColliderDesc.vRotation	= _float3(0.f, XMConvertToRadians(45.f), 0.f);
	}
	CCollider_Sphere* pCollider = nullptr;
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), (CComponent**)&pCollider, &ColliderDesc)))
		return E_FAIL;

	m_pColliderComs.push_back(pCollider);

	return S_OK;
}

HRESULT CPlayer::Bind_ShaderResources()
{
	/* WVP 행렬을 셰이더에 던진다. */
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	Safe_AddRef(pGameInstance);
	{
		if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
			return E_FAIL;

		if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::STATE_VIEW)))
			return E_FAIL;

		if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::STATE_PROJ)))
			return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);

	
	/* 메시가 사용하는 매태리얼과 본의 정보를 셰이더에 던진다. */
	for (_uint i = 0; i < m_pModelCom->Get_NumMeshes(); ++i)
	{
		/* 셰이더에 현재 메시의 매테리얼 정보를 바인딩 한다. */
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		//if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
		//	return E_FAIL;

		/* 현재 메시가 사용하는 모든 본의 최종 트랜스폼 계산 + 셰이더 바인딩 */
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
			return E_FAIL;
	}

	return S_OK;
}

CPlayer * CPlayer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPlayer*	pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayer::Clone(void* pArg)
{
	CPlayer*	pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	for (auto& pCollider : m_pColliderComs)
		Safe_Release(pCollider);

	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);	
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
