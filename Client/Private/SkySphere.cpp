#include "..\Default\stdafx.h"
#include "..\Public\SkySphere.h"

#include "GameInstance.h"

CSkySphere::CSkySphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CSkySphere::CSkySphere(const CGameObject& rhs)
	: CGameObject(rhs)
{

}

HRESULT CSkySphere::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkySphere::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CSkySphere::PriorityTick(_float fTimeDelta)
{

}

void CSkySphere::Tick(_float fTimeDelta)
{

}

void CSkySphere::LateTick(_float fTimeDelta)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, GAME_INSTNACE->Get_CamPosition());

	m_pRendererCom->Add_RenderGroup(CRenderer::RG_PRIORITY, this);
}

HRESULT CSkySphere::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pShaderCom, 0)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkySphere::Ready_Components()
{
	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Renderer"),
		ComponentNames[COM_RENDERER], (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LV_STATIC, ShaderNames[SHADER_SKY_SPHERE],
		ComponentNames[COM_SHADER], (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Texture_SkySphere"),
		ComponentNames[COM_TEXTURE], (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"),
		ComponentNames[COM_TRANSFORM], (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Static_Geometry_Sphere"),
		ComponentNames[COM_MODEL], (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkySphere::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(GAME_INSTNACE->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::STATE_VIEW)))
		return E_FAIL;

	if (FAILED(GAME_INSTNACE->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::STATE_PROJ)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	return S_OK;
}

CSkySphere* CSkySphere::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkySphere* pInstance = new CSkySphere(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSkySphere");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkySphere::Clone(void* pArg)
{
	CSkySphere* pInstance = new CSkySphere(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSkySphere");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkySphere::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
