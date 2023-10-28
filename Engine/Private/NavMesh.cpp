#include "..\Public\NavMesh.h"

#include "GameInstance.h"
#include "Collision_Manager.h"
#include "Cell.h"

IMPLEMENT_SINGLETON(CNavMesh)

CNavMesh::CNavMesh()
{

}

HRESULT CNavMesh::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	m_pShader = CShader::Create(m_pDevice,m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	return TRUE;
}

HRESULT CNavMesh::Render()
{
	if (!m_bRender || nullptr == m_pShader || m_Cells.empty())
		return S_OK;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	/* Color */
	_float4 vColor{ 0.f, 1.f, 0.f, 1.f };
	if (FAILED(m_pShader->Bind_RawValue("g_vLineColor", &vColor, sizeof(_float4))))
		return E_FAIL;

	/* Height */
	_float	fHeight = 0.01f;
	if (FAILED(m_pShader->Bind_RawValue("g_fHeight", &fHeight, sizeof(_float))))
		return E_FAIL;
	
	/* Render */
	if (FAILED(m_pShader->Begin(0)))
		return E_FAIL;
	
	for (auto& pCell : m_Cells)
	{
		if (nullptr != pCell)
		{
			/* Check Range */
			_float fDist = Vec3(pCell->Get_CenterPoint() - GAME_INSTNACE->Get_CamPosition().xyz()).Length();

			if (m_fRenderRange < fDist) continue;

			/* Render */
			pCell->Render();
		}
	}

	return S_OK;
}

HRESULT CNavMesh::Render_Cell(const _int& iInedx)
{
	if (!m_bRender || nullptr == m_pShader || m_Cells.size() <= iInedx || m_Cells.empty())
		return S_OK;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	/* Color */
	_float4 vColor{ 1.f, 0.f, 0.f, 1.f };
	if (FAILED(m_pShader->Bind_RawValue("g_vLineColor", &vColor, sizeof(_float4))))
		return E_FAIL;

	/* Height */
	_float		fHeight = 0.1f;
	if (FAILED(m_pShader->Bind_RawValue("g_fHeight", &fHeight, sizeof(_float))))
		return E_FAIL;

	/* Render */
	if (FAILED(m_pShader->Begin(0)))
		return E_FAIL;

	_float fDist = Vec3(m_Cells[iInedx]->Get_CenterPoint() - GAME_INSTNACE->Get_CamPosition().xyz()).Length();

	if (m_fRenderRange >= fDist)
	{
		m_Cells[iInedx]->Render();
	}

	return S_OK;
}

HRESULT CNavMesh::Render_Picked()
{
	if (!m_bRender || nullptr == m_pShader || m_Cells.empty())
		return S_OK;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	/* Color */
	_float4 vColor{ 0.f, 0.f, 1.f, 1.f };
	if (FAILED(m_pShader->Bind_RawValue("g_vLineColor", &vColor, sizeof(_float4))))
		return E_FAIL;

	/* Height */
	_float		fHeight = 0.1f;
	if (FAILED(m_pShader->Bind_RawValue("g_fHeight", &fHeight, sizeof(_float))))
		return E_FAIL;

	/* Render */
	if (FAILED(m_pShader->Begin(0)))
		return E_FAIL;

	for (auto& pCell : m_Cells)
	{
		if (nullptr != pCell && pCell->Is_Picked())
		{
			/* Check Range */
			_float fDist = Vec3(pCell->Get_CenterPoint() - GAME_INSTNACE->Get_CamPosition().xyz()).Length();

			if (m_fRenderRange < fDist) continue;

			/* Render */
			pCell->Render();
		}
	}
}

HRESULT CNavMesh::Set_Neighbors()
{
	/* 01. 셀 자신의 인덱스 및 이웃의 인덱스까지 초기화 */
	for (size_t i = 0; i < m_Cells.size(); i++)
	{
		m_Cells[i]->Set_Index((_uint)i);
		m_Cells[i]->Clear_NeighborIndices();
	}

	/* 02. 네비게이션을 구성하는 각각 셀들의 이웃을 새로 설정한다. */
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell) continue;

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_AB, pDestCell);
			}
			else if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_BC, pDestCell);
			}

			else if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_CA, pDestCell);
			}
		}
	}

	return S_OK;
}

const _float CNavMesh::Get_AgentHeight(const _int& iIndex, const Vec3& vPos)
{
	if (m_Cells.empty() || iIndex < 0 || nullptr == m_Cells[iIndex])
		return -1.f;

	const _float3* vPoints = m_Cells[iIndex]->Get_Points();

	_vector		vPlane;


	/* How To */
	{
		/* 평면 방정식에 a, b, c, d 다 구했다. */
		/* 내 위치 x, y, z */
		// ax + by + cz + d = 0
		// y = (-ax - cz - d) / b

		/* 평면 상에 존재할 수 있도록하는 y를 구하자. */
		/* 평면상에 존재하면 되는 것이기 때문에 x, z는 변할 이유가 없다. */
	}

	vPlane = XMPlaneFromPoints(XMLoadFloat3(&vPoints[CCell::POINT_A]),
								XMLoadFloat3(&vPoints[CCell::POINT_B]),
								XMLoadFloat3(&vPoints[CCell::POINT_C]));

	_float	fY = ((-XMVectorGetX(vPlane) * XMVectorGetX(vPos) - (XMVectorGetZ(vPlane) * XMVectorGetZ(vPos)) - XMVectorGetW(vPlane))) / XMVectorGetY(vPlane);

	return fY;
}

HRESULT CNavMesh::Set_NavDate(vector<CCell*>& Cells)
{
	if(FAILED(Clear_NavDate()))
		return E_FAIL;

	m_Cells.reserve(Cells.size());
	
	for (auto& pCell : Cells)
		m_Cells.push_back(pCell);

	return TRUE;
}

HRESULT CNavMesh::Save_NavData(const wstring& strFilePath)
{
	shared_ptr<Util_File> file = make_shared<Util_File>();

	if (FAILED(Util_File::CheckOrCreatePath(Util_String::ToString(strFilePath))))
		return E_FAIL;

	file->Open(strFilePath, FileMode::Write);

	file->Write<size_t>(m_Cells.size());

	for (auto& pCell : m_Cells)
	{
		if (nullptr == pCell) continue;

		/* Index */
		file->Write<_uint>(pCell->Get_Index());

		/* Points */
		for (_int i = 0; i < CCell::POINT_END; i++)
			file->Write<_float3>(*(pCell->Get_Point(CCell::POINTS(i))));

		/* Normals */
		for (_int i = 0; i < CCell::LINE_END; i++)
			file->Write<_float3>(*(pCell->Get_Normal(CCell::LINE(i))));

		/* NeighborIndieces */
		for (_int i = 0; i < CCell::LINE_END; i++)
			file->Write<_int>(pCell->Get_NeighborIndex(CCell::LINE(i)));
	}

	return S_OK;
}

HRESULT CNavMesh::Load_NavData(const wstring& strFilePath)
{
	/* Clear Data */
	if (FAILED(Clear_NavDate()))
		return E_FAIL;

	/* Open File */
	shared_ptr<Util_File> file = make_shared<Util_File>();
	if (nullptr == file) return E_FAIL;

	if (!Util_File::IsExistFile(Util_String::ToString(strFilePath)))
	{
		MSG_BOX("Nothing Nav File");
		return E_FAIL;
	}

	file->Open(strFilePath, FileMode::Read);

	/* Pharse Data */
	const size_t countCell = file->Read<size_t>();
	m_Cells.reserve(countCell);

	for (size_t i = 0; i < countCell; i++)
	{
		/* Index */
		_uint iIndex = file->Read<_uint>();

		/* Points */
		_float3 vPoints[CCell::POINT_END];
		for (_int i = 0; i < CCell::POINT_END; i++)
			vPoints[i] = file->Read<_float3>();

		CCell* pCell = CCell::Create(GAME_INSTNACE->Get_Device(), GAME_INSTNACE->Get_Context(), vPoints, iIndex);
		if (nullptr == pCell)
			return E_FAIL;

		/* Normals */
		for (_int i = 0; i < CCell::LINE_END; i++)
			pCell->Set_Normal((CCell::LINE)i, file->Read<_float3>());

		/* NeighborIndieces */
		for (_int i = 0; i < CCell::LINE_END; i++)
			pCell->Set_NeighborIndex((CCell::LINE)i, file->Read<_int>());

		m_Cells.push_back(pCell);
	}

	return S_OK;
}

const _bool CNavMesh::Can_Move(_fvector vPoint, _int& iCurIndex)
{
	/* 반환값이 단순 bool 값이 아니라 여러 열거체로 사용가능 drop, jump, wall, 등 */
	/* 수업 코드상 트랜스폼 컴포넌트에서 사용 -> CTransform::Go_Straight() */

	if (m_Cells.empty() || m_Cells.size() <= iCurIndex)
		return FALSE;

	_int		iNeighborIndex = 0;

	/* 현재 셀을 나갔는가 */
	if (m_Cells[iCurIndex]->Is_Out(vPoint, &iNeighborIndex))
	{
		/* 나간 방향에 이웃셀이 있는가 */
		if (-1 != iNeighborIndex)
		{
			while (TRUE)
			{
				if (-1 == iNeighborIndex)
					return FALSE;

				if (FALSE == m_Cells[iNeighborIndex]->Is_Out(vPoint, &iNeighborIndex))
				{
					iCurIndex = iNeighborIndex;
					break;
				}
			}
			return TRUE;
		}
		else
			return FALSE;

	}
	else
		return TRUE;
}

HRESULT CNavMesh::Bind_ShaderResources()
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

	return S_OK;
}

HRESULT CNavMesh::Clear_NavDate()
{
	if (!m_Cells.empty())
	{
		for (auto& pCell : m_Cells)
			Safe_Release(pCell);

		m_Cells.clear();
		m_Cells.shrink_to_fit();
	}

	if (!m_AddedCellsCache.empty())
	{
		for (auto& pCell : m_AddedCellsCache)
			Safe_Release(pCell);

		m_AddedCellsCache.clear();
		m_AddedCellsCache.shrink_to_fit();
	}
	return S_OK;
}

HRESULT CNavMesh::Add_Cell(const Vec3* pPoints)
{
	CCell* pCell = CCell::Create(m_pDevice, m_pContext, pPoints, (_uint)m_Cells.size());

	if (nullptr != pCell)
	{
		m_Cells.push_back(pCell);
		m_AddedCellsCache.push_back(pCell);
		return S_OK;
	}

	return E_FAIL;
}

HRESULT CNavMesh::Delete_Cell(const _uint iIndex)
{
	/* iIndex를 레퍼런스로 전달하면 아래 Safe_Release 부분에서 iIndex의 값이 바뀌게 된다. */

	/* 캐시에서도 삭제 */
	for (vector<CCell*>::iterator iter = m_AddedCellsCache.begin(); iter != m_AddedCellsCache.end();)
	{
		if (*iter == m_Cells[iIndex])
		{
			iter = m_AddedCellsCache.erase(iter);
			break;
		}
		else
			++iter;
	}


	/*const _int* NeighborIndeces = m_Cells[iIndex]->Get_NeighborIndices();

	for (size_t i = 0; i < CCell::LINE_END; i++)
	{
		if(-1 != NeighborIndeces[i])
			m_Cells[NeighborIndeces[i]]->Remove_Neighbor(iIndex);
	}*/

	//Safe_Release(m_Cells[iIndex]);


	for (vector<CCell*>::iterator iter = m_Cells.begin(); iter != m_Cells.end();)
	{
		if ((*iter)->Get_Index() == iIndex)
		{
			Safe_Release(*iter);
			iter = m_Cells.erase(iter);
			return S_OK;
		}
		else
			++iter;
	}

	return E_FAIL;
}

const _int CNavMesh::Find_Cell(Vec3 vWorldPos)
{
	vWorldPos.y += 0.1f;

	Ray ray(vWorldPos, Vec3::Down);

	_int iIndex = -1;
	_float fMinDist = 9999;

	for (auto& pCell : m_Cells)
	{
		const Vec3* vPoints = pCell->Get_Points();
		_float fDist;
		if (ray.Intersects(vPoints[CCell::POINT_A], vPoints[CCell::POINT_B], vPoints[CCell::POINT_C], fDist))
		{
			if (fDist < fMinDist)
			{
				fMinDist = fDist;
				iIndex = pCell->Get_Index();
			}			
		}
	}

	return iIndex;
}

void CNavMesh::Get_SnapCellPos(_Inout_ Vec3& vWorldPos)
{
	const _float	fCanSnapPointDistnace = 0.5f;

	_float fMinDistance = 9999.f;

	for (auto& pCell : m_Cells)
	{
		if (nullptr == pCell) continue;

		const Vec3* vPoints = pCell->Get_Points();

		_float fDistWorldA = Vec3(vWorldPos - vPoints[CCell::POINT_A]).Length();
		_float fDistWorldB = Vec3(vWorldPos - vPoints[CCell::POINT_B]).Length();
		_float fDistWorldC = Vec3(vWorldPos - vPoints[CCell::POINT_C]).Length();

		_float fCurMinDistance = min(min(fDistWorldA, fDistWorldB), fDistWorldC);

		/* 세 점과 매개변수 사이 거리가 스냅 포인트보다 가깝고, 이전 최소 거리보다 작을 경우*/
		if (fCurMinDistance <= fCanSnapPointDistnace && fCurMinDistance <= fMinDistance)
		{
			fMinDistance = fCurMinDistance;

			if (fMinDistance == fDistWorldA)
			{
				vWorldPos = vPoints[CCell::POINT_A];
			}
			else if (fMinDistance == fDistWorldB)
			{
				vWorldPos = vPoints[CCell::POINT_B];
			}
			else if (fMinDistance == fDistWorldC)
			{
				vWorldPos = vPoints[CCell::POINT_C];
			}
		}
	}
}



void CNavMesh::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Clear_NavDate();
}
