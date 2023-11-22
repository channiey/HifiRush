#include "..\Default\stdafx.h"
#include "Dynamic_Macaron_Wall.h"

#include "EngineInstance.h"
#include "Animation.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CDynamic_Macaron_Wall::CDynamic_Macaron_Wall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CDynamic(pDevice, pContext)
{
}

CDynamic_Macaron_Wall::CDynamic_Macaron_Wall(const CDynamic_Macaron_Wall& rhs)
	: CDynamic(rhs)
{
}

HRESULT CDynamic_Macaron_Wall::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDynamic_Macaron_Wall::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	CAnimation* pAnim = m_pModelCom->Get_Animation(m_AnimNames[ANIM_MA_WALL::IDLE_MA_WALL]);

	if (nullptr == pAnim) return E_FAIL;

	m_pModelCom->Set_Animation(pAnim, pAnim->Get_TickPerFrame(), DF_TW_TIME);

	return S_OK;
}

void CDynamic_Macaron_Wall::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CDynamic_Macaron_Wall::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CDynamic_Macaron_Wall::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDynamic_Macaron_Wall::Set_On()
{
	CAnimation* pAnim = m_pModelCom->Get_Animation(m_AnimNames[ANIM_MA_WALL::ACTIVE_MA_WALL]);

	if (nullptr == pAnim) return E_FAIL;

	m_pModelCom->Set_Animation(pAnim, pAnim->Get_TickPerFrame(), DF_TW_TIME);

	return S_OK;
}

HRESULT CDynamic_Macaron_Wall::Set_Off()
{
	CAnimation* pAnim = m_pModelCom->Get_Animation(m_AnimNames[ANIM_MA_WALL::IDLE_MA_WALL]);

	if (nullptr == pAnim) return E_FAIL;

	m_pModelCom->Set_Animation(pAnim, pAnim->Get_TickPerFrame(), DF_TW_TIME);

	return S_OK;
}

HRESULT CDynamic_Macaron_Wall::Ready_Components()
{
	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Crane"),
		ComponentNames[COM_MODEL], (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDynamic_Macaron_Wall::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::STATE_VIEW)))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::STATE_PROJ)))
		return E_FAIL;

	return S_OK;
}

CDynamic_Macaron_Wall* CDynamic_Macaron_Wall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDynamic_Macaron_Wall* pInstance = new CDynamic_Macaron_Wall(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDynamic_Macaron_Wall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CDynamic_Macaron_Wall* CDynamic_Macaron_Wall::Clone(void* pArg)
{
	CDynamic_Macaron_Wall* pInstance = new CDynamic_Macaron_Wall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDynamic_Macaron_Wall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDynamic_Macaron_Wall::Free()
{
	__super::Free();
}
