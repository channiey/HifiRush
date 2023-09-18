#pragma once

#include "Component.h"

/* CVIBuffer : Vertices + Indices */
/* 정점과 인덱스를 할당하고 초기화한다. */
/* 렌더함수에서  이 두 버퍼를 이용하여 그린다.  */

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract : public CComponent
{
protected:
	CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render();

protected:
	ID3D11Buffer*				m_pVB = { nullptr };
	ID3D11Buffer*				m_pIB = { nullptr };	

	D3D11_BUFFER_DESC			m_BufferDesc;				/* 버퍼를 만들기 위한 정보를 가진 구조체 1 */
	D3D11_SUBRESOURCE_DATA		m_InitialData;				/* 버퍼를 만들기 위한 정보를 가진 구조체 2 */

	_uint						m_iVertexStride = { 0 };	/* 정점 하나의 크기(Byte) */
	_uint						m_iNumVertices	= { 0 };	/* 정점의 갯수 */

	_uint						m_iIndexStride	= { 0 };	/* 인덱스 하나의 크기 - 2 or 4*/
	_uint						m_iNumIndices	= { 0 };	/* 인덱스 갯수*/
	DXGI_FORMAT					m_eIndexFormat;				/* 인덱스 버퍼 포맷 - 인덱스 크기에 따라 결정 된다.*/

	D3D11_PRIMITIVE_TOPOLOGY	m_eTopology;				/* 정점을 어떻게 그릴 것인지 */
	_uint						m_iNumVBs = { 0 };			/* 버텍스 버퍼의 갯수 - 후반에는 여러 개의 버텍스 버퍼 사용 가능*/

	_uint						m_iNumTirs = { 0 };			/* 삼각형 갯수 */

protected:
	/* 자식 버퍼 개체에서 사용한다.*/
	HRESULT Create_Buffer(_Inout_ ID3D11Buffer** ppOut);

	

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END