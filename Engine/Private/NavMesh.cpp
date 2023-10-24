#include "..\Public\NavMesh.h"

#include "GameInstance.h"
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

#ifdef _DEBUG
	m_pShader = CShader::Create(m_pDevice,m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;
#endif // _DEBUG
	return TRUE;

}

#ifdef _DEBUG
HRESULT CNavMesh::Render()
{
	if (!m_bRender || nullptr == m_pShader)
		return S_OK;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	/* Color */
	_float4 vColor{ 0.f, 1.f, 0.f, 1.f };
	if (FAILED(m_pShader->Bind_RawValue("g_vLineColor", &vColor, sizeof(_float4))))
		return E_FAIL;

	/* Height */
	_float	fHeight = 0.f;
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
			if (m_fRenderRange < fDist)
				continue;
			pCell->Render();
		}
	}

	return S_OK;
}
HRESULT CNavMesh::Render_Cell(const _int& iInedx)
{
	if (!m_bRender || nullptr == m_pShader || m_Cells.size() <= iInedx)
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

	/*_float fDist = Vec3(m_Cells[iInedx]->Get_CenterPoint() - GAME_INSTNACE->Get_CamPosition().xyz()).Length();
	if (m_fRenderRange >= fDist)*/
	{
		m_Cells[iInedx]->Render();
	}

	return S_OK;
}
#endif // _DEBUG

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
	return S_OK;
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



void CNavMesh::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Clear_NavDate();
}
