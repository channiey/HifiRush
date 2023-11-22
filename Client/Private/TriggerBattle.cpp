#include "..\Default\stdafx.h"
#include "..\Public\TriggerBattle.h"
#include "EngineInstance.h"

#include "Util_String.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CTriggerBattle::CTriggerBattle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext)
{
}

CTriggerBattle::CTriggerBattle(const CTriggerBattle& rhs)
	: CGameObject(rhs)
{
}

HRESULT CTriggerBattle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTriggerBattle::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Load()))
		return E_FAIL;

	return S_OK;
}

void CTriggerBattle::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);

	/* Update Colliders */
	if (nullptr != m_pCollider && m_pCollider->Is_Active())
		m_pCollider->Update(m_pTransformCom->Get_WorldMat());
}

void CTriggerBattle::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef _DEBUG
	if (nullptr != m_pCollider && m_pCollider->Is_Active() && CImGui_Manager::GetInstance()->Is_Render_Collider())
		m_pRendererCom->Add_Debug(m_pCollider);
#endif // _DEBUG

	if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;
}


HRESULT CTriggerBattle::Render()
{	
	/* Render Collider */
	if (nullptr != m_pCollider && m_pCollider->Is_Active() && CImGui_Manager::GetInstance()->Is_Render_Collider())
	{
		return m_pCollider->Render();
	}

	return S_OK;
}

HRESULT CTriggerBattle::Add_Clone(const _uint iFlowLevel, CGameObject* pObject)
{
	if (nullptr != Find_Object(pObject))
		return E_FAIL;

	auto Pair = m_CacheFlows.find(iFlowLevel);

	if (Pair == m_CacheFlows.end())
	{
		list<CGameObject*> list;

		list.push_back(pObject);

		m_CacheFlows.emplace(iFlowLevel, list);
	}
	else
	{
		Pair->second.push_back(pObject);
	}

	return S_OK;
}

HRESULT CTriggerBattle::Clear_Clone()
{
	m_CacheFlows.clear();

	return S_OK;
}

HRESULT CTriggerBattle::Set_TriggerData()
{
	/* 캐시 데이터 */
	for (auto Pair : m_CacheFlows)
	{
		list<CLONE_DESC> list;

		for (auto pObejct : Pair.second)
		{
			const _int iIndex = CNavMesh::GetInstance()->Find_Cell(pObejct->Get_Transform()->Get_FinalPosition().xyz());

			CLONE_DESC desc;
			{
				desc.iFlowLevel = Pair.first;
				desc.strPrototypeTag = Util_String::Remove_LastNumChar(pObejct->Get_Name(), CLONE_PIN_MAX_DIGIT);
				desc.iCellIndex = iIndex;
				desc.matWorld = pObejct->Get_Transform()->Get_WorldMat();
			}
			list.push_back(desc);  
		}
		if(!list.empty())
			m_Flows.emplace(Pair.first, list);
	}

	return S_OK;
}

HRESULT CTriggerBattle::Clear_TriggerData()
{
	m_Flows.clear();

	return S_OK;
}

HRESULT CTriggerBattle::Save()
{
	if (FAILED(Set_TriggerData()))
		return E_FAIL;

	wstring strSaveFilePath = TriggerFilePath + LevelNames[ENGINE_INSTANCE->Get_CurLevelIndex()] + L"_" + m_strTriggerTag + L".bin";
	
	shared_ptr<Util_File> file = make_shared<Util_File>();
	file->Open(strSaveFilePath, FileMode::Write);

	file->Write<size_t>(m_Flows.size());
	for (auto Pair : m_Flows)
	{
		file->Write<_uint>(Pair.first);
		file->Write<size_t>(Pair.second.size());
		for (auto pClone: Pair.second)
		{
			file->Write<_uint>(pClone.iFlowLevel);
			file->Write<string>(Util_String::ToString(pClone.strPrototypeTag));
			file->Write<_uint>(pClone.iCellIndex);
			file->Write<Matrix>(pClone.matWorld);
		}
	}

	return S_OK;
}

HRESULT CTriggerBattle::Load()
{
	wstring strLoadFilePath = TriggerFilePath + LevelNames[ENGINE_INSTANCE->Get_CurLoadingLevel()] + L"_" + m_strTriggerTag + L".bin";

	if (!Util_File::IsExistFile(Util_String::ToString(strLoadFilePath)))
	{
		MSG_BOX("Nothing Trigger File");
		return E_FAIL;
	}

	shared_ptr<Util_File> file = make_shared<Util_File>();
	file->Open(strLoadFilePath, FileMode::Read);

	const size_t iMapSize = file->Read<size_t>();
	for (size_t i = 0; i < iMapSize; i++)
	{
		list<CLONE_DESC> list;

		const _uint iKey = file->Read<_uint>();
		const size_t iListSize = file->Read<size_t>();
		for (size_t j = 0; j < iListSize; j++)
		{
			CLONE_DESC desc;
			{
				desc.iFlowLevel = file->Read<_uint>();
				desc.strPrototypeTag = Util_String::ToWString(file->Read<string>());
				desc.iCellIndex = file->Read<_uint>();
				desc.matWorld = file->Read<Matrix>();
			}
			list.push_back(desc);
		}

		if (!list.empty())
			m_Flows.emplace(iKey, list);
	}

	return S_OK;
}

HRESULT CTriggerBattle::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Transform"),
		ComponentNames[COM_TRANSFORM], (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Renderer"),
		ComponentNames[COM_RENDERER], (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Collider */
	CCollider_Sphere* pCollider = nullptr;
	{
		CCollider::COLLIDERDESC	ColliderDesc(Vec3::Zero, 10.f);

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
			ComponentNames[COM_COLLIDER_SPHERE], (CComponent**)&m_pCollider, &ColliderDesc)))
			return E_FAIL;
	}

	return S_OK;
}

const _bool CTriggerBattle::Is_Finish_Battle()
{
	if (nullptr == ENGINE_INSTANCE->Get_GameObject_InCurLevel_InLayerFirst(LayerNames[LAYER_ID::LAYER_ENEMY]))
		return TRUE;
	
	return FALSE;
}

CGameObject* CTriggerBattle::Find_Object(CGameObject* pObject)
{

	return nullptr;
}

void CTriggerBattle::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pCollider);
	Safe_Release(m_pRendererCom);
}
