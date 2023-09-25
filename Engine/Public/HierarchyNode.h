#pragma once

#include "Base.h"

/* 계층구조(뼈끼리의 상속관계)를 표현하기위한 aiNode. */

BEGIN(Engine)

class ENGINE_DLL CHierarchyNode final : public CBase
{
private:
	CHierarchyNode();
	virtual ~CHierarchyNode() = default;

public:
	HRESULT Initialize(aiNode* pAINode, class CHierarchyNode* pParent, _uint iDepth);
	void Set_CombinedTransformation();
	void Set_OffsetMatrix(_fmatrix OffsetMatrix);

public:
	const char*		Get_Name() const { return m_szName; }
	_uint			Get_Depth() const { return m_iDepth; }
	_matrix			Get_OffSetMatrix() { return XMLoadFloat4x4(&m_OffsetMatrix); }
	_matrix			Get_CombinedTransformation() { return XMLoadFloat4x4(&m_CombinedTransformation); }

public:
	void			Set_Transformation(_fmatrix Transformation) { XMStoreFloat4x4(&m_Transformation, Transformation); }

private:
	char				m_szName[MAX_PATH] = "";
	_float4x4			m_OffsetMatrix; /* ToRoot 행렬(부모의 m_CombinedTransformation)의 역행렬 ? */
	_float4x4			m_Transformation; /* Relative(부모 기준) - 여기다가 부모의 m_CombinedTransformation을 곱하면 루트 기준으로 변환 */
	_float4x4			m_CombinedTransformation; /* Global(루트 기준) */
	CHierarchyNode*		m_pParent = nullptr;
	_uint				m_iDepth = 0;

public:
	static CHierarchyNode* Create(aiNode* pAINode, class CHierarchyNode* pParent, _uint iDepth);
	virtual void Free();
};

END