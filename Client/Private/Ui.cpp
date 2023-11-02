#include "..\Default\stdafx.h"
#include "..\Public\Ui.h"

#include "EngineInstance.h"

CUi::CUi(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CUi::CUi(const CGameObject& rhs)
	: CGameObject(rhs)
{

}

HRESULT CUi::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUi::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* ���� ������ ���� ���� ���� (������ ��ǥ ���� ũ��, ����) */
	{
		m_eDesc.vSize.x = g_iWinSizeX * 0.5f;
		m_eDesc.vSize.y = g_iWinSizeY * 0.5f;

	
		m_eDesc.vPos.x = g_iWinSizeX * 0.5f;
		m_eDesc.vPos.y = g_iWinSizeY * 0.5f;

		m_pTransformCom->Set_Scale(Vec3(m_eDesc.vSize.x, m_eDesc.vSize.y, 1.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec4(m_eDesc.vPos.x - g_iWinSizeX * 0.5f, -m_eDesc.vPos.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	}
	
	/* UI�� ������ �� ��İ� ������ķ� �׸���. */
	{
		m_ViewMatrix = Matrix::Identity;
		m_ProjMatrix = Matrix::CreateOrthographic(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f);
	}

	return S_OK;
}

void CUi::Tick(_double fTimeDelta)
{

}

void CUi::LateTick(_double fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CUi::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUi::Ready_Components()
{
	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Texture_BackGround"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi::Bind_ShaderResources()
{
	/* ���̴� ���������� ������ �� ������ ������. */

	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShaderResources(m_pShaderCom, "g_Textures")))
		return E_FAIL;

	return S_OK;
}

void CUi::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
