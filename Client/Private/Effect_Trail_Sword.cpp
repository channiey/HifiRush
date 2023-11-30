#include "..\Default\stdafx.h"
#include "Effect_Trail_Sword.h"

#include "EngineInstance.h"

#include "EffcetManager.h"

CEffect_Trail_Sword::CEffect_Trail_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect(pDevice, pContext)

{
}

CEffect_Trail_Sword::CEffect_Trail_Sword(const CEffect_Trail_Sword& rhs)
	: CEffect(rhs)
{
}

HRESULT CEffect_Trail_Sword::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Trail_Sword::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_eEffectID = CEffectManager::EFFECT_ID::TRAIL_SWORD_CHAI;

	CEffectManager::GetInstance()->Add_Effect(this);

	return S_OK;
}

void CEffect_Trail_Sword::Tick(_double fTimeDelta)
{
	if (nullptr == m_pTargetObject)
		return;

	__super::Tick(fTimeDelta);
}

void CEffect_Trail_Sword::LateTick(_double fTimeDelta)
{
	if (nullptr == m_pTargetObject)
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CEffect_Trail_Sword::Render()
{
	if (nullptr == m_pVIBufferCom || nullptr == m_pTargetObject || m_pTextureComs.empty())
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

void CEffect_Trail_Sword::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);
}

HRESULT CEffect_Trail_Sword::Return_Pool()
{
	return 	__super::Return_Pool();
}

HRESULT CEffect_Trail_Sword::Start_Effect()
{
	if (nullptr == m_pTargetObject)
		return E_FAIL;

	/* Set Transform */
	{
		m_pTransformCom->Set_WorldMat(m_pTargetObject->Get_Transform()->Get_FinalMat());
		m_pTransformCom->Set_Scale(Vec3(10.f)); /* 이거 조절해줘야 함 */

		/* CCW, CW에 따른 회전 처리 어떻게 ?*/
	}


	Set_State(CGameObject::OBJ_STATE::STATE_ACTIVE);

	return S_OK;
}

HRESULT CEffect_Trail_Sword::Finish_Effect()
{

	return Return_Pool();
}

HRESULT CEffect_Trail_Sword::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	/* Texture */
	{
		m_pTextureComs.resize(CEffect_Trail_Sword::TEXTURE_TYPE::TEXTURE_TYPEEND);
		
		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_VFX_tk_swing_tc_02"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureComs[CEffect_Trail_Sword::TEXTURE_TYPE::DIFFUESE])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffect_Trail_Sword::Bind_ShaderResources()
{
	if (FAILED(m_pTextureComs[CEffect_Trail_Sword::TEXTURE_TYPE::DIFFUESE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_EffectDesc", &m_tEffectDesc, sizeof(EFFECT_DESC))))
		return E_FAIL;

	return S_OK;
}

CEffect_Trail_Sword* CEffect_Trail_Sword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Trail_Sword* pInstance = new CEffect_Trail_Sword(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffect_Trail_Sword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CEffect_Trail_Sword* CEffect_Trail_Sword::Clone(void* pArg)
{
	CEffect_Trail_Sword* pInstance = new CEffect_Trail_Sword(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect_Trail_Sword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Trail_Sword::Free()
{
	__super::Free();
}
