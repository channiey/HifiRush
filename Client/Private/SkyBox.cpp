#include "..\Default\stdafx.h"
#include "..\Public\SkyBox.h"

#include "EngineInstance.h"

CSkyBox::CSkyBox(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{

}

CSkyBox::CSkyBox(const CGameObject & rhs)
	: CGameObject(rhs)
{

}

HRESULT CSkyBox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkyBox::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CSkyBox::PriorityTick(_float fTimeDelta)
{	
	
}

void CSkyBox::Tick(_float fTimeDelta)
{

}

void CSkyBox::LateTick(_float fTimeDelta)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, ENGINE_INSTANCE->Get_CamPosition());

	m_pRendererCom->Add_RenderGroup(CRenderer::RG_PRIORITY, this);	
}

HRESULT CSkyBox::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CSkyBox::Ready_Components()
{
	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Renderer"),
		ComponentNames[COM_RENDERER], (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LV_STATIC, ShaderNames[SHADER_CUBE],
		ComponentNames[COM_SHADER], (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		ComponentNames[COM_VIBUFFER], (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Texture_Sky"),
		ComponentNames[COM_TEXTURE], (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"),
		ComponentNames[COM_TRANSFORM], (CComponent**)&m_pTransformCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CSkyBox::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::STATE_VIEW)))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::STATE_PROJ)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 3)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	return S_OK;
}

CSkyBox * CSkyBox::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CSkyBox*	pInstance = new CSkyBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSkyBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CSkyBox::Clone(void* pArg)
{
	CSkyBox*	pInstance = new CSkyBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSkyBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkyBox::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);	
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
