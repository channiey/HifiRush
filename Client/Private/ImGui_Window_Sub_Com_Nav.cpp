#ifdef _DEBUG
#include "../Default/stdafx.h"

#include "ImGui_Window_Sub_Com_Nav.h"
#include "GameObject.h"
#include "Util_String.h"

#include "Cell.h"

CImGui_Window_Sub_Com_Nav::CImGui_Window_Sub_Com_Nav()
{
}

HRESULT CImGui_Window_Sub_Com_Nav::Initialize()
{
	return S_OK;
}

void CImGui_Window_Sub_Com_Nav::Show_Window()
{
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;

	if (ImGui::Begin(m_pImGui_Manager->str_SubWindowType[m_pImGui_Manager->WINDOW_SUB_COM_NAV], NULL, window_flags))
	{
		/* 피킹으로 굽지 않아도 되는 애들 처리해서, 구울 애들만 선택적으로 굽게 처리 필요 */
		if (ImGui::Button("Auto Bake"))
		{
			if (FAILED(Bake()))
				return;
		}
		ImGui::SameLine();
		if (ImGui::Button("Relate Neighbor"))
		{
			if (FAILED(Relate_Neighbor()))
				return;
		}
		ImGui::SameLine();
		if (ImGui::Button("File Save"))
		{
			if (FAILED(Save()))
				return;
		}

		ImGui::DragFloat("Max Anlge", &m_fMaxAngle, 0.5f);

		ImGui::DragFloat("Min Area", &m_fMinArea, 0.05f);

		ImGui::DragFloat("Render Range", &m_fRenderRange, 1.f);

	}
	ImGui::End();

	/* Childe Window */
	Show_ChildWindow();
}

void CImGui_Window_Sub_Com_Nav::Clear_Reference_Data()
{
}

HRESULT CImGui_Window_Sub_Com_Nav::Bake()
{
	list<class CGameObject*>* pGameObjects = m_pGameInstance->Get_Layer(m_pImGui_Manager->m_iIndex_CurLevelID, LayerNames[LAYER_ENV_STATIC]);

	if (nullptr == pGameObjects)
		return E_FAIL;

	for (auto& pGameObject : *pGameObjects)
	{
		if (nullptr == pGameObject)
			continue;

		CModel* pModel = pGameObject->Get_Model();
		if (nullptr == pModel)
			continue;

		vector<class CMesh*>* pMeshes = pModel->Get_Meshes();
		if (nullptr == pMeshes)
			continue;

		for (auto& pMesh : *pMeshes)
		{
			const _uint&	iPrimitives		= pMesh->Get_NumPrimitives();

			Vec3*			VerticesPos		= pMesh->Get_VerticesPos();
			Vec3*			VerticesNor		= pMesh->Get_VerticesNor();
			FACEINDICES32*	Indices			= pMesh->Get_Indices();

			for (size_t i = 0; i < (size_t)iPrimitives; i++)
			{
				/* 기본 정보 세팅 */
				_float3 Points[CCell::POINT_END];

				Vec3 vPoint_A = Points[CCell::POINT_A] = VerticesPos[Indices[i]._0];
				Vec3 vPoint_B = Points[CCell::POINT_B] = VerticesPos[Indices[i]._1];
				Vec3 vPoint_C = Points[CCell::POINT_C] = VerticesPos[Indices[i]._2];

				/* 각도 필터링 */
				{
					Vec3 vPolygonNormal = VerticesNor[Indices[i]._0] + VerticesNor[Indices[i]._1] + VerticesNor[Indices[i]._2];
					vPolygonNormal.Normalize();

					_float fTheta = acos(vPolygonNormal.Dot(Vec3::Up));
				}

				/* 면적 필터링 */
				{
					double fDist_AB = Vec3::DistanceSquared(vPoint_A, vPoint_B);
					double fDist_BC = Vec3::DistanceSquared(vPoint_B, vPoint_C);
					double fDist_CA = Vec3::DistanceSquared(vPoint_C, vPoint_A);

					double fArea = 4 * fDist_AB * fDist_BC - (fDist_AB + fDist_BC - fDist_CA) * (fDist_AB + fDist_BC - fDist_CA);
					fArea = sqrt(fArea) / 4;
				}

				/* 최종 생성 */
				CCell* pCell = CCell::Create(m_pImGui_Manager->m_pDevice, m_pImGui_Manager->m_pContext, Points, 0);

				if (nullptr != pCell)
					m_Cells.push_back(pCell);
			}
		}
	}
	return S_OK;
}

HRESULT CImGui_Window_Sub_Com_Nav::Relate_Neighbor()
{
	return S_OK;
}

HRESULT CImGui_Window_Sub_Com_Nav::Save()
{
	return S_OK;
}

CImGui_Window_Sub_Com_Nav* CImGui_Window_Sub_Com_Nav::Create()
{
	CImGui_Window_Sub_Com_Nav* pInstance = new CImGui_Window_Sub_Com_Nav();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CImGui_Window_Sub_Com_Nav");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGui_Window_Sub_Com_Nav::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);
}

#endif // _DEBUG