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
	if (FAILED(Ready_Components()))
		return E_FAIL;

	/*CAnimation* pAnim = m_pModelCom->Get_Animation(m_AnimNames[ANIM_MA_WALL::IDLE_MA_WALL]);

	if (nullptr == pAnim) return E_FAIL;

	m_pModelCom->Set_Animation(pAnim, pAnim->Get_TickPerFrame(), DF_TW_TIME);*/

	return S_OK;
}

void CDynamic_Macaron_Wall::Tick(_double fTimeDelta)
{
	if (nullptr != m_pCollider && m_pCollider->Is_Active())
		m_pCollider->Update(m_pTransformCom->Get_FinalMat());

	Check_Progress(fTimeDelta);
}

void CDynamic_Macaron_Wall::LateTick(_double fTimeDelta)
{
	if (CDynamic_Macaron_Wall::STATE_TYPE::BROKEN == m_eStateType)
	{
		if (FAILED(m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::DEBRIS]->Update(fTimeDelta)))
			return;
	}

	if (nullptr != m_pCollider && m_pCollider->Is_Active() && CImGui_Manager::GetInstance()->Is_Render_Collider())
		m_pRendererCom->Add_Debug(m_pCollider);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
}

HRESULT CDynamic_Macaron_Wall::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (CDynamic_Macaron_Wall::STATE_TYPE::UNBROKEN == m_eStateType)
	{
		/* Static - UnBroken */
		for (_uint i = 0; i < m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::UNBROKEN]->Get_MeshCount(); ++i)
		{
			if (FAILED(m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::UNBROKEN]->Bind_Material(
				m_pShaderCom_Static, 
				m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::UNBROKEN]->Get_MaterialIndex(i),
				aiTextureType_DIFFUSE, 
				"g_DiffuseTexture")))
				return E_FAIL;

			if (FAILED(m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::UNBROKEN]->Render(m_pShaderCom_Static, i)))
				return E_FAIL;
		}
	}
	else if (CDynamic_Macaron_Wall::STATE_TYPE::BROKEN == m_eStateType)
	{
		/* Static - Broken */
		for (_uint i = 0; i < m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::BROKEN]->Get_MeshCount(); ++i)
		{
			if (FAILED(m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::BROKEN]->Bind_Material(
				m_pShaderCom_Static,
				m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::BROKEN]->Get_MaterialIndex(i),
				aiTextureType_DIFFUSE,
				"g_DiffuseTexture")))
				return E_FAIL;

			if (FAILED(m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::BROKEN]->Render(m_pShaderCom_Static, i)))
				return E_FAIL;
		}

		/* Dynamic - Broken */
		for (_uint i = 0; i < m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::DEBRIS]->Get_MeshCount(); ++i)
		{
			if (FAILED(m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::DEBRIS]->Bind_Material(
				m_pShaderCom_Dynamic,
				m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::DEBRIS]->Get_MaterialIndex(i),
				aiTextureType_DIFFUSE,
				"g_DiffuseTexture")))
				return E_FAIL;

			if (FAILED(m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::DEBRIS]->Render(m_pShaderCom_Dynamic, i)))
				return E_FAIL;
		}
	}

	/* Render Collider */
	if (nullptr != m_pCollider && m_pCollider->Is_Active() && CImGui_Manager::GetInstance()->Is_Render_Collider())
	{
		m_pCollider->Render();
	}



	return S_OK;
}

HRESULT CDynamic_Macaron_Wall::Set_On()
{
	m_eStateType = CDynamic_Macaron_Wall::STATE_TYPE::BROKEN;

	m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::DEBRIS]->Stop_Animation(FALSE);

	m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::DEBRIS]->Clear_Animation();

	CAnimation* pAnim = m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::DEBRIS]->Get_Animation(m_AnimNames[ANIM_MA_WALL::BROKE_WALL]);

	if (nullptr == pAnim) return E_FAIL;

	m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::DEBRIS]->Set_Animation(pAnim, pAnim->Get_TickPerFrame() * 0.75f, DF_TW_TIME);

	return S_OK;
}

HRESULT CDynamic_Macaron_Wall::Set_Off()
{
	m_eStateType = CDynamic_Macaron_Wall::STATE_TYPE::UNBROKEN;

	/*CAnimation* pAnim = m_pModelCom->Get_Animation(m_AnimNames[ANIM_MA_WALL::IDLE_MA_WALL]);

	if (nullptr == pAnim) return E_FAIL;

	m_pModelCom->Set_Animation(pAnim, pAnim->Get_TickPerFrame(), DF_TW_TIME);*/

	return S_OK;
}

void CDynamic_Macaron_Wall::Check_Progress(_double fTimeDelta)
{
	if (CDynamic_Macaron_Wall::STATE_TYPE::BROKEN == m_eStateType)
	{
		if (115 == m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::DEBRIS]->Get_TweenDesc().cur.iCurFrame)
		{
			m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::DEBRIS]->Stop_Animation(TRUE);
		}
	}
}

HRESULT CDynamic_Macaron_Wall::Ready_Components()
{
	/* Com_Shaders*/
	{
		/* Dynamic */
		if (FAILED(__super::Add_Component(LV_STATIC, ShaderNames[SHADER_VTF],
			TEXT("Com_Shader_Dynamic"), (CComponent**)&m_pShaderCom_Dynamic)))
			return E_FAIL;

		/* Static */
		if (FAILED(__super::Add_Component(LV_STATIC, ShaderNames[SHADER_STATIC],
			TEXT("Com_Shader_Static"), (CComponent**)&m_pShaderCom_Static)))
			return E_FAIL;
	}

	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Renderer"),
		ComponentNames[COM_RENDERER], (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"),
		ComponentNames[COM_TRANSFORM], (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Model */
	{
		/* UnBroken */
		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Wall_Macaron_UnBroken"),
			TEXT("Com_Model_UnBroken"), (CComponent**)&m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::UNBROKEN])))
			return E_FAIL;


		/* Broken */
		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Wall_Macaron_Broken"),
			TEXT("Com_Model_Broken"), (CComponent**)&m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::BROKEN])))
			return E_FAIL;


		/* Debris */
		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_DoorDebris"),
			TEXT("Com_Model_Debris"), (CComponent**)&m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::DEBRIS])))
			return E_FAIL;

	}
	

	return S_OK;
}

HRESULT CDynamic_Macaron_Wall::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom_Static, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom_Static, "g_ViewMatrix", CPipeLine::STATE_VIEW)))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom_Static, "g_ProjMatrix", CPipeLine::STATE_PROJ)))
		return E_FAIL;

	if (CDynamic_Macaron_Wall::STATE_TYPE::BROKEN == m_eStateType)
	{
		Matrix matOrigin = m_pTransformCom->Get_FinalMat();

		m_pTransformCom->Rotate(Vec4::UnitY, DEG2RAD(90.f));
		m_pTransformCom->Translate(m_pTransformCom->Get_State(CTransform::STATE_RIGHT) * 6.f);

		if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom_Dynamic, "g_WorldMatrix")))
			return E_FAIL;

		if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom_Dynamic, "g_ViewMatrix", CPipeLine::STATE_VIEW)))
			return E_FAIL;

		if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom_Dynamic, "g_ProjMatrix", CPipeLine::STATE_PROJ)))
			return E_FAIL;

		m_pTransformCom->Set_WorldMat(matOrigin);
	}

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
