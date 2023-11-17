#include "..\Default\stdafx.h"
#include "..\Public\Parry_Rhythm.h"

#include "EngineInstance.h"
#include "ImGui_Manager.h"
#include "UiManager.h"

CParry_Rhythm::CParry_Rhythm(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUi(pDevice, pContext)
{

}

CParry_Rhythm::CParry_Rhythm(const CGameObject& rhs)
	: CUi(rhs)
{

}

HRESULT CParry_Rhythm::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParry_Rhythm::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Load()))
		return E_FAIL;

	Set_State(OBJ_STATE::STATE_UNACTIVE);

	if (FAILED(CUiManager::GetInstance()->Add_UI(UI_ID::UI_PARRY_RHYTHM, this)))
		return E_FAIL;

	return S_OK;
}

void CParry_Rhythm::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CParry_Rhythm::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CParry_Rhythm::Render()
{
	if (m_TextureLocalDesc.empty()) return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	Matrix matWorldOrigin = m_pTransformCom->Get_WorldMat();

	const size_t iSize = (size_t)m_pTextureComs.size();
	for (size_t i = 0; i < iSize; i++)
	{
		if (nullptr == m_pTextureComs[i]) continue;

		if (TEX_TYPE::BEAT == i)
		{
			for (auto Pair : m_Beats)
			{
				if (Pair.first)
				{
					const Matrix matNewWorld = Matrix::CreateScale(m_TextureLocalDesc[i].vSize.x, m_TextureLocalDesc[i].vSize.y, 1.f)
						* Matrix::CreateTranslation(Pair.second.x, Pair.second.y, 0.f)
						* matWorldOrigin;

					m_pTransformCom->Set_WorldMat(matNewWorld);

					if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
						return E_FAIL;

					if (FAILED(m_pTextureComs[i]->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
						return E_FAIL;

					m_pShaderCom->Begin(0);

					m_pVIBufferCom->Render();
				}
			}
		}
		else if (TEX_TYPE::DIALOGUE == i)
		{
			const Matrix matNewWorld = Matrix::CreateScale(m_TextureLocalDesc[i].vSize.x, m_TextureLocalDesc[i].vSize.y, 1.f)
				* Matrix::CreateTranslation(m_TextureLocalDesc[i].vPos.x, m_TextureLocalDesc[i].vPos.y, 0.f)
				* matWorldOrigin;

			m_pTransformCom->Set_WorldMat(matNewWorld);

			if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
				return E_FAIL;

			if (FAILED(m_pTextureComs[i]->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
				return E_FAIL;

			m_pShaderCom->Begin(0);

			m_pVIBufferCom->Render();

		}
	}

	m_pTransformCom->Set_WorldMat(matWorldOrigin);

	return S_OK;
}

void CParry_Rhythm::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);
}

void CParry_Rhythm::Set_Beat(const _uint& iBeatCount)
{
	m_Beats.clear();
	m_Beats.shrink_to_fit();

	m_Beats.resize(iBeatCount, pair{ FALSE,Vec2::Zero });

	Calculate_BeatsPos();
}

void CParry_Rhythm::On_Beat()
{
	for (auto& Pair : m_Beats)
	{
		if (!Pair.first)
		{
			Pair.first = TRUE;
			return;
		}
	}
}

HRESULT CParry_Rhythm::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Vector Resize */
	m_pTextureComs.resize(TEX_TYPE::TYPEEND);
	m_TextureLocalDesc.resize(TEX_TYPE::TYPEEND);

	/* Load Texture */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_VFX_tk_RhythmParry_ready"),
		TEXT("Com_Texture_WORD_READY"), (CComponent**)&m_pTextureComs[TEX_TYPE::WORD_READY])))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_KorsicaGimmick_RTIcon_A"),
		TEXT("Com_Texture_DIALOGUE"), (CComponent**)&m_pTextureComs[TEX_TYPE::DIALOGUE])))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_MagnetTiming_JustBg_6"),
		TEXT("Com_Texture_BEAT"), (CComponent**)&m_pTextureComs[TEX_TYPE::BEAT])))
		return E_FAIL;

	return S_OK;
}

HRESULT CParry_Rhythm::Bind_ShaderResources()
{
	return S_OK;
}

void CParry_Rhythm::Calculate_BeatsPos()
{
	const _float	fWidth = m_TextureLocalDesc[TEX_TYPE::DIALOGUE].vSize.x;
	const Vec2		vRootPos = { m_TextureLocalDesc[TEX_TYPE::DIALOGUE].vPos.x - (fWidth * 0.5f), m_TextureLocalDesc[TEX_TYPE::DIALOGUE].vPos.y };
	const _float	fInterval = fWidth / (_float)(m_Beats.size() + 1);

	for (size_t i = 1; i <= m_Beats.size(); i++)
		m_Beats[i - 1].second = Vec2{ vRootPos.x + (fInterval * i), vRootPos.y };
}

CParry_Rhythm* CParry_Rhythm::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParry_Rhythm* pInstance = new CParry_Rhythm(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParry_Rhythm");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParry_Rhythm* CParry_Rhythm::Clone(void* pArg)
{
	CParry_Rhythm* pInstance = new CParry_Rhythm(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CParry_Rhythm");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParry_Rhythm::Free()
{
	__super::Free();
}
