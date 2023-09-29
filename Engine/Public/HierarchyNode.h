#pragma once

#include "Base.h"

/* 계층구조(뼈끼리의 상속관계)를 표현하기위한 aiNode. */

BEGIN(Engine)

class ENGINE_DLL CHierarchyNode final : public CBase
{
private:
	CHierarchyNode();
	CHierarchyNode(const CHierarchyNode& rhs);
	virtual ~CHierarchyNode() = default;

public:
	HRESULT Initialize_Prototype(string strName, Matrix transformMatrix, Matrix offsetMatrix, _uint iBoneIndex, _uint iParentIndex, _uint iDepth);
	HRESULT Initialize(void* pArg);
public:
	const char*		Get_Name() const { return m_szName; }
	_uint			Get_Depth() const { return m_iDepth; }
	_matrix			Get_OffSetMatrix() { return XMLoadFloat4x4(&m_OffsetMatrix); }
	_matrix			Get_CombinedTransformation() { return XMLoadFloat4x4(&m_CombinedTransformation); }
	const _uint&	Get_Index() const { return m_iIndex; }
	const _uint&	Get_ParentIndex() const { return m_iParentIndex; }

public:
	void			Set_CombinedTransformation();
	void			Set_OffsetMatrix(_fmatrix OffsetMatrix);
	void			Set_Transformation(_fmatrix Transformation) { XMStoreFloat4x4(&m_Transformation, Transformation); }
	HRESULT			Set_Parent(CHierarchyNode* pParent);

private:
	char				m_szName[MAX_PATH] = "";

	_float4x4			m_Transformation; /* Relative(부모 기준) - 여기다가 부모의 m_CombinedTransformation을 곱하면 루트 기준으로 변환 */
	_float4x4			m_CombinedTransformation; /* Global(루트 기준) */
	_float4x4			m_OffsetMatrix; /* Global(루트 기준 = m_CombinedTransformation) 의 역행렬 */

	CHierarchyNode*		m_pParent = nullptr;
	_uint				m_iParentIndex = 0;
	_uint				m_iIndex = 0;
	_uint				m_iDepth = 0; /* 트리에서의 깊이값, 즉 몇차 자식이냐 */

public:
	static CHierarchyNode* Create(string strName, Matrix transformMatrix, Matrix offsetMatrix, _uint iBoneIndex, _uint iParentIndex, _uint iDepth);
	CHierarchyNode* Clone(void* pArg = nullptr);
	virtual void Free();
};

END