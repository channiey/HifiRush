#include "..\Default\stdafx.h"
#include "..\Public\Hud.h"

#include "EngineInstance.h"
#include "ImGui_Manager.h"
#include "BeatManager.h"
#include "Character.h"
#include "UiManager.h"
CHud::CHud(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUi(pDevice, pContext)
{

}

CHud::CHud(const CGameObject& rhs)
	: CUi(rhs)
{

}

HRESULT CHud::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHud::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Load()))
		return E_FAIL;

	m_TextureLocalDescOrigin.resize(m_TextureLocalDesc.size());
	for (size_t i = 0; i < m_TextureLocalDesc.size(); i++)
		m_TextureLocalDescOrigin[i] = m_TextureLocalDesc[i];

	if (FAILED(CUiManager::GetInstance()->Add_UI(UI_ID::UI_HUD, this)))
		return E_FAIL;

	return S_OK;
}

void CHud::Tick(_double fTimeDelta)
{
	if (nullptr == m_pPlayer)
		Set_Player();

	__super::Tick(fTimeDelta);

	Drop(fTimeDelta);
}

void CHud::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CHud::Render()
{
	if (m_TextureLocalDesc.empty()) return E_FAIL;

	if (FAILED(__super::Render())) return E_FAIL;

	if (FAILED(Bind_ShaderResources())) return E_FAIL;

	Matrix matWorldOrigin = m_pTransformCom->Get_WorldMat();

	for (size_t i = 0; i < m_pTextureComs.size(); i++)
	{
		if (nullptr == m_pTextureComs[i]) continue;

		const Matrix matNewWorld = Matrix::CreateScale(m_TextureLocalDesc[i].vSize.x, m_TextureLocalDesc[i].vSize.y, 1.f)
									* Matrix::CreateTranslation(m_TextureLocalDesc[i].vPos.x, m_TextureLocalDesc[i].vPos.y, 0.f)
									* matWorldOrigin;

		m_pTransformCom->Set_WorldMat(matNewWorld);

		if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
			return E_FAIL;

		if (FAILED(m_pTextureComs[i]->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
			return E_FAIL;

		if (LOGO_NORMAL <= i && LOGO_SPECIAL >= i)
		{
			if (LOGO_SPECIAL == i)
				continue;

			m_pShaderCom->Begin(0);
		}
		else if (HEALTH_EMPTY <= i && HEALTH_DAMAGED >= i)
		{
			if (HEALTH_DAMAGED == i)
				continue;

			m_pShaderCom->Begin(1);
		}
		else if (SPECIAL_EMPTY_0 <= i && SPECIAL_FULL_2 >= i)
		{
			if (HEALTH_FULL == i)
				m_pShaderCom->Begin(2);
			else
				m_pShaderCom->Begin(0);
		}
		else if (PARTNER_MACARON_ON <= i && PARTNER_PEPPERMINT_OFF >= i)
		{
			m_pShaderCom->Begin(0);
		}
		else
		{
			m_pShaderCom->Begin(0);
		}

		m_pVIBufferCom->Render();
	}
	
	m_pTransformCom->Set_WorldMat(matWorldOrigin);

	return S_OK;
}

HRESULT CHud::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LV_STATIC, ShaderNames[SHADER_ID::SHADER_UI_HUD],
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))

		return E_FAIL;
	/* Vector Resize */
	m_pTextureComs.resize(TEX_TYPE::TYPEEND);
	m_TextureLocalDesc.resize(TEX_TYPE::TYPEEND);

	/* Load Texture */
	{
		/* Drop */
		{
			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_HUD_GuiterBody_DropShadow"),
				TEXT("Com_Texture_DROP_LOGO"), (CComponent**)&m_pTextureComs[TEX_TYPE::DROP_LOGO])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_HealthBar_DropShadow_10"),
				TEXT("Com_Texture_DROP_HEALTH"), (CComponent**)&m_pTextureComs[TEX_TYPE::DROP_HEALTH])))
				return E_FAIL;
		}

		/* Logo */
		{
			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_HUD_PlayerGauge_GuiterBack"),
				TEXT("Com_Texture_LOGO"), (CComponent**)&m_pTextureComs[TEX_TYPE::LOGO_NORMAL])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_HUD_LifeGauge_GuiterBack_Color"),
				TEXT("Com_Texture_LOGO_SPECIAL"), (CComponent**)&m_pTextureComs[TEX_TYPE::LOGO_SPECIAL])))
				return E_FAIL;
		}

		/* Health */
		{
			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_HealthBar_Bg_10"),
				TEXT("Com_Texture_HEALTH_EMPTY"), (CComponent**)&m_pTextureComs[TEX_TYPE::HEALTH_EMPTY])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_HealthBar_Bar_10"),
				TEXT("Com_Texture_HEALTH_FULL"), (CComponent**)&m_pTextureComs[TEX_TYPE::HEALTH_FULL])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_HealthBar_DamageEffect_10"),
				TEXT("Com_Texture_HEALTH_DAMAGED"), (CComponent**)&m_pTextureComs[TEX_TYPE::HEALTH_DAMAGED])))
				return E_FAIL;
		}

		/* Special */
		{
			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_HUD_PlayerGauge_EmptyReverb_A_01"),
				TEXT("Com_Texture_SPECIAL_EMPTY_0"), (CComponent**)&m_pTextureComs[TEX_TYPE::SPECIAL_EMPTY_0])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_HUD_PlayerGauge_EmptyReverb_A_02"),
				TEXT("Com_Texture_SPECIAL_EMPTY_1"), (CComponent**)&m_pTextureComs[TEX_TYPE::SPECIAL_EMPTY_1])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_HUD_PlayerGauge_EmptyReverb_A_03"),
				TEXT("Com_Texture_SPECIAL_EMPTY_2"), (CComponent**)&m_pTextureComs[TEX_TYPE::SPECIAL_EMPTY_2])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_HUD_PlayerGauge_ReverbBar_A_01"),
				TEXT("Com_Texture_SPECIAL_FULL_0"), (CComponent**)&m_pTextureComs[TEX_TYPE::SPECIAL_FULL_0])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_HUD_PlayerGauge_ReverbBar_A_02"),
				TEXT("Com_Texture_SPECIAL_FULL_1"), (CComponent**)&m_pTextureComs[TEX_TYPE::SPECIAL_FULL_1])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_HUD_PlayerGauge_ReverbBar_A_03"),
				TEXT("Com_Texture_SPECIAL_FULL_2"), (CComponent**)&m_pTextureComs[TEX_TYPE::SPECIAL_FULL_2])))
				return E_FAIL;

		}

		/* Partner */
		{
			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_gauge_partner_new_Macaron_01"),
				TEXT("Com_Texture_PARTNER_MACARON_ON"), (CComponent**)&m_pTextureComs[TEX_TYPE::PARTNER_MACARON_ON])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_gauge_partner_new_Macaron_01_BW"),
				TEXT("Com_Texture_PARTNER_MACARON_OFF"), (CComponent**)&m_pTextureComs[TEX_TYPE::PARTNER_MACARON_OFF])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_gauge_partner_new_Korsica_01"),
				TEXT("Com_Texture_PARTNER_KORSICA_ON"), (CComponent**)&m_pTextureComs[TEX_TYPE::PARTNER_KORSICA_ON])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_gauge_partner_new_Korsica_01_BW"),
				TEXT("Com_Texture_PARTNER_KORSICA_OFF"), (CComponent**)&m_pTextureComs[TEX_TYPE::PARTNER_KORSICA_OFF])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_gauge_partner_new_Peppermint_01"),
				TEXT("Com_Texture_PARTNER_PEPPERMINT_ON"), (CComponent**)&m_pTextureComs[TEX_TYPE::PARTNER_PEPPERMINT_ON])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_gauge_partner_new_Peppermint_01_BW"),
				TEXT("Com_Texture_PARTNER_PEPPERMINT_OFF"), (CComponent**)&m_pTextureComs[TEX_TYPE::PARTNER_PEPPERMINT_OFF])))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CHud::Bind_ShaderResources()
{
	if (nullptr != m_pPlayer)
		m_pShaderCom->Bind_RawValue("g_HpPercent", &m_pPlayer->Get_StatDesc().fCurHp, sizeof(_float));

	return S_OK;
}

void CHud::Drop(_double fTimeDelta)
{
	if (!m_tLerpDesc.bActive)
	{
		if(!m_bMax)
			m_tLerpDesc.Start(0.f, 10.f, CBeatManager::GetInstance()->Get_SPB() * 0.2f, LERP_MODE::EASE_OUT);
		else
			m_tLerpDesc.Start(10.f, 0.f, CBeatManager::GetInstance()->Get_SPB() * 0.8f, LERP_MODE::EASE_OUT);

		m_bMax = !m_bMax;
	}

	m_tLerpDesc.Update(fTimeDelta);

	m_TextureLocalDesc[TEX_TYPE::DROP_LOGO].vPos.x = m_TextureLocalDescOrigin[TEX_TYPE::DROP_LOGO].vPos.x + m_tLerpDesc.fCurValue;
	m_TextureLocalDesc[TEX_TYPE::DROP_LOGO].vPos.y = m_TextureLocalDescOrigin[TEX_TYPE::DROP_LOGO].vPos.y - m_tLerpDesc.fCurValue;

	m_TextureLocalDesc[TEX_TYPE::DROP_HEALTH].vPos.x = m_TextureLocalDescOrigin[TEX_TYPE::DROP_HEALTH].vPos.x + m_tLerpDesc.fCurValue;
	m_TextureLocalDesc[TEX_TYPE::DROP_HEALTH].vPos.y = m_TextureLocalDescOrigin[TEX_TYPE::DROP_HEALTH].vPos.y - m_tLerpDesc.fCurValue;
}

void CHud::Set_Player()
{
	CGameObject* pObj = ENGINE_INSTANCE->Get_GameObject_InCurLevel_InLayerFirst(LayerNames[LAYER_PLAYER]);
	if (nullptr != pObj)
	{
		m_pPlayer = dynamic_cast<CCharacter*>(pObj);
	}
}

CHud* CHud::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHud* pInstance = new CHud(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHud");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CHud* CHud::Clone(void* pArg)
{
	CHud* pInstance = new CHud(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CHud");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHud::Free()
{
	__super::Free();
}
