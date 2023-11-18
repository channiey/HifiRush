#include "..\Default\stdafx.h"
#include "..\Public\Ui_Hud_Boss.h"

#include "EngineInstance.h"
#include "ImGui_Manager.h"
#include "BeatManager.h"
#include "Character.h"
#include "UiManager.h"

CUi_Hud_Boss::CUi_Hud_Boss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUi(pDevice, pContext)
{

}

CUi_Hud_Boss::CUi_Hud_Boss(const CGameObject& rhs)
	: CUi(rhs)
{

}

HRESULT CUi_Hud_Boss::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUi_Hud_Boss::Initialize(void* pArg)
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

	if (FAILED(CUiManager::GetInstance()->Add_UI(UI_ID::UI_HUD_BOSS, this)))
		return E_FAIL;

	return S_OK;
}

void CUi_Hud_Boss::Tick(_double fTimeDelta)
{
	if (nullptr == m_pBoss) return;

	__super::Tick(fTimeDelta);

	Drop(fTimeDelta);
}

void CUi_Hud_Boss::LateTick(_double fTimeDelta)
{
	if (nullptr == m_pBoss) return;

	__super::LateTick(fTimeDelta);
}

HRESULT CUi_Hud_Boss::Render()
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

		if (HEALTH_CRACKED_0 <= i && HEALTH_CRACKED_3 >= i)
			continue;
		else if (HEALTH_FULL_0 <= i && HEALTH_FULL_3 >= i)
		{
			const _float fHpPercent = m_pBoss->Get_StatDesc().fCurHp / m_pBoss->Get_StatDesc().fMaxHp;
		
			if (fHpPercent < 0.25f)
			{
				if (HEALTH_FULL_0 == i)
				{
					m_pShaderCom->Begin(1);
				}
				else if (HEALTH_FULL_0 < i)
					continue;
				else
					m_pShaderCom->Begin(0);
			}
			else if (fHpPercent < 0.5f)
			{
				if (HEALTH_FULL_1 == i)
				{
					m_pShaderCom->Begin(1);
				}
				else if (HEALTH_FULL_1 < i)
					continue;
				else
					m_pShaderCom->Begin(0);
			}
			else if (fHpPercent < 0.75f)
			{
				if (HEALTH_FULL_2 == i)
				{
					m_pShaderCom->Begin(1);
				}
				else if (HEALTH_FULL_2 < i)
					continue;
				else
					m_pShaderCom->Begin(0);
			}
			else if (0.75f <= fHpPercent)
			{
				if (HEALTH_FULL_3 == i)
				{
					m_pShaderCom->Begin(1);
				}
				else
					m_pShaderCom->Begin(0);
			}
		}
		else if (HEALTH_DAMAGED_0 <= i && HEALTH_DAMAGED_3 >= i)
		{
			continue;
		}
		else
			m_pShaderCom->Begin(0);

		m_pVIBufferCom->Render();
	}

	m_pTransformCom->Set_WorldMat(matWorldOrigin);

	return S_OK;
}

HRESULT CUi_Hud_Boss::Set_Target(CGameObject* pTarget)
{
	if (nullptr == pTarget) 
		return E_FAIL;

	m_pBoss = dynamic_cast<CCharacter*>(pTarget);

	if (nullptr == m_pBoss)
		return E_FAIL;

	return S_OK;
}

HRESULT CUi_Hud_Boss::Ready_Components()
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
			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_ST01Boss_HealthBar_DropShadow"),
				TEXT("Com_Texture_DROP_HEALTH"), (CComponent**)&m_pTextureComs[CUi_Hud_Boss::TEX_TYPE::DROP_HEALTH])))
				return E_FAIL;
		}

		/* Side */
		{
			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_ST01Boss_SideIcon_1"),
				TEXT("Com_Texture_SIDE_ICON_LEFT"), (CComponent**)&m_pTextureComs[CUi_Hud_Boss::TEX_TYPE::SIDE_ICON_LEFT])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_ST01Boss_SideIcon_2"),
				TEXT("Com_Texture_SIDE_ICON_RIGHT"), (CComponent**)&m_pTextureComs[CUi_Hud_Boss::TEX_TYPE::SIDE_ICON_RIGHT])))
				return E_FAIL;
		}

		/* Health */
		{
			/* Empty */
			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_ST01Boss_BG_1"),
				TEXT("Com_Texture_HEALTH_EMPTY_0"), (CComponent**)&m_pTextureComs[CUi_Hud_Boss::TEX_TYPE::HEALTH_EMPTY_0])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_ST01Boss_BG_2"),
				TEXT("Com_Texture_HEALTH_EMPTY_1"), (CComponent**)&m_pTextureComs[CUi_Hud_Boss::TEX_TYPE::HEALTH_EMPTY_1])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_ST01Boss_BG_3"),
				TEXT("Com_Texture_HEALTH_EMPTY_2"), (CComponent**)&m_pTextureComs[CUi_Hud_Boss::TEX_TYPE::HEALTH_EMPTY_2])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_ST01Boss_BG_4"),
				TEXT("Com_Texture_HEALTH_EMPTY_3"), (CComponent**)&m_pTextureComs[CUi_Hud_Boss::TEX_TYPE::HEALTH_EMPTY_3])))
				return E_FAIL;

			/* Cracked */
			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_ST01Boss_DamagedBG_1"),
				TEXT("Com_Texture_HEALTH_CRACKED_0"), (CComponent**)&m_pTextureComs[CUi_Hud_Boss::TEX_TYPE::HEALTH_CRACKED_0])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_ST01Boss_DamagedBG_2"),
				TEXT("Com_Texture_HEALTH_CRACKED_1"), (CComponent**)&m_pTextureComs[CUi_Hud_Boss::TEX_TYPE::HEALTH_CRACKED_1])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_ST01Boss_DamagedBG_3"),
				TEXT("Com_Texture_HEALTH_CRACKED_2"), (CComponent**)&m_pTextureComs[CUi_Hud_Boss::TEX_TYPE::HEALTH_CRACKED_2])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_ST01Boss_DamagedBG_4"),
				TEXT("Com_Texture_HEALTH_CRACKED_3"), (CComponent**)&m_pTextureComs[CUi_Hud_Boss::TEX_TYPE::HEALTH_CRACKED_3])))
				return E_FAIL;

			/* Full */
			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_ST01Boss_HealthBar_1"),
				TEXT("Com_Texture_HEALTH_FULL_0"), (CComponent**)&m_pTextureComs[CUi_Hud_Boss::TEX_TYPE::HEALTH_FULL_0])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_ST01Boss_HealthBar_2"),
				TEXT("Com_Texture_HEALTH_FULL_1"), (CComponent**)&m_pTextureComs[CUi_Hud_Boss::TEX_TYPE::HEALTH_FULL_1])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_ST01Boss_HealthBar_3"),
				TEXT("Com_Texture_HEALTH_FULL_2"), (CComponent**)&m_pTextureComs[CUi_Hud_Boss::TEX_TYPE::HEALTH_FULL_2])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_ST01Boss_HealthBar_4"),
				TEXT("Com_Texture_HEALTH_FULL_3"), (CComponent**)&m_pTextureComs[CUi_Hud_Boss::TEX_TYPE::HEALTH_FULL_3])))
				return E_FAIL;

			/* Damaged */
			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_ST01Boss_HealthBar_DamageBlur_1"),
				TEXT("Com_Texture_HEALTH_DAMAGED_0"), (CComponent**)&m_pTextureComs[CUi_Hud_Boss::TEX_TYPE::HEALTH_DAMAGED_0])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_ST01Boss_HealthBar_DamageBlur_2"),
				TEXT("Com_Texture_HEALTH_DAMAGED_1"), (CComponent**)&m_pTextureComs[CUi_Hud_Boss::TEX_TYPE::HEALTH_DAMAGED_1])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_ST01Boss_HealthBar_DamageBlur_3"),
				TEXT("Com_Texture_HEALTH_DAMAGED_2"), (CComponent**)&m_pTextureComs[CUi_Hud_Boss::TEX_TYPE::HEALTH_DAMAGED_2])))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_ST01Boss_HealthBar_DamageBlur_4"),
				TEXT("Com_Texture_HEALTH_DAMAGED_3"), (CComponent**)&m_pTextureComs[CUi_Hud_Boss::TEX_TYPE::HEALTH_DAMAGED_3])))
				return E_FAIL;
		}

		/* Logo */
		{
			if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_ST01Boss_Name"),
				TEXT("Com_Texture_LOGO"), (CComponent**)&m_pTextureComs[CUi_Hud_Boss::TEX_TYPE::LOGO])))
				return E_FAIL;

		}
	}

	return S_OK;
}

HRESULT CUi_Hud_Boss::Bind_ShaderResources()
{		
	const _float fHpPercent = m_pBoss->Get_StatDesc().fCurHp / m_pBoss->Get_StatDesc().fMaxHp;

	m_pShaderCom->Bind_RawValue("g_HpPercent", &fHpPercent, sizeof(_float));

	return S_OK;
}

void CUi_Hud_Boss::Drop(_double fTimeDelta)
{
	if (!m_tLerpDesc.bActive)
	{
		if (!m_bMax)
			m_tLerpDesc.Start(0.f, 10.f, CBeatManager::GetInstance()->Get_SPB() * 0.2f, LERP_MODE::EASE_OUT);
		else
			m_tLerpDesc.Start(10.f, 0.f, CBeatManager::GetInstance()->Get_SPB() * 0.8f, LERP_MODE::EASE_OUT);

		m_bMax = !m_bMax;
	}

	m_tLerpDesc.Update(fTimeDelta);

	m_TextureLocalDesc[TEX_TYPE::DROP_HEALTH].vPos.y = m_TextureLocalDescOrigin[TEX_TYPE::DROP_HEALTH].vPos.y - m_tLerpDesc.fCurValue - 7.f;
}

CUi_Hud_Boss* CUi_Hud_Boss::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUi_Hud_Boss* pInstance = new CUi_Hud_Boss(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUi_Hud_Boss");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CUi_Hud_Boss* CUi_Hud_Boss::Clone(void* pArg)
{
	CUi_Hud_Boss* pInstance = new CUi_Hud_Boss(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUi_Hud_Boss");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Hud_Boss::Free()
{
	__super::Free();
}
