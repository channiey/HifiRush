#include "..\Default\stdafx.h"
#include "..\Public\Fight_Logo.h"

#include "EngineInstance.h"
#include "ImGui_Manager.h"
#include "UiManager.h"

CFight_Logo::CFight_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUi(pDevice, pContext)
{

}

CFight_Logo::CFight_Logo(const CGameObject& rhs)
	: CUi(rhs)
{

}

HRESULT CFight_Logo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFight_Logo::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Load()))
		return E_FAIL;

	if (FAILED(CUiManager::GetInstance()->Add_UI(UI_ID::UI_FIGHT, this)))
		return E_FAIL;

	return S_OK;
}

void CFight_Logo::Tick(_double fTimeDelta)
{	
	__super::Tick(fTimeDelta);

	/* 처음 보간 */
	if (!m_bFix && m_tLerpDescHeight.bActive)
	{
		m_tLerpDescHeight.Update(fTimeDelta);
		m_TextureLocalDesc[TEX_TYPE::LOGO].vPos.y = m_tLerpDescHeight.fCurValue;
	}

	/* 나중 보간*/
	if (m_bFix && m_tLerpDescHeight.bActive)
	{
		m_tLerpDescHeight.Update(fTimeDelta);
		m_fAlpha = m_tLerpDescHeight.fCurValue;
	}

	/* 나중 보간 끝 */
	if (m_bFix && !m_tLerpDescHeight.bActive)
	{
		Set_State(CGameObject::OBJ_STATE::STATE_UNACTIVE);
	}

	/* 처음 보간 끝 */
	if (!m_bFix && !m_tLerpDescHeight.bActive)
	{
		m_fAcc += fTimeDelta;

		if (!m_bSound && m_fLimit * 0.5f <= m_fAcc)
		{
			ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_ETC_FIGHT, CHANNEL_ID::ETC, 0.7f);
			m_bSound = TRUE;
		}
		if (m_fLimit <= m_fAcc)
		{
			m_bFix = TRUE;
			m_tLerpDescHeight.Start(m_fAlpha, 0.f, 0.5f, LERP_MODE::SMOOTHER_STEP);
		}
	}
}

void CFight_Logo::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CFight_Logo::Render()
{
	if (m_TextureLocalDesc.empty()) return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float));


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

		m_pShaderCom->Begin(2);

		m_pVIBufferCom->Render();
	}

	m_pTransformCom->Set_WorldMat(matWorldOrigin);

	return S_OK;
}

void CFight_Logo::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);

	if (OBJ_STATE::STATE_ACTIVE == eState)
	{
		m_tLerpDescHeight.Start(-300.f, 0.f, 0.1f, LERP_MODE::EASE_IN);

		m_fAcc = 0.f;
	}
	else if (OBJ_STATE::STATE_UNACTIVE == eState)
	{
		m_fLimit = 1.5f;
		m_fAcc = 0.f;
		m_fAlpha = 0.6f;
		m_bFix = FALSE;
		m_bSound = FALSE;
	}
}

HRESULT CFight_Logo::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Vector Resize */
	m_pTextureComs.resize(TEX_TYPE::TYPEEND);
	m_TextureLocalDesc.resize(TEX_TYPE::TYPEEND);

	/* Load Texture */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_VFX_tk_word_FIGHT"),
		TEXT("Com_Texture_LOGO"), (CComponent**)&m_pTextureComs[TEX_TYPE::LOGO])))
		return E_FAIL;

	return S_OK;
}

HRESULT CFight_Logo::Bind_ShaderResources()
{
	return S_OK;
}

CFight_Logo* CFight_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFight_Logo* pInstance = new CFight_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFight_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CFight_Logo* CFight_Logo::Clone(void* pArg)
{
	CFight_Logo* pInstance = new CFight_Logo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFight_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFight_Logo::Free()
{
	__super::Free();
}
