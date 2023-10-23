#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

public:
	virtual HRESULT Render();

public:
	Vec3*			Get_VerticesPos() const { return m_pVerticesPos; }
	Vec3*			Get_VerticesNor() const { return m_pVerticesNor; }
	//const _uint&	Get_IndicesNum() const { return m_iNumPrimitives; }
	const _uint&	Get_NumPrimitives() const { return m_iNumPrimitives; }

protected:
	ID3D11Buffer*					m_pVB = nullptr;
	ID3D11Buffer*					m_pIB = nullptr;

	D3D11_BUFFER_DESC				m_BufferDesc;
	D3D11_SUBRESOURCE_DATA			m_SubResourceData;

	D3D11_PRIMITIVE_TOPOLOGY		m_eTopology;

	Vec3*							m_pVerticesPos = nullptr;
	Vec3*							m_pVerticesNor = nullptr;

protected:
	_uint						m_iNumVertexBuffers = 0; // 버텍스 버퍼 갯수(m_iNumVBs)
	_uint						m_iNumVertices = 0; // 버텍스 갯수(")
	_uint						m_iStride = 0; // 버텍스 하나 크기 (")

	_uint						m_iNumPrimitives = 0; // 폴리곤 갯수(m_iNumIndices)
	_uint						m_iIndexSizeofPrimitive = 0; // 인덱스 하나 크기(m_iIndexStride)
	_uint						m_iNumIndicesofPrimitive = 0;  // 폴리곤 하나가 사용하는 인덱스 갯수 (3개)

	DXGI_FORMAT					m_eIndexFormat;


protected:
	HRESULT Create_VertexBuffer();
	HRESULT Create_IndexBuffer();

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free();
};

END