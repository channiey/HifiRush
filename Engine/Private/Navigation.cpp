#include "..\Public\Navigation.h"
#include "Cell.h"
#include "GameInstance.h"
_float4x4 CNavigation::m_WorldMatrix = {};

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{

}

CNavigation::CNavigation(const CNavigation & rhs)
	: CComponent(rhs)
	, m_iCurrentIndex(rhs.m_iCurrentIndex)
	, m_Cells(rhs.m_Cells)	 // 셀의 정보는 얕은복사로 진행 (크기 큼)
#ifdef _DEBUG
	, m_pShader(rhs.m_pShader)
#endif
{

#ifdef _DEBUG
	Safe_AddRef(m_pShader);
#endif

	for (auto& pCell : m_Cells)	
		Safe_AddRef(pCell);
}

HRESULT CNavigation::Initialize_Prototype(const wstring & strNavigationDataFiles)
{
	/* 1. 파일에 저장된 폴리곤 정보(삼각형 정점들의 연속)을 읽어 셀을 생성한다. */
	{
		_ulong		dwByte = 0;
		HANDLE		hFile = CreateFile(strNavigationDataFiles.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
			return E_FAIL;

		while (true)
		{
			_float3		vPoints[CCell::POINT_END] = {};

			ReadFile(hFile, vPoints, sizeof(_float3) * CCell::POINT_END, &dwByte, nullptr);

			if (0 == dwByte)
				break;

			CCell*		pCell = CCell::Create(m_pDevice, m_pContext, vPoints, (_uint)m_Cells.size());
			if (nullptr == pCell)
				return E_FAIL;

			m_Cells.push_back(pCell);
		}

		CloseHandle(hFile);

	}

	/* 2. 셀이 모두 생성되면 셀의 변 정보를 비교하여 각 셀들의 이웃을 설정한다. */
	if (FAILED(Set_Neighbors()))
		return E_FAIL;	

	/* 3. 셀을 그리기 위한 셰이더를 생성한다. */
#ifdef _DEBUG
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;
#endif

	return S_OK;
}

HRESULT CNavigation::Initialize(void * pArg)
{	
	if (nullptr == pArg)
		return S_OK;

	NAVIGATION_DESC*		pNaviDesc = (NAVIGATION_DESC*)pArg;

	/*  이 네비게이션을 이용하고자하는 객체가 어떤 셀에 있는지 저장한다. */
	m_iCurrentIndex = pNaviDesc->iCurrentIndex;

	return S_OK;
}

void CNavigation::Update(_fmatrix WorldMatrix)
{
	/* 네비게이션 정보를 제공하는 메시 or 터레인 기준 좌표로 변경? */
	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);

	for (auto& pCell : m_Cells)
	{
		if (nullptr != pCell)
			pCell->Update(WorldMatrix);
	}
}

_bool CNavigation::Can_Move(_fvector vPoint)
{
	/* 반환값이 단순 bool 값이 아니라 여러 열거체로 사용가능 drop, jump, wall, 등 */
	/* 수업 코드상 트랜스폼 컴포넌트에서 사용 -> CTransform::Go_Straight() */

	//_int		iNeighborIndex = 0;

	//if (true == m_Cells[m_iCurrentIndex]->Is_Out(vPoint, XMLoadFloat4x4(&m_WorldMatrix), &iNeighborIndex))
	//{
	//	/* 나간 방향에 이웃셀이 있으면 움직여야해! */
	//	if (-1 != iNeighborIndex)
	//	{
	//		while (true)
	//		{
	//			if (-1 == iNeighborIndex)
	//				return false;

	//			if (false == m_Cells[iNeighborIndex]->Is_Out(vPoint, XMLoadFloat4x4(&m_WorldMatrix), &iNeighborIndex))
	//			{
	//				m_iCurrentIndex = iNeighborIndex;
	//				break;
	//			}
	//		}			
	//		return true;
	//	}
	//	else
	//		return false;

	//}
	//else
	//	return true;	

	return TRUE;

}



#ifdef _DEBUG

HRESULT CNavigation::Render()
{
	/* WVP 세팅 */
	{
		if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
			return E_FAIL;

		_float4x4 matView = GAME_INSTNACE->Get_Transform(CPipeLine::STATE_VIEW);
		_float4x4 matProj = GAME_INSTNACE->Get_Transform(CPipeLine::STATE_PROJ);

		if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &matView)))
			return E_FAIL;

		if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &matProj)))
			return E_FAIL;
	}

	/* 렌더는 스태틱, 다이나믹 오브젝트 모두 호출한다. */
	/* 스태틱은 현재 인덱스를 세팅하지 않았으므로 모든 셀을 그린으로 그리고 */
	/* 다이나믹은 현재 셀만 레드로 그린다. */
	if (-1 == m_iCurrentIndex)
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
				pCell->Render();
		}
	}
	else
	{
		_float4 vColor{ 1.f, 0.f, 0.f, 1.f };
		if (FAILED(m_pShader->Bind_RawValue("g_vLineColor", &vColor, sizeof(_float4))))
			return E_FAIL;

		_float		fHeight = 0.1f;
		if (FAILED(m_pShader->Bind_RawValue("g_fHeight", &fHeight, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pShader->Begin(0)))
			return E_FAIL;

		m_Cells[m_iCurrentIndex]->Render();
	}	

	return S_OK;
}

#endif

HRESULT CNavigation::Set_Neighbors()
{
	/* 네비게이션을 구성하는 각각의 셀들의 이웃을 설정한다. */
	
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

	/*for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

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
	}*/
	
	return S_OK;
}

HRESULT CNavigation::Clear_CellData()
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

CNavigation * CNavigation::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring & strNavigationDataFiles)
{
	CNavigation*	pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(strNavigationDataFiles)))
	{
		MSG_BOX("Failed to Created : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CNavigation::Clone(void * pArg)
{
	CNavigation*	pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

#ifdef _DEBUG
	Safe_Release(m_pShader);
#endif

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);
}
