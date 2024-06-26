#include "..\Default\stdafx.h"
#include "..\Public\Ui_Hud.h"

#include "EngineInstance.h"

#include "ImGui_Manager.h"
#include "BeatManager.h"
#include "UiManager.h"

#include "PlayerController.h"
#include "Chai.h"


CUi_Hud::CUi_Hud(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUi(pDevice, pContext)
{

}

CUi_Hud::CUi_Hud(const CGameObject& rhs)
	: CUi(rhs)
{

}

HRESULT CUi_Hud::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUi_Hud::Initialize(void* pArg)
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

void CUi_Hud::Tick(_double fTimeDelta)
{
	if (nullptr == m_pChai)
		Set_Chai();

	__super::Tick(fTimeDelta);

	Drop(fTimeDelta);
}

void CUi_Hud::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUi_Hud::Render()
{
	if (m_TextureLocalDesc.empty()) return E_FAIL;

	if (FAILED(__super::Render())) return E_FAIL;

	if (FAILED(Bind_ShaderResources())) return E_FAIL;

	Matrix matWorldOrigin = m_pTransformCom->Get_WorldMat();

	CChai::CHAI_DESC tChaiDesc = m_pChai->Get_ChaiDesc();

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
			if (tChaiDesc.Is_Full_ReverbGuage())
			{
				if (LOGO_SPECIAL == i)
					m_pShaderCom->Begin(0);
				else
					continue;
			}
			else
			{
				if(LOGO_NORMAL == i)
					m_pShaderCom->Begin(0);
				else
					continue;
			}
		}
		else if (HEALTH_EMPTY <= i && HEALTH_DAMAGED >= i)
		{
			if (HEALTH_DAMAGED == i)
				continue;

			if (HEALTH_FULL == i)
				m_pShaderCom->Begin(1);
			else
				m_pShaderCom->Begin(0);
		}
		else if (SPECIAL_EMPTY_0 <= i && SPECIAL_FULL_2 >= i) /* 리버브 게이지 */
		{
			_float fReverbGuagePercent = tChaiDesc.Get_ReverbGuage_Percent();

			if (fReverbGuagePercent <= 0.33f)
			{
				if (SPECIAL_FULL_0 == i)
				{
					fReverbGuagePercent /= 0.33f;
					m_pShaderCom->Bind_RawValue("g_SpPercent", &fReverbGuagePercent, sizeof(_float));

					m_pShaderCom->Begin(2);
				}
				else if (SPECIAL_EMPTY_0 == i || SPECIAL_EMPTY_1 == i || SPECIAL_EMPTY_2 == i)
					m_pShaderCom->Begin(0);
				else
					continue;
			}
			else if (fReverbGuagePercent <= 0.66f)
			{
				if (SPECIAL_FULL_1 == i)
				{
					fReverbGuagePercent = (fReverbGuagePercent - 0.33f) / 0.33f;
					m_pShaderCom->Bind_RawValue("g_SpPercent", &fReverbGuagePercent, sizeof(_float));
					m_pShaderCom->Begin(2);
				}
				else if (SPECIAL_EMPTY_0 == i || SPECIAL_EMPTY_1 == i || SPECIAL_EMPTY_2 == i || SPECIAL_FULL_0 == i)
					m_pShaderCom->Begin(0);
				else
					continue;
			}
			else
			{
				if (SPECIAL_FULL_2 == i)
				{
					fReverbGuagePercent = (fReverbGuagePercent - 0.66f) / 0.33f;
					m_pShaderCom->Bind_RawValue("g_SpPercent", &fReverbGuagePercent, sizeof(_float));
					m_pShaderCom->Begin(2);
				}
				else if (SPECIAL_EMPTY_0 == i || SPECIAL_EMPTY_1 == i || SPECIAL_EMPTY_2 == i || SPECIAL_FULL_0 == i || SPECIAL_FULL_1 == i)
					m_pShaderCom->Begin(0);
				else
					continue;
			}
		}
		else if (PARTNER_MACARON_ON <= i && PARTNER_PEPPERMINT_OFF >= i)
		{
			if (PARTNER_MACARON_ON <= i && PARTNER_MACARON_OFF >= i)
			{
				PLAYER_STATE eState = CPlayerController::GetInstance()->Get_PlayerState(PLAYER_TYPE::MACARON);
				switch (eState)
				{
				case WAIT:
				{
					if (PARTNER_MACARON_ON == i)
						m_pShaderCom->Begin(0);
				}
					break;
				case APPEAR:
				{
					if (PARTNER_MACARON_OFF == i)
						m_pShaderCom->Begin(0);
				}
					break;
				case COOLTIME:
				{
					if (PARTNER_MACARON_ON == i)
						m_pShaderCom->Begin(0);

					if (PARTNER_MACARON_OFF == i)
					{
						const PLAYER_DESC	tDesc = CPlayerController::GetInstance()->Get_PlayerDesc(PLAYER_TYPE::MACARON);
						const _float		fCoolTimePercent = tDesc.fCoolTimeAcc / tDesc.fCoolTimeLimit;

						m_pShaderCom->Bind_RawValue("g_CoolTime", &fCoolTimePercent, sizeof(_float));
						m_pShaderCom->Begin(3);
					}
				}
					break;
				default:
					break;
				}
			}
			else if (PARTNER_KORSICA_ON <= i && PARTNER_KORSICA_OFF >= i)
			{
				PLAYER_STATE eState = CPlayerController::GetInstance()->Get_PlayerState(PLAYER_TYPE::KORSICA);
				switch (eState)
				{
				case WAIT:
				{
					if (PARTNER_KORSICA_ON == i)
						m_pShaderCom->Begin(0);
				}
				break;
				case APPEAR:
				{
					if (PARTNER_KORSICA_OFF == i)
						m_pShaderCom->Begin(0);
				}
				break;
				case COOLTIME:
				{
					if (PARTNER_KORSICA_ON == i)
						m_pShaderCom->Begin(0);

					if (PARTNER_KORSICA_OFF == i)
					{
						const PLAYER_DESC	tDesc = CPlayerController::GetInstance()->Get_PlayerDesc(PLAYER_TYPE::KORSICA);
						const _float		fCoolTimePercent = tDesc.fCoolTimeAcc / tDesc.fCoolTimeLimit;

						m_pShaderCom->Bind_RawValue("g_CoolTime", &fCoolTimePercent, sizeof(_float));
						m_pShaderCom->Begin(3);
					}
				}
				break;
				default:
					break;
				}
			}
			else if (PARTNER_PEPPERMINT_ON <= i && PARTNER_PEPPERMINT_OFF >= i)
			{
				PLAYER_STATE eState = CPlayerController::GetInstance()->Get_PlayerState(PLAYER_TYPE::PEPPERMINT);
				switch (eState)
				{
				case WAIT:
				{
					if (PARTNER_PEPPERMINT_ON == i)
						m_pShaderCom->Begin(0);
				}
				break;
				case APPEAR:
				{
					if (PARTNER_PEPPERMINT_OFF == i)
						m_pShaderCom->Begin(0);
				}
				break;
				case COOLTIME:
				{
					if (PARTNER_PEPPERMINT_ON == i)
						m_pShaderCom->Begin(0);

					if (PARTNER_PEPPERMINT_OFF == i)
					{
						const PLAYER_DESC	tDesc = CPlayerController::GetInstance()->Get_PlayerDesc(PLAYER_TYPE::PEPPERMINT);
						const _float		fCoolTimePercent = tDesc.fCoolTimeAcc / tDesc.fCoolTimeLimit;

						m_pShaderCom->Bind_RawValue("g_CoolTime", &fCoolTimePercent, sizeof(_float));
						m_pShaderCom->Begin(3);
					}
				}
				break;
				default:
					break;
				}
			}

			

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

HRESULT CUi_Hud::Ready_Components()
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

HRESULT CUi_Hud::Bind_ShaderResources()
{
	const _float fHpPercent = m_pChai->Get_StatDesc().fCurHp / m_pChai->Get_StatDesc().fMaxHp;
	const _float fReverbPercent = m_pChai->Get_ChaiDesc().Get_ReverbGuage_Percent();
	m_pShaderCom->Bind_RawValue("g_SpPercent", &fReverbPercent, sizeof(_float));

	if (nullptr != m_pChai)
	{
		m_pShaderCom->Bind_RawValue("g_HpPercent", &fHpPercent, sizeof(_float));
	}

	return S_OK;
}

void CUi_Hud::Drop(_double fTimeDelta)
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

void CUi_Hud::Set_Chai()
{
	CGameObject* pObj = ENGINE_INSTANCE->Get_GameObject_InCurLevel_InLayerFirst(LayerNames[LAYER_PLAYER]);
	if (nullptr != pObj)
	{
		m_pChai = dynamic_cast<CChai*>(pObj);
	}
}

CUi_Hud* CUi_Hud::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUi_Hud* pInstance = new CUi_Hud(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUi_Hud");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CUi_Hud* CUi_Hud::Clone(void* pArg)
{
	CUi_Hud* pInstance = new CUi_Hud(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUi_Hud");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Hud::Free()
{
	__super::Free();
}
