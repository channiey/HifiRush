#include "..\Default\stdafx.h"
#include "..\Public\Ui_Gimmick_Peppermint_Aim.h"

#include "EngineInstance.h"
#include "ImGui_Manager.h"
#include "UiManager.h"

CUi_Peppermint_Aim::CUi_Peppermint_Aim(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUi(pDevice, pContext)
{

}

CUi_Peppermint_Aim::CUi_Peppermint_Aim(const CGameObject& rhs)
	: CUi(rhs)
{

}

HRESULT CUi_Peppermint_Aim::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUi_Peppermint_Aim::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Load()))
		return E_FAIL;

	if (FAILED(CUiManager::GetInstance()->Add_UI(UI_ID::UI_PEPPERMINT_AIM, this)))
		return E_FAIL;

	return S_OK;
}

void CUi_Peppermint_Aim::Tick(_double fTimeDelta)
{
	
}

void CUi_Peppermint_Aim::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUi_Peppermint_Aim::Render()
{
	if (m_TextureLocalDesc.empty()) return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	Matrix matWorldOrigin = m_pTransformCom->Get_WorldMat();

	const size_t iSize = (size_t)m_pTextureComs.size();
	for (size_t i = 0; i < iSize; i++)
	{
		if (nullptr == m_pTextureComs[i]) continue;

		/* 텍스처의 로컬 정보를 Ui 월드로 올린다. */
		{
			const Matrix matNewWorld = Matrix::CreateScale(m_TextureLocalDesc[i].vSize.x, m_TextureLocalDesc[i].vSize.y, 1.f)
				* Matrix::CreateTranslation(m_TextureLocalDesc[i].vPos.x, m_TextureLocalDesc[i].vPos.y, 0.f)
				* matWorldOrigin;

			m_pTransformCom->Set_WorldMat(matNewWorld);

			if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
				return E_FAIL;
		}

		if (FAILED(m_pTextureComs[i]->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
			return E_FAIL;

		if (TEX_TYPE::DOT_OUT == i)
		{
			m_fAlpha = 0.5f;
			m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float));
		}
		m_pShaderCom->Begin(1);
		if (TEX_TYPE::DOT_OUT == i)
		{
			m_fAlpha = 1.f;
			m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float));
		}

		m_pVIBufferCom->Render();
	}

	m_pTransformCom->Set_WorldMat(matWorldOrigin);

	return S_OK;
}

void CUi_Peppermint_Aim::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);
}

HRESULT CUi_Peppermint_Aim::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Vector Resize */
	m_pTextureComs.resize(TEX_TYPE::TYPEEND);
	m_TextureLocalDesc.resize(TEX_TYPE::TYPEEND);
	
	/* Load Texture */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_PartnerAction_PlayerReticle_Frame_1"),
		TEXT("Com_Texture_SIDE_LEFT"), (CComponent**)&m_pTextureComs[TEX_TYPE::SIDE_LEFT])))
		return E_FAIL;
	
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_PartnerAction_PlayerReticle_Frame_2"),
		TEXT("Com_Texture_SIDE_RIGHT"), (CComponent**)&m_pTextureComs[TEX_TYPE::SIDE_RIGHT])))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_PartnerAction_PlayerReticle_Arrow_1"),
		TEXT("Com_Texture_ARROW_TOP"), (CComponent**)&m_pTextureComs[TEX_TYPE::ARROW_TOP])))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_PartnerAction_PlayerReticle_Arrow_2"),
		TEXT("Com_Texture_ARROW_RIGHT"), (CComponent**)&m_pTextureComs[TEX_TYPE::ARROW_RIGHT])))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_PartnerAction_PlayerReticle_Arrow_3"),
		TEXT("Com_Texture_ARROW_DOWN"), (CComponent**)&m_pTextureComs[TEX_TYPE::ARROW_DOWN])))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_PartnerAction_PlayerReticle_Arrow_4"),
		TEXT("Com_Texture_ARROW_LEFT"), (CComponent**)&m_pTextureComs[TEX_TYPE::ARROW_LEFT])))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_PartnerAction_PlayerReticle_Ring"),
		TEXT("Com_Texture_DOT_OUT"), (CComponent**)&m_pTextureComs[TEX_TYPE::DOT_OUT])))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_PartnerAction_Reticle_TargetDot_1"),
		TEXT("Com_Texture_DOT_IN"), (CComponent**)&m_pTextureComs[TEX_TYPE::DOT_IN])))
		return E_FAIL;


	return S_OK;
}

HRESULT CUi_Peppermint_Aim::Bind_ShaderResources()
{
	m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float));

	return S_OK;
}

CUi_Peppermint_Aim* CUi_Peppermint_Aim::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUi_Peppermint_Aim* pInstance = new CUi_Peppermint_Aim(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUi_Peppermint_Aim");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CUi_Peppermint_Aim* CUi_Peppermint_Aim::Clone(void* pArg)
{
	CUi_Peppermint_Aim* pInstance = new CUi_Peppermint_Aim(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUi_Peppermint_Aim");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Peppermint_Aim::Free()
{
	__super::Free();
}
