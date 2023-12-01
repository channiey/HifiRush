#include "..\Default\stdafx.h"
#include "Effect_Damaged_Enemy.h"

#include "EngineInstance.h"

#include "EffectManager.h"

CEffect_Damaged_Enemy::CEffect_Damaged_Enemy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect(pDevice, pContext)

{
}

CEffect_Damaged_Enemy::CEffect_Damaged_Enemy(const CEffect_Damaged_Enemy& rhs)
	: CEffect(rhs)
{
}

HRESULT CEffect_Damaged_Enemy::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Damaged_Enemy::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_eEffectID = CEffectManager::EFFECT_ID::DAMAGED_EMENY;

	Set_State(CGameObject::STATE_UNACTIVE);

	m_tTimeDesc.fTimeLimit = 0.1f;

	return S_OK;
}

void CEffect_Damaged_Enemy::Tick(_double fTimeDelta)
{
	if (1.f <= m_tEffectDesc.vUVOffset2.x)
	{
		Finish_Effect();
		return;
	}

	m_tTimeDesc.Update(fTimeDelta);

	Update_UV(fTimeDelta);
}

void CEffect_Damaged_Enemy::LateTick(_double fTimeDelta)
{
	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RG_BLEND, this)))
		return;
}

HRESULT CEffect_Damaged_Enemy::Render()
{
	if (nullptr == m_pVIBufferCom || m_pTextureComs.empty())
		return E_FAIL;

	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(1)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Damaged_Enemy::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);
}

HRESULT CEffect_Damaged_Enemy::Return_Pool()
{
	return 	__super::Return_Pool();
}

HRESULT CEffect_Damaged_Enemy::Start_Effect()
{
	Set_State(CGameObject::OBJ_STATE::STATE_ACTIVE);

	m_tTimeDesc.Reset();
	m_tEffectDesc.Reset();

	return S_OK;
}

HRESULT CEffect_Damaged_Enemy::Finish_Effect()
{
	Set_State(CGameObject::OBJ_STATE::STATE_UNACTIVE);

	return S_OK;
}

HRESULT CEffect_Damaged_Enemy::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LV_STATIC, ShaderNames[SHADER_EFFECT_WORLDTEX],
		ComponentNames[COM_SHADER], (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* Texture */
	{
		m_pTextureComs.resize(CEffect_Damaged_Enemy::TEXTURE_TYPE::TEXTURE_TYPEEND);

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_VFX_tk_swing_tc_02"),
			TEXT("Com_Texture_TRAIL_LIGHT"), (CComponent**)&m_pTextureComs[CEffect_Damaged_Enemy::TEXTURE_TYPE::TRAIL_LIGHT])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_VFX_tk_swing_tc_05"),
			TEXT("Com_Texture_TRAIL_MIDDLE"), (CComponent**)&m_pTextureComs[CEffect_Damaged_Enemy::TEXTURE_TYPE::TRAIL_MIDDLE])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_VFX_ym_swing_tc_01"),
			TEXT("Com_Texture_TRAIL_HEAVY"), (CComponent**)&m_pTextureComs[CEffect_Damaged_Enemy::TEXTURE_TYPE::TRAIL_HEAVY])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffect_Damaged_Enemy::Bind_ShaderResources()
{
	if (FAILED(m_pTextureComs[m_iTextureIndex]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_EffectDesc", &m_tEffectDesc, sizeof(EFFECT_DESC))))
		return E_FAIL;

	return S_OK;
}

void CEffect_Damaged_Enemy::Update_UV(_double fTimeDelta)
{
	/* vUVOffset1 - Head */
	m_tEffectDesc.vUVOffset1.x = m_tTimeDesc.fPercentage;

	/* vUVOffset2 - Tail */
	if (m_tTimeDesc.bFull)
	{
		m_tEffectDesc.vUVOffset2.x += fTimeDelta * 4.5f;
		if (1.f < m_tEffectDesc.vUVOffset2.x)
			m_tEffectDesc.vUVOffset2.x = 1.f;
	}
	else
	{
		m_tEffectDesc.vUVOffset2.x = m_tTimeDesc.fPercentage - m_fUVDelta;
		if (m_tEffectDesc.vUVOffset2.x < 0)
			m_tEffectDesc.vUVOffset2.x = 0.f;
	}
}

CEffect_Damaged_Enemy* CEffect_Damaged_Enemy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Damaged_Enemy* pInstance = new CEffect_Damaged_Enemy(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Damaged_Enemy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CEffect_Damaged_Enemy* CEffect_Damaged_Enemy::Clone(void* pArg)
{
	CEffect_Damaged_Enemy* pInstance = new CEffect_Damaged_Enemy(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Damaged_Enemy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Damaged_Enemy::Free()
{
	__super::Free();
}
