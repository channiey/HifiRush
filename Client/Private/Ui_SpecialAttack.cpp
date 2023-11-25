#include "..\Default\stdafx.h"
#include "..\Public\Ui_SpecialAttack.h"

#include "EngineInstance.h"
#include "ImGui_Manager.h"
#include "UiManager.h"

CUi_SpecialAttack::CUi_SpecialAttack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUi(pDevice, pContext)
{

}

CUi_SpecialAttack::CUi_SpecialAttack(const CGameObject& rhs)
	: CUi(rhs)
{

}

HRESULT CUi_SpecialAttack::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUi_SpecialAttack::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Load()))
		return E_FAIL;

	if (FAILED(CUiManager::GetInstance()->Add_UI(UI_ID::UI_SPECIALATTACK, this)))
		return E_FAIL;

	return S_OK;
}

void CUi_SpecialAttack::Tick(_double fTimeDelta)
{
	if (m_tLerpDesc_Alpha.bActive)
	{
		m_tLerpDesc_Alpha.Update(fTimeDelta);

		if (0.f == m_tLerpDesc_Alpha.fCurValue)
			Set_State(OBJ_STATE::STATE_UNACTIVE);
	}

	__super::Tick(fTimeDelta);
}

void CUi_SpecialAttack::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUi_SpecialAttack::Render()
{
	if (m_TextureLocalDesc.empty()) return E_FAIL;

	if (FAILED(__super::Render())) return E_FAIL;

	if (FAILED(Bind_ShaderResources())) return E_FAIL;

	/* Render Texture */
	Matrix matWorldOrigin = m_pTransformCom->Get_WorldMat();

	for (size_t i = 0; i < m_pTextureComs.size(); i++)
	{
		if (nullptr == m_pTextureComs[i]) continue;

		if (TEX_TYPE::WORD_HIBIKI == i && TEX_TYPE::WORD_HIBIKI != m_eSpecialAttackType)
			continue;
		else if (TEX_TYPE::WORD_POWERCHORD == i && TEX_TYPE::WORD_POWERCHORD != m_eSpecialAttackType)
			continue;

		const Matrix matNewWorld = Matrix::CreateScale(m_TextureLocalDesc[i].vSize.x, m_TextureLocalDesc[i].vSize.y, 1.f)
			* Matrix::CreateTranslation(m_TextureLocalDesc[i].vPos.x, m_TextureLocalDesc[i].vPos.y, 0.f)
			* matWorldOrigin;

		m_pTransformCom->Set_WorldMat(matNewWorld);

		if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
			return E_FAIL;

		if (FAILED(m_pTextureComs[i]->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();
	}

	m_pTransformCom->Set_WorldMat(matWorldOrigin);

	return S_OK;
}

void CUi_SpecialAttack::Set_On(const _bool& bHibiki)
{
	if (bHibiki)
		m_eSpecialAttackType = TEX_TYPE::WORD_HIBIKI;
	else
		m_eSpecialAttackType = TEX_TYPE::WORD_POWERCHORD;

	m_tLerpDesc_Alpha.Start(0.f, 1.f, 0.1f, LERP_MODE::EASE_IN);

	Set_State(OBJ_STATE::STATE_ACTIVE);
}

void CUi_SpecialAttack::Set_Off()
{
	m_tLerpDesc_Alpha.Start(1.f, 0.f, 0.1f, LERP_MODE::EASE_IN);
}

HRESULT CUi_SpecialAttack::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Vector Resize */
	m_pTextureComs.resize(TEX_TYPE::TYPEEND);
	m_TextureLocalDesc.resize(TEX_TYPE::TYPEEND);

	/* Load Texture */
	{
		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_special_combo_BG"),
			TEXT("Com_Texture_BACKLOGO"), (CComponent**)&m_pTextureComs[TEX_TYPE::BACKLOGO])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_SpAttackCutIn_Chai"),
			TEXT("Com_Texture_CHAI"), (CComponent**)&m_pTextureComs[TEX_TYPE::CHAI])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_SP_Word_SpAttackCutIn_Hibiki"),
			TEXT("Com_Texture_WORD_HIBIKI"), (CComponent**)&m_pTextureComs[TEX_TYPE::WORD_HIBIKI])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_SP_Word_SpAttackCutIn_PowerChord"),
			TEXT("Com_Texture_WORD_POWERCHORD"), (CComponent**)&m_pTextureComs[TEX_TYPE::WORD_POWERCHORD])))
			return E_FAIL;	
	}

	return S_OK;
}

HRESULT CUi_SpecialAttack::Bind_ShaderResources()
{
	const _float fAlpha = (_float)m_tLerpDesc_Alpha.fCurValue;

	m_pShaderCom->Bind_RawValue("g_Alpha", &fAlpha, sizeof(_float));

	return S_OK;
}

CUi_SpecialAttack* CUi_SpecialAttack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUi_SpecialAttack* pInstance = new CUi_SpecialAttack(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUi_SpecialAttack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CUi_SpecialAttack* CUi_SpecialAttack::Clone(void* pArg)
{
	CUi_SpecialAttack* pInstance = new CUi_SpecialAttack(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUi_SpecialAttack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_SpecialAttack::Free()
{
	__super::Free();
}
