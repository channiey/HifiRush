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
	/* 1. ���̴� ��� ���̺��� ���� ä���. (�ǵ��� �� �Լ����� �ڷ����� �ٲ��� �ʵ��� �Ѵ�. */
	HRESULT Bind_Matrix(const char* pConstantName, const _float4x4* pMatrix) const;
	HRESULT Bind_RawValue(const char* pConstantName, const void* pData, _uint iSize);
	HRESULT Bind_Matrices(const char* pConstantName, const _float4x4* pMatrices, _uint iNumMatrices)const;
	HRESULT Bind_Texture(const char * pConstantName, ID3D11ShaderResourceView* pSRV)const;
	HRESULT Bind_Textures(const char * pConstantName, ID3D11ShaderResourceView ** ppSRVs, _uint iNumTextures)const;
	
	/* 2. �׸��� ���� (�ݵ�� �ش� �Լ� ȣ������ ���̴� ������̺� ���� ���� �Ǿ� �־�� �Ѵ�. */
	HRESULT	Begin(_uint iPassIndex);

private:	
	ID3DX11Effect*				m_pEffect = { nullptr };
	/* ID3D11InputLayout�� ���� ������ �����Ѵ�. (dx9������ fvf�� �����ϴ�) */
	/* ID3D11InputLayout�� �� �н����� ������ָ�, ���� ����ü�� ������ �ִ� ������ ������ ��ġ�ؾ��Ѵ�. */
	vector<ID3D11InputLayout*>	m_InputLayouts; 

public:
	static CShader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strShaderFilePath, 
		const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END