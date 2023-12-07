#include "..\Default\stdafx.h"
#include "Npc.h"

#include "EngineInstance.h"
#include "Animation.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CNpc::CNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCharacter(pDevice, pContext)
{
}

CNpc::CNpc(const CNpc& rhs)
	: CCharacter(rhs)
{
}

HRESULT CNpc::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNpc::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CNpc::Tick(_double fTimeDelta)
{
	for (auto& pCollider : m_pColliderComs)
	{
		if (nullptr != pCollider && pCollider->Is_Active())
			pCollider->Update(m_pTransformCom->Get_FinalMat());
	}
}

void CNpc::LateTick(_double fTimeDelta)
{
	if (!CImGui_Manager::GetInstance()->Is_DebugCam())
	{
		if (FAILED(m_pModelCom->Update(fTimeDelta)))
			return;
	}

	for (auto& pCollider : m_pColliderComs)
	{
		if (nullptr != pCollider && pCollider->Is_Active() && CImGui_Manager::GetInstance()->Is_Render_Collider())
			m_pRendererCom->Add_Debug(pCollider);
	}

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RG_SHADOW, this)))
		return;

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
}

HRESULT CNpc::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CNpc::Set_State(const OBJ_STATE& eState)
{
	__super::Set_State(eState);
}


HRESULT CNpc::Ready_Components()
{
	return S_OK;
}

void CNpc::OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild)
{
	CGameObject* pGameObject = pCollider->Get_Owner();

	if (L"Player_Chai_000" == pGameObject->Get_Name())
	{
		if (!m_bSetDialouge)
		{
			m_bSetDialouge = TRUE;
			Set_Dialogue();
		}
	}
}

void CNpc::OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild)
{
}

void CNpc::OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild)
{
}

void CNpc::Free()
{
	__super::Free();

}
