#include "..\Default\stdafx.h"
#include "..\Public\Fiona.h"

#include "GameInstance.h"

CFiona::CFiona(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{

}

CFiona::CFiona(const CGameObject & rhs)
	: CGameObject(rhs)
{

}

HRESULT CFiona::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFiona::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom->Set_AnimIndex(rand() % 15);


	return S_OK;
}

void CFiona::Tick(_float fTimeDelta)
{

}

void CFiona::LateTick(_float fTimeDelta)
{
	/* 해당 애니메이션이 사용하는 뼈의 정보를 갱신한다. */
	m_pModelCom->Update_Anim(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this);	
}

HRESULT CFiona::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFiona::Ready_Components()
{
	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_AnimModel"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
		TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
	
	/* Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFiona::Bind_ShaderResources()
{
	/* WVP 상태 행렬을 셰이더에 던진다. */
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	Safe_AddRef(pGameInstance);
	{
		if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::D3DTS_VIEW)))
			return E_FAIL;

		if (FAILED(pGameInstance->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::D3DTS_PROJ)))
			return E_FAIL;
	}
	RELEASE_INSTANCE(CGameInstance);

	/* 현재 모델의 렌더링을 위한 버퍼 정보를 전달한다. */
	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		/* 셰이더에 현재 메시의 매테리얼 정보를 바인딩 한다. */
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		//if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
		//	return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
			return E_FAIL;
	}


	return S_OK;
}

CFiona * CFiona::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CFiona*	pInstance = new CFiona(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFiona");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CFiona::Clone(void* pArg)
{
	CFiona*	pInstance = new CFiona(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFiona");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFiona::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pTransformCom);	
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
