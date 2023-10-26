#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CCell final : public CBase
{
public:
	enum POINTS		{ POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE		{ LINE_AB, LINE_BC, LINE_CA, LINE_END };
	
	enum class CELL_TYPE { NORMAL, JUMP, TYPEEND };

private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCell() = default;

public:
	HRESULT		Initialize(const _float3* pPoints, _uint iIndex);
	void		Update(_fmatrix WorldMatrix);
#ifdef _DEBUG
	HRESULT		Render();
#endif


public:
	const _uint&	Get_Index() const { return m_iIndex; }
	const _float3*	Get_Point(POINTS ePoint) const { return &m_vPoints_InWorld[ePoint]; }	
	const _int*		Get_NeighborIndices() const { return m_iNeighborIndices; }
	const _int		Get_NeighborIndex(LINE eLine) const { return m_iNeighborIndices[eLine]; }
	const _float3*	Get_Normal(LINE eLine) const { return &m_vNormals[eLine]; }
	Vec3			Get_CenterPoint() const { return m_vCenterPoint; }
	const Vec3*		Get_Points() const { return m_vPoints_InWorld; }
	
public:
	void			Set_Index(const _uint& iIndex) { memcpy(&m_iIndex, &iIndex, sizeof(_uint)); }
	void			Set_Normal(LINE eLine, Vec3 vNormal) { memcpy(&m_vNormals[eLine], &vNormal, sizeof(Vec3)); }
	void			Set_NeighborIndex(LINE eLine, _int iIndex) { memcpy(&m_iNeighborIndices[eLine], &iIndex, sizeof(_int)); }
	void			Set_Neighbor(LINE eLine, CCell* pCell) { m_iNeighborIndices[eLine] = pCell->m_iIndex; }	
	void			Set_Picked(const _bool bPicked) { m_bPicked = bPicked; }

public:
	_bool			Is_Out(_fvector vPoint, _int* pNeighborIndex);		
	_bool			Is_Picked() const { return m_bPicked; }

public:
	_bool			Compare_Points(const _float3* pSourPoint, const _float3* pDestPoint);	

	HRESULT			Remove_Neighbor(const _int& iIndex);
	HRESULT			Clear_NeighborIndices();

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	_uint					m_iIndex = {};
	Vec3					m_vPoints_InWorld[POINT_END];
	Vec3					m_vNormals[LINE_END];
	_int					m_iNeighborIndices[LINE_END] = { -1, -1, -1 };

	Vec3					m_vCenterPoint;

	_float					m_fPlusHeight = 0.f; // 원래 정점에서 얼마나 올라갔냐 (공중 상태용) -> 정점을 나타내는 모든 데이터 y에 더함
	_bool					m_bPicked = FALSE;

	CELL_TYPE				m_eType = CELL_TYPE::NORMAL;

#ifdef _DEBUG
private:
	class CVIBuffer_Cell*	m_pVIBuffer = { nullptr };
#endif

public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _uint iIndex);
	virtual void Free() override;
};

END