#include "..\Default\stdafx.h"
#include "..\Public\Ui_Gimmick_Peppermint_Target.h"

#include "EngineInstance.h"
#include "ImGui_Manager.h"
#include "UiManager.h"

CUi_Peppermint_Target::CUi_Peppermint_Target(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUi(pDevice, pContext)
{

}

CUi_Peppermint_Target::CUi_Peppermint_Target(const CGameObject& rhs)
	: CUi(rhs)
{

}

HRESULT CUi_Peppermint_Target::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUi_Peppermint_Target::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Load()))
		return E_FAIL;

	if (FAILED(CUiManager::GetInstance()->Add_UI(UI_ID::UI_PEPPERMINT_TARGET, this)))
		return E_FAIL;

	return S_OK;
}

void CUi_Peppermint_Target::Tick(_double fTimeDelta)
{
	if (nullptr == m_pTarget)
	{
		if (FAILED(Set_Target()))
			return;
	}

	__super::Tick(fTimeDelta);
}

void CUi_Peppermint_Target::LateTick(_double fTimeDelta)
{
	if (nullptr == m_pTarget)
	{
		if (FAILED(Set_Target()))
			return;
	}

	__super::LateTick(fTimeDelta);
}

HRESULT CUi_Peppermint_Target::Render()
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

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();
	}

	m_pTransformCom->Set_WorldMat(matWorldOrigin);

	return S_OK;
}

void CUi_Peppermint_Target::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);
}

HRESULT CUi_Peppermint_Target::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Vector Resize */
	m_pTextureComs.resize(TEX_TYPE::TYPEEND);
	m_TextureLocalDesc.resize(TEX_TYPE::TYPEEND);

	/* Load Texture */

	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_PartnerAction_Reticle_Arrow_3"),
		TEXT("Com_Texture_ARROW_TOP_1"), (CComponent**)&m_pTextureComs[TEX_TYPE::ARROW_TOP_1])))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_PartnerAction_Reticle_Arrow_3"),
		TEXT("Com_Texture_ARROW_TOP_2"), (CComponent**)&m_pTextureComs[TEX_TYPE::ARROW_TOP_2])))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_PartnerAction_Reticle_Arrow_4"),
		TEXT("Com_Texture_ARROW_RIGHT_1"), (CComponent**)&m_pTextureComs[TEX_TYPE::ARROW_RIGHT_1])))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_PartnerAction_Reticle_Arrow_4"),
		TEXT("Com_Texture_ARROW_RIGHT_2"), (CComponent**)&m_pTextureComs[TEX_TYPE::ARROW_RIGHT_2])))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_PartnerAction_Reticle_Arrow_1"),
		TEXT("Com_Texture_ARROW_DOWN_1"), (CComponent**)&m_pTextureComs[TEX_TYPE::ARROW_DOWN_1])))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_PartnerAction_Reticle_Arrow_1"),
		TEXT("Com_Texture_ARROW_DOWN_2"), (CComponent**)&m_pTextureComs[TEX_TYPE::ARROW_DOWN_2])))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_PartnerAction_Reticle_Arrow_2"),
		TEXT("Com_Texture_ARROW_LEFT_1"), (CComponent**)&m_pTextureComs[TEX_TYPE::ARROW_LEFT_1])))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_PartnerAction_Reticle_Arrow_2"),
		TEXT("Com_Texture_ARROW_LEFT_2"), (CComponent**)&m_pTextureComs[TEX_TYPE::ARROW_LEFT_2])))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_PartnerAction_Reticle_Frame_1"),
		TEXT("Com_Texture_CIRCLE"), (CComponent**)&m_pTextureComs[TEX_TYPE::CIRCLE])))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_PartnerAction_Reticle_TargetDot_1"),
		TEXT("Com_Texture_DOT"), (CComponent**)&m_pTextureComs[TEX_TYPE::DOT])))
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_Peppermint_Target::Bind_ShaderResources()
{
	m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float));

	return S_OK;
}

HRESULT CUi_Peppermint_Target::Set_Target()
{
	m_pTarget = ENGINE_INSTANCE->Get_GameObject_InCurLevel(LayerNames[LAYER_ID::LAYER_ENV_INTERACTALBE], L"Env_Dynamic_Switch");

	if (nullptr == m_pTarget)
		return E_FAIL;

	return S_OK;
}

CUi_Peppermint_Target* CUi_Peppermint_Target::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUi_Peppermint_Target* pInstance = new CUi_Peppermint_Target(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUi_Peppermint_Target");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CUi_Peppermint_Target* CUi_Peppermint_Target::Clone(void* pArg)
{
	CUi_Peppermint_Target* pInstance = new CUi_Peppermint_Target(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUi_Peppermint_Target");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Peppermint_Target::Free()
{
	__super::Free();
}
