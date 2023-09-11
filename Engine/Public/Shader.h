#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
private:
	CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:
	virtual HRESULT Initialize_Prototype(const wstring& strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual HRESULT Initialize(void* pArg) override;

public:
	/* 1. 셰이더 상수 테이블의 값을 채운다. */
	HRESULT Bind_Matrix(char* pConstantName, const _float4x4* pMatrix);
	HRESULT Bind_Matrices(char* pConstantName, const _float4x4* pMatrices, _uint iNumMatrices);
	HRESULT Bind_Texture(char * pConstantName, ID3D11ShaderResourceView* pSRV);
	HRESULT Bind_Textures(char * pConstantName, ID3D11ShaderResourceView ** ppSRVs, _uint iNumTextures);
	
	/* 2. 그리기 시작 (반드시 해당 함수 호출전에 셰이더 상수테이블에 값이 세팅 되어 있어야 한다. */
	HRESULT	Begin(_uint iPassIndex);

private:	
	ID3DX11Effect*				m_pEffect = { nullptr };
	/* ID3D11InputLayout은 정점 정보를 묘사한다. (dx9에서의 fvf와 유사하다) */
	/* ID3D11InputLayout은 각 패스마다 만들어주며, 정점 구조체가 가지고 있는 변수의 정보와 일치해야한다. */
	vector<ID3D11InputLayout*>	m_InputLayouts; 

public:
	static CShader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strShaderFilePath, 
		const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END