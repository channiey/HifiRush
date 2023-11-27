#include "..\Default\stdafx.h"
#include "Dynamic_AutoDoor.h"

#include "EngineInstance.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CDynamic_AutoDoor::CDynamic_AutoDoor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CDynamic(pDevice, pContext)
{
}

CDynamic_AutoDoor::CDynamic_AutoDoor(const CDynamic_AutoDoor& rhs)
	: CDynamic(rhs)
{
}

HRESULT CDynamic_AutoDoor::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDynamic_AutoDoor::Initialize(void* pArg)
{
	/*if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;*/

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CDynamic_AutoDoor::Tick(_double fTimeDelta)
{
	if (nullptr != m_pCollider && m_pCollider->Is_Active())
		m_pCollider->Update(m_pTransformCom->Get_FinalMat());

	if (m_tLerpDesc.bActive)
		m_tLerpDesc.Update(fTimeDelta);
}

void CDynamic_AutoDoor::LateTick(_double fTimeDelta)
{
	if (nullptr != m_pCollider && m_pCollider->Is_Active() && CImGui_Manager::GetInstance()->Is_Render_Collider())
		m_pRendererCom->Add_Debug(m_pCollider);

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
}

HRESULT CDynamic_AutoDoor::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	Matrix matWorldOrigin = m_pTransformCom->Get_FinalMat();

	for (size_t i = 0; i < CDynamic_AutoDoor::MODEL_TYPE::MODEL_TYPEEND; i++)
	{
		for (_uint j = 0; j < m_pModelComs[i]->Get_MeshCount(); ++j)
		{
			Vec4 vRelativePos = Vec4::UnitW;

			if (CDynamic_AutoDoor::MODEL_TYPE::LEFT_DOOR == i)
				vRelativePos.z = m_tLerpDesc.fCurValue * -1.f; /* 축반대라서 z에 넣어줌 */
			else
				vRelativePos.z = m_tLerpDesc.fCurValue;

			vRelativePos = m_pTransformCom->Get_RelativePosition(vRelativePos);

			m_pTransformCom->Set_Position(m_pTransformCom->Get_FinalPosition() + vRelativePos.ZeroW());

			if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
				return E_FAIL;

			if (FAILED(m_pModelComs[i]->Bind_Material(m_pShaderCom, m_pModelComs[i]->Get_MaterialIndex(j), 
				aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;

			if (FAILED(m_pModelComs[i]->Render(m_pShaderCom, j)))
				return E_FAIL;

			m_pTransformCom->Set_WorldMat(matWorldOrigin);
		}
	}


	/* Render Collider */
	if (nullptr != m_pCollider && m_pCollider->Is_Active() && CImGui_Manager::GetInstance()->Is_Render_Collider())
	{
		m_pCollider->Render();
	}

	return S_OK;
}

void CDynamic_AutoDoor::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	if (L"Player_Chai_000" == pCollider->Get_Owner()->Get_Name())
	{
		Set_On();
	}
}

void CDynamic_AutoDoor::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
}

void CDynamic_AutoDoor::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
	if (L"Player_Chai_000" == pCollider->Get_Owner()->Get_Name())
	{
		Set_Off();
	}
}

HRESULT CDynamic_AutoDoor::Set_On()
{
	/* Open */
	m_tLerpDesc.Start(m_fOriginX, m_fTargetX, m_fLerpTime, LERP_MODE::EXPONENTIAL);

	ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_OBJ_DOOR_OPEN, CHANNEL_ID::ETC_OBJ, 0.7f);
	
	return S_OK;
}

HRESULT CDynamic_AutoDoor::Set_Off()
{
	/* Closed */
	m_tLerpDesc.Start(m_fTargetX, m_fOriginX, m_fLerpTime, LERP_MODE::EXPONENTIAL);

	ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_OBJ_DOOR_CLOSED, CHANNEL_ID::ETC_OBJ, 0.7f);

	return S_OK;
}

void CDynamic_AutoDoor::Check_Progress(_double fTimeDelta)
{
}

HRESULT CDynamic_AutoDoor::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LV_STATIC, ShaderNames[SHADER_STATIC],
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

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
		/* LEFT_DOOR */
		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Prop_Door_A_L"),
			TEXT("Com_Model_LEFT_DOOR"), (CComponent**)&m_pModelComs[CDynamic_AutoDoor::MODEL_TYPE::LEFT_DOOR])))
			return E_FAIL;


		/* RIGHT_DOOR */
		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Model_Prop_Door_A_R"),
			TEXT("Com_Model_RIGHT_DOOR"), (CComponent**)&m_pModelComs[CDynamic_AutoDoor::MODEL_TYPE::RIGHT_DOOR])))
			return E_FAIL;
	}

	/* Com_Collider */
	{
		CCollider::COLLIDERDESC	ColliderDesc(Vec3{ 0.f, m_fColliderSize * 0.7f, 0.f }, m_fColliderSize);

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			ComponentNames[COM_COLLIDER_SPHERE], (CComponent**)&m_pCollider, &ColliderDesc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CDynamic_AutoDoor::Bind_ShaderResources()
{
	/*if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;*/

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::STATE_VIEW)))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::STATE_PROJ)))
		return E_FAIL;

	return S_OK;
}

CDynamic_AutoDoor* CDynamic_AutoDoor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDynamic_AutoDoor* pInstance = new CDynamic_AutoDoor(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDynamic_AutoDoor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CDynamic_AutoDoor* CDynamic_AutoDoor::Clone(void* pArg)
{
	CDynamic_AutoDoor* pInstance = new CDynamic_AutoDoor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDynamic_AutoDoor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDynamic_AutoDoor::Free()
{
	__super::Free();
}
