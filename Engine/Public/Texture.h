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
	/* �ؽ�ó ������ ���̴��� ������. */
	HRESULT Bind_ShaderResource(const class CShader* pShader, const char* pConstantName, _uint iTextureIndex);

	/* �迭�� �߰��س��� ��� �ؽ��ĸ� ���̴� �迭�� ������.  */
	HRESULT Bind_ShaderResources(const class CShader* pShader, const char* pConstantName);

	const string& Get_TextureName() const { return m_strTextureName; }


private:
	/* �ؽ�ó�� ���̴��� ���ε� �ϱ� ���� �������̽�(�ؽ�ó �Ӹ��ƴ϶� ������� � ���ε� �����ϴ�) */
	ID3D11ShaderResourceView**					m_ppSRVs = { nullptr };
	_uint										m_iNumTextures = { 0 };
	string										m_strTextureName;

public:
	static CTexture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strTextureFilePath, _uint iNumTextures = 1);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END