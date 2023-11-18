#pragma once

#include "Base.h"

BEGIN(Engine)

class CFont_Manager final : public CBase
{
	DECLARE_SINGLETON(CFont_Manager)

private:
	CFont_Manager();
	virtual ~CFont_Manager() = default;

public:
	HRESULT Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strFontTag, const wstring& strFontFilePath);
	HRESULT Render(const wstring& strFontTag, const wstring & strText, const _float2 & vPos, FXMVECTOR color, float rotation, XMFLOAT2 const & origin, float scale);

private:
	class CCustomFont* Find_Font(const wstring& strFontTag);

private:
	map<const wstring, class CCustomFont*>	m_Fonts;

public:
	virtual void Free() override;
};

END