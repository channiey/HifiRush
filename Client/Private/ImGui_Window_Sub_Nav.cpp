#ifdef _DEBUG
#include "../Default/stdafx.h"

#include "ImGui_Window_Sub_Nav.h"
#include "GameObject.h"
#include "Util_String.h"

#include "Cell.h"

CImGui_Window_Sub_Nav::CImGui_Window_Sub_Nav()
{
}

HRESULT CImGui_Window_Sub_Nav::Initialize()
{
	return S_OK;
}

void CImGui_Window_Sub_Nav::Show_Window()
{
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;

	if (ImGui::Begin(m_pImGui_Manager->str_SubWindowType[m_pImGui_Manager->WINDOW_SUB_NAV], NULL, window_flags))
	{
		/* Input */
		ImGui::DragFloat("Max Anlge", &m_fMaxAngle, 0.5f);

		ImGui::DragFloat("Min Area", &m_fMinArea, 0.05f);

		ImGui::DragFloat("Render Range", &m_fRenderRange, 1.f);

		/* Button */
		if (ImGui::Button("Auto Bake"))
		{
			if (FAILED(Bake()))
				return;
		}
		ImGui::SameLine();
		if (ImGui::Button("Render"))
		{
			m_bRender = !m_bRender;
		}
		ImGui::SameLine();
		if (ImGui::Button("Save"))
		{
			if (FAILED(Save()))
				return;
		}
		ImGui::SameLine();


		/* Text */
		ImGui::Text("Cells : %d", m_Cells.size());

		/* Render */
		if (!m_Cells.empty()&& m_bRender)
			Render();		
	}
	ImGui::End();

	/* Childe Window */
	Show_ChildWindow();
}

void CImGui_Window_Sub_Nav::Clear_Reference_Data()
{
}

HRESULT CImGui_Window_Sub_Nav::Bake()
{

	if (!m_Cells.empty())
	{
		for (auto& pCell : m_Cells)
			Safe_Release(pCell);

		m_Cells.clear();
		m_Cells.shrink_to_fit();
	}


	if (FAILED(Create_Cells()))
		return E_FAIL;

	if (FAILED(Set_Neighbors()))
		return E_FAIL;
	
	if (nullptr == m_pShader)
	{
		m_pShader = CShader::Create(m_pImGui_Manager->m_pDevice, m_pImGui_Manager->m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
		if (nullptr == m_pShader)
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CImGui_Window_Sub_Nav::Save()
{
	if (ImGui::Begin("Save", NULL))
	{
		
		if (ImGui::Button("Save"))
		{
			
		}

	}
	ImGui::End();

	return S_OK;
}

HRESULT CImGui_Window_Sub_Nav::Render()
{

	/* WVP 세팅 */
	{
		Matrix matWorld;

		if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &matWorld)))
			return E_FAIL;

		_float4x4 matView = GAME_INSTNACE->Get_Transform(CPipeLine::STATE_VIEW);
		_float4x4 matProj = GAME_INSTNACE->Get_Transform(CPipeLine::STATE_PROJ);

		if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &matView)))
			return E_FAIL;

		if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &matProj)))
			return E_FAIL;
	}


	/* Draw Cells */
	{
		_float4 vColor{ 0.f, 1.f, 0.f, 1.f };
		if (FAILED(m_pShader->Bind_RawValue("g_vLineColor", &vColor, sizeof(_float4))))
			return E_FAIL;

		_float		fHeight = 0.f;
		if (FAILED(m_pShader->Bind_RawValue("g_fHeight", &fHeight, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShader->Begin(0)))
			return E_FAIL;

		for (auto& pCell : m_Cells)
		{
			if (nullptr != pCell)
			{
				_float fDist = Vec3(pCell->Get_CenterPoint() - GAME_INSTNACE->Get_CamPosition().xyz()).Length();
				if (m_fRenderRange < fDist)
					continue;
				pCell->Render();
			}
		}
	}

	return S_OK;
}

HRESULT CImGui_Window_Sub_Nav::Create_Cells()
{
	list<class CGameObject*>* pGameObjects = m_pGameInstance->Get_Layer(m_pImGui_Manager->m_iIndex_CurLevelID, LayerNames[LAYER_ENV_STATIC]);

	if (nullptr == pGameObjects)
		return E_FAIL;

	for (auto& pGameObject : *pGameObjects)
	{
		if (nullptr == pGameObject) continue;

		CModel* pModel = pGameObject->Get_Model();
		if (nullptr == pModel) continue;

		vector<class CMesh*>* pMeshes = pModel->Get_Meshes();
		if (nullptr == pMeshes) continue;

		Matrix matWorld = pGameObject->Get_Transform()->Get_WorldMat();

		for (auto& pMesh : *pMeshes)
		{
			const _uint& iPrimitives = pMesh->Get_NumPrimitives(); // 폴리곤 갯수

			Vec3* VerticesPos = pMesh->Get_VerticesPos();
			Vec3* VerticesNor = pMesh->Get_VerticesNor();
			FACEINDICES32* Indices = pMesh->Get_Indices();

			for (size_t i = 0; i < (size_t)iPrimitives; i++)
			{
				/* 기본 정보 세팅 */
				_float3 Points[CCell::POINT_END];


				/* 로컬에서 월드로 올린다. */
				Vec3 VerticesPosWorld[3];

				XMStoreFloat3(&VerticesPosWorld[0],
					XMVector3TransformCoord(XMLoadFloat3(&VerticesPos[Indices[i]._0]), matWorld));
				XMStoreFloat3(&VerticesPosWorld[1],
					XMVector3TransformCoord(XMLoadFloat3(&VerticesPos[Indices[i]._1]), matWorld));
				XMStoreFloat3(&VerticesPosWorld[2],
					XMVector3TransformCoord(XMLoadFloat3(&VerticesPos[Indices[i]._2]), matWorld));

				Vec3 vPoint_A = Points[CCell::POINT_A] = VerticesPosWorld[0];
				Vec3 vPoint_B = Points[CCell::POINT_B] = VerticesPosWorld[1];
				Vec3 vPoint_C = Points[CCell::POINT_C] = VerticesPosWorld[2];

				/* 각도 필터링 */
				{
					Vec3 vPolygonNormal = VerticesNor[Indices[i]._0] + VerticesNor[Indices[i]._1] + VerticesNor[Indices[i]._2];
					vPolygonNormal.Normalize();

					_float fTheta = acos(vPolygonNormal.Dot(Vec3::Up));

					if (m_fMaxAngle <= RAD2DEG(fTheta))
						continue;
				}

				/* 면적 필터링 */
				{
					double fDist_AB = Vec3::DistanceSquared(vPoint_A, vPoint_B);
					double fDist_BC = Vec3::DistanceSquared(vPoint_B, vPoint_C);
					double fDist_CA = Vec3::DistanceSquared(vPoint_C, vPoint_A);

					double fArea = 4 * fDist_AB * fDist_BC - (fDist_AB + fDist_BC - fDist_CA) * (fDist_AB + fDist_BC - fDist_CA);
					fArea = sqrt(fArea) / 4;

					if (fArea <= m_fMinArea)
						continue;
				}

				/* 최종 생성 */
				CCell* pCell = CCell::Create(m_pImGui_Manager->m_pDevice, m_pImGui_Manager->m_pContext, VerticesPosWorld, 0);

				if (nullptr != pCell)
					m_Cells.push_back(pCell);
			}
		}
	}
	return S_OK;
}

HRESULT CImGui_Window_Sub_Nav::Set_Neighbors()

{	/* 네비게이션을 구성하는 각각의 셀들의 이웃을 설정한다. */

	for (size_t i = 0; i < m_Cells.size() - 1; i++)
	{
		for (size_t j = i + 1; j < m_Cells.size(); j++)
		{
			if (true == m_Cells[j]->Compare_Points(m_Cells[i]->Get_Point(CCell::POINT_A), m_Cells[i]->Get_Point(CCell::POINT_B)))
			{
				m_Cells[i]->Set_Neighbor(CCell::LINE_AB, m_Cells[j]);
			}

			else if (true == m_Cells[j]->Compare_Points(m_Cells[i]->Get_Point(CCell::POINT_B), m_Cells[i]->Get_Point(CCell::POINT_C)))
			{
				m_Cells[i]->Set_Neighbor(CCell::LINE_BC, m_Cells[j]);
			}

			else if (true == m_Cells[j]->Compare_Points(m_Cells[i]->Get_Point(CCell::POINT_C), m_Cells[i]->Get_Point(CCell::POINT_A)))
			{
				m_Cells[i]->Set_Neighbor(CCell::LINE_CA, m_Cells[j]);
			}
		}
	}
	return S_OK;
}

CImGui_Window_Sub_Nav* CImGui_Window_Sub_Nav::Create()
{
	CImGui_Window_Sub_Nav* pInstance = new CImGui_Window_Sub_Nav();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CImGui_Window_Sub_Nav");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGui_Window_Sub_Nav::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);

	Safe_Release(m_pShader);
}

#endif // _DEBUG