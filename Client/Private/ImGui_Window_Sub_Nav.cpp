#ifdef _DEBUG
#include "../Default/stdafx.h"

#include "ImGui_Window_Sub_Nav.h"
#include "GameObject.h"
#include "Util_String.h"
#include "Util_File.h"

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
		{
			ImGui::DragFloat("Max Anlge", &m_fMaxSlope, 0.5f);

			ImGui::DragFloat("Min Area", &m_fMinArea, 0.05f);

			_float fRenderRange = CNavMesh::GetInstance()->Get_RenderRange();
			if (ImGui::DragFloat("Render Range", &fRenderRange, 1.f))
			{
				CNavMesh::GetInstance()->Set_RenderRange(fRenderRange);
			}

		}

		/* Button */
		{
			if (ImGui::Button("Clear"))
			{
				m_bPopUp_Clear = TRUE;
			}
			ImGui::SameLine();

			if (ImGui::Button("Auto Bake"))
			{
				if (FAILED(Bake()))
					return;
			}
			ImGui::SameLine();

			if (ImGui::Button("Render"))
			{
				CNavMesh::GetInstance()->Set_Render();
			}
			ImGui::SameLine();

			if (ImGui::Button("Save"))
			{
				m_bPopUp_Save = TRUE;
			}
			ImGui::SameLine();
		}

		/* Text */
		{
			ImGui::Text("Cells : %d", CNavMesh::GetInstance()->Get_CountCells());
		}

		/* Render */
		if (CNavMesh::GetInstance()->Is_Render())
			CNavMesh::GetInstance()->Render();

		/* PopUp */

		if (m_bPopUp_Clear)
			Render_PopUp_Clear();
		if (m_bPopUp_Save)
			Render_PopUp_Save();
	}
	ImGui::End();

	/* Childe Window */
	Show_ChildWindow();
}

void CImGui_Window_Sub_Nav::Clear_Reference_Data()
{
	Clear();

	m_fMaxSlope = 80.f;
	m_fMinArea = -1.f;

	m_bPopUp_Clear = FALSE;
	m_bPopUp_Save = FALSE;
}

HRESULT CImGui_Window_Sub_Nav::Clear()
{
	if (!CNavMesh::GetInstance()->Is_EmptyCells())
		CNavMesh::GetInstance()->Clear_NavDate();

	return S_OK;
}

HRESULT CImGui_Window_Sub_Nav::Bake()
{
	vector<CCell*> Cells;

	if (!CNavMesh::GetInstance()->Is_EmptyCells())
		return S_OK;

	if (FAILED(Create_Cells(Cells)))
		return E_FAIL;

	if (FAILED(Set_Neighbors(Cells)))
		return E_FAIL;
	
	CNavMesh::GetInstance()->Set_NavDate(Cells);
	return S_OK;
}

HRESULT CImGui_Window_Sub_Nav::Save_NavData()
{
	
	return S_OK;
}


HRESULT CImGui_Window_Sub_Nav::Create_Cells(vector<CCell*>& Cells)
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

				/* 경사도 필터링 */
				{
					Vec3 vPolygonNormal = VerticesNor[Indices[i]._0] + VerticesNor[Indices[i]._1] + VerticesNor[Indices[i]._2];
					vPolygonNormal.Normalize();

					_float fTheta = acos(vPolygonNormal.Dot(Vec3::Up));

					if (m_fMaxSlope <= RAD2DEG(fTheta))
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
					Cells.push_back(pCell);
			}
		}
	}
	return S_OK;
}

HRESULT CImGui_Window_Sub_Nav::Set_Neighbors(vector<CCell*>& Cells)

{	/* 네비게이션을 구성하는 각각의 셀들의 이웃을 설정한다. */

	for (size_t i = 0; i < Cells.size() - 1; i++)
	{
		for (size_t j = i + 1; j < Cells.size(); j++)
		{
			if (true == Cells[j]->Compare_Points(Cells[i]->Get_Point(CCell::POINT_A), Cells[i]->Get_Point(CCell::POINT_B)))
			{
				Cells[i]->Set_Neighbor(CCell::LINE_AB, Cells[j]);
			}

			else if (true == Cells[j]->Compare_Points(Cells[i]->Get_Point(CCell::POINT_B), Cells[i]->Get_Point(CCell::POINT_C)))
			{
				Cells[i]->Set_Neighbor(CCell::LINE_BC, Cells[j]);
			}

			else if (true == Cells[j]->Compare_Points(Cells[i]->Get_Point(CCell::POINT_C), Cells[i]->Get_Point(CCell::POINT_A)))
			{
				Cells[i]->Set_Neighbor(CCell::LINE_CA, Cells[j]);
			}
		}
	}
	return S_OK;
}

void CImGui_Window_Sub_Nav::Render_PopUp_Clear()
{
	ImGui::OpenPopup("PopUp");
	if (ImGui::BeginPopup("PopUp"))
	{
		ImGui::Text("Are you Sure?");
		ImGui::SameLine();

		if (ImGui::Button("OK"))
		{
			m_bPopUp_Clear = FALSE;
			Clear();
		}
		ImGui::EndPopup();
	}
}

void CImGui_Window_Sub_Nav::Render_PopUp_Save()
{
	ImGui::OpenPopup("PopUp");
	if (ImGui::BeginPopup("PopUp"))
	{
		ImGui::Text("Are you Sure?");
		ImGui::SameLine();

		if (ImGui::Button("OK"))
		{
			m_bPopUp_Save = FALSE;
			Save_NavData();
		}
		ImGui::EndPopup();
	}
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
}

#endif // _DEBUG