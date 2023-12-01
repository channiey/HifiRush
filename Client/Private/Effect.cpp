#include "..\Default\stdafx.h"
#include "Effect.h"

#include "EngineInstance.h"


CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CEffect::CEffect(const CEffect& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CEffect::Tick(_double fTimeDelta)
{
	if (Is_Finish_LifeTime(fTimeDelta))
	{
		Finish_Effect();
		return;
	}

	__super::Tick(fTimeDelta);
}

void CEffect::LateTick(_double fTimeDelta)
{
	if (Is_Finish_LifeTime(fTimeDelta))
	{
		Finish_Effect();
		return;
	}

	__super::LateTick(fTimeDelta);

	if (!m_pModelComs.empty())
	{
		for (auto& pModelCom : m_pModelComs)
		{
			if (nullptr == pModelCom) continue;

			pModelCom->Update(fTimeDelta);
		}
	}

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
}

HRESULT CEffect::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

void CEffect::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);
}

void CEffect::Set_TextureIndex(const _uint& iIndex)
{
	if (m_pTextureComs.size() <= iIndex)
		return;

	m_iTextureIndex = iIndex;
}

HRESULT CEffect::Return_Pool()
{
	return ENGINE_INSTANCE->Return_Pool(ENGINE_INSTANCE->Get_CurLevelIndex(), this);
}

HRESULT CEffect::Ready_Components()
{
	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Renderer"),
		ComponentNames[COM_RENDERER], (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"),
		ComponentNames[COM_TRANSFORM], (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* 모델과 텍스처는 파생 클래스에서 스스로 추가한다. */

	return S_OK;
}

HRESULT CEffect::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::STATE_VIEW)))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::STATE_PROJ)))
		return E_FAIL;

	return S_OK;
}

const _bool CEffect::Is_Finish_LifeTime(_double fTimeDelta)
{
	m_tTimeDesc.Update(fTimeDelta);

	return m_tTimeDesc.bFull;
}

void CEffect::Free()
{
	__super::Free();
}
