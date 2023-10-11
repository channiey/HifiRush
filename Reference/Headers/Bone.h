#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CBone final : public CBase
{
private:
	CBone();
	CBone(const CBone& rhs);
	virtual ~CBone() = default;

public:
	HRESULT Initialize_Prototype(string strName, Matrix transformMatrix, Matrix offsetMatrix, _int iBoneIndex, _int iParentIndex, _uint iDepth);
	HRESULT Initialize(void* pArg);
public:
	const char*		Get_Name() const { return m_szName; }
	_uint			Get_Depth() const { return m_iDepth; }
	const _int&		Get_Index() const { return m_iIndex; }
	const _int&		Get_ParentIndex() const { return m_iParentIndex; }
	_matrix			Get_OffSetMatrix() { return XMLoadFloat4x4(&m_OffsetMatrix); }
	_matrix			Get_CombinedTransformation() { return XMLoadFloat4x4(&m_CombinedTransformation); }
	const Matrix	Get_Transfromation() const { return m_Transformation; }

public:
	HRESULT			Set_Parent(CBone* pParent);
	void			Set_Transformation(_fmatrix Transformation) { XMStoreFloat4x4(&m_Transformation, Transformation); }
	void			Set_CombinedTransformation();
	void			Set_OffsetMatrix(_fmatrix OffsetMatrix) { XMStoreFloat4x4(&m_OffsetMatrix, OffsetMatrix); }

private:
	char			m_szName[MAX_PATH] = "";

	_float4x4		m_Transformation;			/* Relative(부모 기준) - 여기다가 부모의 m_CombinedTransformation을 곱하면 루트 기준으로 변환 */
	_float4x4		m_CombinedTransformation;	/* Global(루트 기준) */
	_float4x4		m_OffsetMatrix;				/* 애님이 서로 다른 모델에도 적용될 수 있게 하기 위한 매트릭스? */

	CBone*			m_pParent = nullptr;
	_int			m_iParentIndex = 0;
	_int			m_iIndex = 0;
	_uint			m_iDepth = 0; 

public:
	static CBone* Create(string strName, Matrix transformMatrix, Matrix offsetMatrix, _int iBoneIndex, _int iParentIndex, _uint iDepth);
	CBone* Clone(void* pArg = nullptr);
	virtual void Free();
};

END