#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CCell final : public CBase
{
public:
	enum POINTS		{ POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE		{ LINE_AB, LINE_BC, LINE_CA, LINE_END };
	enum CELL_TYPE	{ WALL, JUMP, TYPE_END };
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
	const _int		Get_NeighborIndex(LINE eLine) const { return m_iNeighborIndices[eLine]; }
	const _float3*	Get_Normal(LINE eLine) const { return &m_vNormals[eLine]; }
	Vec3			Get_CenterPoint() const { return m_vCenterPoint; }
	
public:
	void			Set_Index(const _uint& iIndex) { memcpy(&m_iIndex, &iIndex, sizeof(_uint)); }
	void			Set_Normal(LINE eLine, Vec3 vNormal) { memcpy(&m_vNormals[eLine], &vNormal, sizeof(Vec3)); }
	void			Set_NeighborIndex(LINE eLine, _int iIndex) { memcpy(&m_iNeighborIndices[eLine], &iIndex, sizeof(_int)); }
	void			Set_Neighbor(LINE eLine, CCell* pCell) { m_iNeighborIndices[eLine] = pCell->m_iIndex; }	// 이웃 설정 

public:
	_bool			Is_Out(_fvector vPoint, _int* pNeighborIndex);		// 객체가 현재 셀을 나갔는지 판단 
	_bool			Compare_Points(const _float3* pSourPoint, const _float3* pDestPoint);	// 이웃 설정 

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	_uint					m_iIndex = {};
	Vec3					m_vPoints_InWorld[POINT_END];	// 스태틱 객체 월드로 올림 
	Vec3					m_vNormals[LINE_END];
	_int					m_iNeighborIndices[LINE_END] = { -1, -1, -1 };

	Vec3					m_vCenterPoint;
	//_float3					m_vPoints_Original[POINT_END];	// 로컬 

#ifdef _DEBUG
private:
	class CVIBuffer_Cell*	m_pVIBuffer = { nullptr };
#endif

public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _uint iIndex);
	virtual void Free() override;
};

END