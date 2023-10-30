#ifdef _DEBUG
#include "../Default/stdafx.h"

#include "ImGui_Window_Mid_Nav.h"
#include "GameObject.h"
#include "NavMesh.h"
#include "Util_String.h"
#include "Util_File.h"

#include "Cell.h"

#include "DebugDraw.h"


CImGui_Window_Mid_Nav::CImGui_Window_Mid_Nav()
{
}

HRESULT CImGui_Window_Mid_Nav::Initialize()
{
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pImGui_Manager->m_pContext);
	m_pEffect = new BasicEffect(m_pImGui_Manager->m_pDevice);

	m_pEffect->SetVertexColorEnabled(true);

	const void*		pShaderByteCodes = nullptr;
	size_t			iLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCodes, &iLength);

	if (FAILED(m_pImGui_Manager->m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCodes, iLength, &m_pInputLayout)))
		return E_FAIL;

	_float	fRadius = 0.5f;
	Vec3	vOrigin = { 0.f, fRadius, 0.f };
	m_Shperes[CCell::POINT_A] = new BoundingSphere(vOrigin, fRadius);
	m_Shperes[CCell::POINT_B] = new BoundingSphere(vOrigin, fRadius);
	m_Shperes[CCell::POINT_C] = new BoundingSphere(vOrigin, fRadius);
	return S_OK;
}

void CImGui_Window_Mid_Nav::Show_Window()
{
	ImGuiWindowFlags window_flags = 0;
	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;

	if (ImGui::Begin(m_pImGui_Manager->str_MainWindowType[m_pImGui_Manager->WINDOW_MAIN_NAV], NULL, window_flags))
	{
		/* Input */
		{
			ImGui::DragFloat("Max Anlge", &m_fMaxSlope, 0.5f);

			ImGui::DragFloat("Min Area", &m_fMinArea, 0.05f);

			_float fRenderRange = CNavMesh::GetInstance()->Get_RenderRange();

			if (ImGui::DragFloat("Render Range", &fRenderRange, 1.f))
				CNavMesh::GetInstance()->Set_RenderRange(fRenderRange);
		}

		/* Button */
		{
			ImGui::SeparatorText("Auto");
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
				if (ImGui::Button("Load"))
				{
					m_bPopUp_Load = TRUE;
				}
			}

			ImGui::SeparatorText("Custom");
			{
				if (ImGui::Checkbox("Edit", &m_bEditing))
				{

				}
				ImGui::SameLine();
				if (ImGui::Button("Relate"))
				{
					if (FAILED(Set_Neighbors()))
						assert(FALSE);
				}
				ImGui::SameLine();
				if (ImGui::Button("Del Cell"))
				{
					if (FAILED(Delete_Cell()))
						assert(FALSE);
				}
				ImGui::SameLine();
				if (ImGui::Button("Del Point"))
				{
					if (FAILED(Delete_Point()))
						assert(FALSE);
				}
				ImGui::SameLine();
				ImGui::Text("Cells : %d", CNavMesh::GetInstance()->Get_CountCells());
			}


			ImGui::SeparatorText("Points");
			{
				if (!m_CellPointsCache.empty())
				{
					if (ImGui::Checkbox("Auto Height", &m_bAutoCellHeight))
					{

					}

					ImGui::PushItemWidth(100.f);
					{
						// x
						_float fX = m_CellPointsCache.back().x;
						if (ImGui::InputFloat("x", &fX, 0.01f, 0.1f, "%.3f"))
						{
							m_CellPointsCache.back().x = fX;
						}
						ImGui::SameLine();

						// y 
						_float fY = m_CellPointsCache.back().y;
						if (ImGui::InputFloat("y", &fY, 0.01f, 0.1f, "%.3f"))
						{
							m_CellPointsCache.back().y = fY;
						}
						ImGui::SameLine();

						// z
						_float fZ = m_CellPointsCache.back().z;
						if (ImGui::InputFloat("z", &fZ, 0.01f, 0.1f, "%.3f"))
						{
							m_CellPointsCache.back().z = fZ;
						}
					}
					ImGui::PopItemWidth();
				}
			}
		}

		/* PopUp */
		{
			if (m_bPopUp_Clear)
				Render_PopUp_Clear();
			if (m_bPopUp_Save)
				Render_PopUp_Save();
			if (m_bPopUp_Load)
				Render_PopUp_Load();
		}

		/* Update */
		if (m_bEditing)
			Edit_Update();


		/* Render */
		if (CNavMesh::GetInstance()->Is_Render())
			CNavMesh::GetInstance()->Render();

		Render_Sphere();

	}
	ImGui::End();

	/* Childe Window */
	Show_ChildWindow();
}

void CImGui_Window_Mid_Nav::Clear_Reference_Data()
{
	Clear();

	m_fMaxSlope = 80.f;
	m_fMinArea = -1.f;

	m_bPopUp_Clear = FALSE;
	m_bPopUp_Save = FALSE;
}

HRESULT CImGui_Window_Mid_Nav::Clear()
{
	if (!CNavMesh::GetInstance()->Is_EmptyCells())
		CNavMesh::GetInstance()->Clear_NavDate();

	m_CellPicked.clear();
	m_CellPointsCache.clear();

	CNavMesh::GetInstance()->Set_Render(FALSE);
	CNavMesh::GetInstance()->Set_RenderRange(10.f);

	return S_OK;
}

HRESULT CImGui_Window_Mid_Nav::Bake()
{
	vector<CCell*> Cells;

	if (!CNavMesh::GetInstance()->Is_EmptyCells())
		return S_OK;

	if (FAILED(Create_Cells(Cells)))
		return E_FAIL;

	if (FAILED(CNavMesh::GetInstance()->Set_NavDate(Cells)))
		return E_FAIL;

	if (FAILED(Set_Neighbors()))
		return E_FAIL;

	return S_OK;
}

HRESULT CImGui_Window_Mid_Nav::Save_NavData()
{
	if (FAILED(CNavMesh::GetInstance()->Save_NavData(NavPaths[m_pImGui_Manager->m_iIndex_CurLevelID])))
		return E_FAIL;

	return S_OK;
}

HRESULT CImGui_Window_Mid_Nav::Load_NavData()
{
	if (FAILED(CNavMesh::GetInstance()->Load_NavData(NavPaths[m_pImGui_Manager->m_iIndex_CurLevelID])))
		return E_FAIL;

	return S_OK;
}

void CImGui_Window_Mid_Nav::Edit_Update()
{
	/* Create Cell */
	if (GAME_INSTNACE->Key_Down(VK_RBUTTON))
	{
		/* 메시 피킹 위치 + 스냅 X*/
		RAYHIT_DESC hit = GAME_INSTNACE->Check_ScreenRay(LayerNames[LAYER_ENV_STATIC], FALSE);

		if (nullptr != hit.pGameObject)
		{
			Vec3 vPickedPos = hit.vHitPoint;

			/* 셀들과 스냅 체크*/
			CNavMesh::GetInstance()->Get_SnapCellPos(vPickedPos);

			if (FAILED(Create_Cell(vPickedPos)))
			{
				
			}
		}
	}

	/* Picked Cell */
	if (GAME_INSTNACE->Key_Pressing(VK_MBUTTON))
	{
		CCell* pPickedCell = GAME_INSTNACE->Check_ScreenRay();

		if (nullptr != pPickedCell)
		{
			if (FAILED(Pick_Cell(pPickedCell)))
				assert(FALSE);
		}
	}
}

HRESULT CImGui_Window_Mid_Nav::Create_Cell(Vec3 vPoint)
{	
	if (CCell::POINT_END <= m_CellPointsCache.size())
		m_CellPointsCache.clear();

	/* 이전 포인트의 y로 자동 세팅 */
	if (m_bAutoCellHeight && !m_CellPointsCache.empty())
	{
		vPoint.y = m_CellPointsCache.back().y;
	}
	m_CellPointsCache.push_back(vPoint);

	if (CCell::POINT_END <= m_CellPointsCache.size())
	{

		/* 시계 방향 정렬 */
		Vec3 vLineAB = Vec3(m_CellPointsCache[CCell::POINT_B] - m_CellPointsCache[CCell::POINT_A]).Normalized();
		Vec3 vLineAC = Vec3(m_CellPointsCache[CCell::POINT_C] - m_CellPointsCache[CCell::POINT_A]).Normalized();

		Vec3 vCross = vLineAB.Cross(vLineAC);

		/*if (0.f > vCross.y)
		{
			Vec3 vTemp = m_CellPointsCache[CCell::POINT_B];
			m_CellPointsCache[CCell::POINT_B] = m_CellPointsCache[CCell::POINT_C];
			m_CellPointsCache[CCell::POINT_C] = vTemp;
		}*/



		/* 같은 점이 있는지 필터링 */
		if (m_CellPointsCache[CCell::POINT_A] != m_CellPointsCache[CCell::POINT_B] && m_CellPointsCache[CCell::POINT_B] != m_CellPointsCache[CCell::POINT_C] && m_CellPointsCache[CCell::POINT_C] != m_CellPointsCache[CCell::POINT_A])
		{
			// 셀 생성 
			Vec3 Points[CCell::POINT_END] = { m_CellPointsCache[CCell::POINT_A], m_CellPointsCache[CCell::POINT_B], m_CellPointsCache[CCell::POINT_C] };

			if (FAILED(CNavMesh::GetInstance()->Add_Cell(Points)))
			{
				m_CellPointsCache.pop_back();
				return E_FAIL;
			}
		}
	}

	return S_OK;
}

HRESULT CImGui_Window_Mid_Nav::Delete_Cell()
{
	for (auto& pCell : m_CellPicked)
	{
		if (nullptr != pCell)
		{
			if (FAILED(CNavMesh::GetInstance()->Delete_Cell(pCell->Get_Index())))
				return E_FAIL;
		}
	}

	m_CellPicked.clear();

	return S_OK;
}

HRESULT CImGui_Window_Mid_Nav::Delete_Point()
{
	if (!m_CellPointsCache.empty())
		m_CellPointsCache.pop_back();

	return S_OK;
}

HRESULT CImGui_Window_Mid_Nav::Pick_Cell(CCell* pPickedCell)
{
	if (nullptr == pPickedCell)
		return E_FAIL;

	for (auto& pCell : m_CellPicked)
	{
		if (pCell == pPickedCell)
			return S_OK;
	}

	m_CellPicked.push_back(pPickedCell);

	return S_OK;
}

HRESULT CImGui_Window_Mid_Nav::Create_Cells(vector<CCell*>& Cells)
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
				Vec3 VerticesPosWorld[CCell::POINT_END];

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
				CCell* pCell = CCell::Create(m_pImGui_Manager->m_pDevice, m_pImGui_Manager->m_pContext, VerticesPosWorld, (_uint)Cells.size());

				if (nullptr != pCell)
					Cells.push_back(pCell);
			}
		}
	}
	return S_OK;
}

HRESULT CImGui_Window_Mid_Nav::Set_Neighbors()
{	
	/* 모든 셀의 현재 인덱스 및 이웃 인덱스 초기화, 네비게이션을 구성하는 각각의 셀들의 이웃을 설정한다. */
	CNavMesh::GetInstance()->Set_Neighbors();

	/* 네비이션을 사용하는 오브젝트들의 현재 위치한 셀의 인덱스를 리셋하여 다시 적용한다. */
	list<CGameObject*>* pLayers[3];

	pLayers[0] = GAME_INSTNACE->Get_Layer(m_pImGui_Manager->m_iIndex_CurLevelID, LayerNames[LAYER_PLAYER]);
	pLayers[1] = GAME_INSTNACE->Get_Layer(m_pImGui_Manager->m_iIndex_CurLevelID, LayerNames[LAYER_ENEMY]);
	pLayers[2] = GAME_INSTNACE->Get_Layer(m_pImGui_Manager->m_iIndex_CurLevelID, LayerNames[LAYER_NPC]);


	for (size_t i = 0; i < 3; i++)
	{
		if (nullptr == pLayers[i]) continue;
		
		for (auto& pCharacter : *pLayers[i])
		{
			if (nullptr == pCharacter) continue;

			CNavMeshAgent* pNavMeshAgent = pCharacter->Get_NavMeshAgent();

			if (nullptr == pNavMeshAgent) continue;

			const _int iIndex = CNavMesh::GetInstance()->Find_Cell(pCharacter->Get_Transform()->Get_FinalPosition().xyz());

			pNavMeshAgent->Set_CurIndex(iIndex);
		}
	}

	return S_OK;
}

void CImGui_Window_Mid_Nav::Render_PopUp_Clear()
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

void CImGui_Window_Mid_Nav::Render_PopUp_Save()
{
	ImGui::OpenPopup("PopUp");
	if (ImGui::BeginPopup("PopUp"))
	{
		ImGui::Text("Did you set Neighbors?");
		ImGui::SameLine();

		if (ImGui::Button("OK"))
		{
			m_bPopUp_Save = FALSE;
			Save_NavData();
		}
		ImGui::EndPopup();
	}
}

void CImGui_Window_Mid_Nav::Render_PopUp_Load()
{
	ImGui::OpenPopup("PopUp");
	if (ImGui::BeginPopup("PopUp"))
	{
		ImGui::Text("Are you Sure?");
		ImGui::SameLine();

		if (ImGui::Button("OK"))
		{
			m_bPopUp_Load = FALSE;
			Load_NavData();
		}
		ImGui::EndPopup();
	}
}

void CImGui_Window_Mid_Nav::Render_Sphere()
{
	_float4 vColor(1.f, 0.f, 1.f, 1.f);

	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(GAME_INSTNACE->Get_Transform(CPipeLine::STATE_VIEW));
	m_pEffect->SetProjection(GAME_INSTNACE->Get_Transform(CPipeLine::STATE_PROJ));

	m_pEffect->Apply(m_pImGui_Manager->m_pContext);

	m_pImGui_Manager->m_pContext->IASetInputLayout(m_pInputLayout);

	m_pBatch->Begin();

	for (size_t i = 0; i < m_CellPointsCache.size(); i++)
	{
		m_Shperes[i]->Center = m_CellPointsCache[i];

		DX::Draw(m_pBatch, *m_Shperes[i], XMLoadFloat4(&vColor));
	}

	m_pBatch->End();
}

CImGui_Window_Mid_Nav* CImGui_Window_Mid_Nav::Create()
{
	CImGui_Window_Mid_Nav* pInstance = new CImGui_Window_Mid_Nav();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CImGui_Window_Mid_Nav");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGui_Window_Mid_Nav::Free()
{
	__super::Free();

	for (size_t i = 0; i < 3; i++)
	{
		Safe_Delete(m_Shperes[i]);
	}

	Safe_Delete(m_pBatch);
	Safe_Delete(m_pEffect);
	Safe_Release(m_pInputLayout);
}

#endif // _DEBUG