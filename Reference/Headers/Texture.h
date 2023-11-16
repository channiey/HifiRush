#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTexture final : public CComponent
{
private:
	CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;
public:
	virtual HRESULT Initialize_Prototype(const wstring& strTextureFilePath, _uint iNumTextures);
	virtual HRESULT Initialize(void* pArg) override;

public:
	/* 텍스처 한장을 쉐이더로 던진다. */
	HRESULT Bind_ShaderResource(const class CShader* pShader, const char* pConstantName, _uint iTextureIndex);

	/* 배열에 추가해놓은 모든 텍스쳐를 셰이더 배열에 던진다.  */
	HRESULT Bind_ShaderResources(const class CShader* pShader, const char* pConstantName);

	const string& Get_TextureName() const { return m_strTextureName; }


private:
	/* 텍스처를 셰이더에 바인딩 하기 위한 인터페이스(텍스처 뿐만아니라 상수버퍼 등도 바인딩 가능하다) */
	ID3D11ShaderResourceView**					m_ppSRVs = { nullptr };
	_uint										m_iNumTextures = { 0 };
	string										m_strTextureName;

public:
	static CTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strTextureFilePath, _uint iNumTextures = 1);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END