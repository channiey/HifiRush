#include "..\Default\stdafx.h"
#include "..\Public\HealthBar.h"

#include "EngineInstance.h"
#include "BeatManager.h"

#include "ImGui_Manager.h"

CHealthBar::CHealthBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{

}

CHealthBar::CHealthBar(const CGameObject& rhs)
	: CGameObject(rhs)
{

}

HRESULT CHealthBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHealthBar::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* 직교 투영을 위한 변수 세팅 */

	/* 윈도우 좌표 (크기) */
	m_fSizeX = g_iWinSizeX * 0.5f;
	m_fSizeY = g_iWinSizeY * 0.5f;
	/* 윈도우 좌표 (중점) */
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec4(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	/* UI는 별도의 뷰 행렬과 투영행렬로 그린다. */

	m_ViewMatrix = Matrix::Identity;

	m_ProjMatrix = Matrix::CreateOrthographic(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f);

	return S_OK;
}

void CHealthBar::Tick(_double fTimeDelta)
{
	if (CImGui_Manager::GetInstance()->Is_DebugCam())
		return;

	/* 최초 1회 실행 */
	if (Vec3::Zero == m_vOriginPos)
		m_vOriginPos = m_pTransformCom->Get_Position().xyz();

	if (!m_LerpDesc.bActive)
	{
		if (!m_bMax)
			m_LerpDesc.Start(0.f, 30.f, CBeatManager::GetInstance()->Get_SPB() * 0.2f, LERP_MODE::EASE_OUT);
		else
			m_LerpDesc.Start(30.f, 0.f, CBeatManager::GetInstance()->Get_SPB() * 0.8f, LERP_MODE::EASE_OUT);

		m_bMax = !m_bMax;
	}
	m_LerpDesc.Update(fTimeDelta);
}

void CHealthBar::LateTick(_double fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CHealthBar::Render()
{

	/* 0 */
	m_pTransformCom->Set_Position(m_vOriginPos);

	if (FAILED(Bind_ShaderResources(0)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	/* 1 */

	Vec4 vPos{ m_vOriginPos.x + (_float)m_LerpDesc.fCurValue, m_vOriginPos.y, m_vOriginPos.z, 1.f };
	m_pTransformCom->Set_Position(vPos);
	if (FAILED(Bind_ShaderResources(1)))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CHealthBar::Ready_Components()
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
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Texture_HealthBarBody"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHealthBar::Bind_ShaderResources(const _uint iTexIndex)
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", iTexIndex)))
		return E_FAIL;

	return S_OK;
}

CHealthBar* CHealthBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHealthBar* pInstance = new CHealthBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHealthBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHealthBar::Clone(void* pArg)
{
	CHealthBar* pInstance = new CHealthBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CHealthBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHealthBar::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
