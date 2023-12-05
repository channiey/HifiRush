#include "..\Default\stdafx.h"
#include "Character.h"
#include "EngineInstance.h"

#include "Animation.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG


CCharacter::CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CCharacter::CCharacter(const CCharacter& rhs)
	: CGameObject(rhs)
	, m_tStatDesc(rhs.m_tStatDesc)
	, m_tFightDesc(rhs.m_tFightDesc)
	, m_tPhysicsDesc(rhs.m_tPhysicsDesc)
{
}

HRESULT CCharacter::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCharacter::Initialize(void* pArg)
{
	if(FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CCharacter::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!CImGui_Manager::GetInstance()->Is_DebugCam())
	{
		if (nullptr != m_pStateMachineCom)
		{
			if (FAILED(m_pStateMachineCom->Tick(fTimeDelta)))
				return;
		}

		if (nullptr != m_pRigidbodyCom)
			m_pRigidbodyCom->Tick(fTimeDelta);
	}

	for (auto& pCollider : m_pColliderComs)
	{
		if (nullptr != pCollider && pCollider->Is_Active())
			pCollider->Update(m_pTransformCom->Get_FinalMat());
	}
}

void CCharacter::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	
	if (!CImGui_Manager::GetInstance()->Is_DebugCam())
	{
		if (FAILED(m_pModelCom->Update(fTimeDelta)))
			return;

		if (nullptr != m_pStateMachineCom)
		{
			if (FAILED(m_pStateMachineCom->LateTick(fTimeDelta)))
				return;
		}
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

HRESULT CCharacter::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	const _uint iNumMeshes = m_pModelCom->Get_MeshCount();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
			return E_FAIL;
	}

	/* Render Collider */
	for (auto pCollider : m_pColliderComs)
	{
		if (nullptr != pCollider && pCollider->Is_Active() && CImGui_Manager::GetInstance()->Is_Render_Collider())
		{
			pCollider->Render();
		}
	}

	/* Render NavMeshAgent */
	if (nullptr != m_pNavMeshAgentCom)
	{
		if (FAILED(m_pNavMeshAgentCom->Render()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CCharacter::Render_LightDepth()
{
	Matrix matViewShadow = ENGINE_INSTANCE->Get_ShadowLight_MatView();

	Matrix matProjShadow = ENGINE_INSTANCE->Get_ShadowLight_MatProj();

	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &matViewShadow)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &matProjShadow)))
		return E_FAIL;

	const _uint iNumMeshes = m_pModelCom->Get_MeshCount();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 1)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CCharacter::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LV_STATIC, ShaderNames[SHADER_VTF],
		ComponentNames[COM_SHADER], (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Renderer"),
		ComponentNames[COM_RENDERER], (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"),
		ComponentNames[COM_TRANSFORM], (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* Com_StateMachine */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_StateMachine"),
		ComponentNames[COM_STATEMACHINE], (CComponent**)&m_pStateMachineCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCharacter::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_ViewMatrix", CPipeLine::STATE_VIEW)))
		return E_FAIL;

	if (FAILED(ENGINE_INSTANCE->Bind_TransformToShader(m_pShaderCom, "g_ProjMatrix", CPipeLine::STATE_PROJ)))
		return E_FAIL;

	return S_OK;
}

void CCharacter::Attack(CCharacter* pCharacter)
{
	
}

void CCharacter::Damaged(CCharacter* pCharacter, const ATK_TYPE& eAtkType)
{
	if (nullptr == pCharacter || m_tStatDesc.bDead)
		return;

	/* 패링 여부 검사 */
	if (m_tFightDesc.bParry)
	{
		pCharacter->Damaged(this);
		return;
	}

	/* 상대방 및 공격 타입 검사*/
	m_tFightDesc.pAttacker = pCharacter;
	m_tFightDesc.eAtkType = eAtkType;

	/* 체력 및 사망 여부 검사 */
	m_tFightDesc.bDamaged = TRUE;

	m_tStatDesc.fCurHp -= pCharacter->Get_StatDesc().fAd;
	
	if (m_tStatDesc.fCurHp <= 0)
		m_tStatDesc.bDead = TRUE;
}

void CCharacter::Die()
{
}

void CCharacter::KnockBack(CCharacter* pAttacker)
{
	if (nullptr == pAttacker)
		return;

	Vec3 vDir = m_pTransformCom->Get_FinalPosition().xyz() - pAttacker->Get_Transform()->Get_FinalPosition().xyz();
	vDir.y = 0.f;

	m_pRigidbodyCom->Add_Force(vDir.Normalized() * pAttacker->Get_PhysicsDesc().fKnockBackPower, CRigidbody::FORCE_MODE::IMPULSE);
}

void CCharacter::Reset_Desc()
{
	m_tStatDesc = STAT_DESC{};
	m_tFightDesc = FIGHT_DESC{};
	m_tPhysicsDesc = PHYSICS_DESC{};
}

void CCharacter::Debug_Animation()
{
	if (nullptr == m_pModelCom)
		return;

	CModel::TweenDesc desc = m_pModelCom->Get_TweenDesc();

	CAnimation* pCurAnim = m_pModelCom->Get_Animation(desc.cur.iAnimIndex);
	CAnimation* pNextAnim = m_pModelCom->Get_Animation(desc.next.iAnimIndex);

	string strCur = "NONE", strNext = "NONE";

	if (nullptr != pCurAnim)
		strCur = pCurAnim->Get_Name();

	if (nullptr != pNextAnim)
		strNext = pNextAnim->Get_Name();

	cout << strCur << "\t"
		<< strNext << "\t"
		<< desc.cur.iCurFrame << "\t"
		<< desc.next.iCurFrame << endl;
}



void CCharacter::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pNavMeshAgentCom);

	if(nullptr != m_pStateMachineCom)
		Safe_Release(m_pStateMachineCom);

	if (nullptr != m_pRigidbodyCom)
		Safe_Release(m_pRigidbodyCom);

	for (auto& pCollider : m_pColliderComs)
		Safe_Release(pCollider);
}
