#pragma once

/* 렌더타겟들을 생성해서 모아둔다. */
#include "Base.h"

BEGIN(Engine)

class CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)

private:
	CTarget_Manager();
	virtual ~CTarget_Manager() = default;

public:
	HRESULT			Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vColor);
	HRESULT			Add_MRT(const wstring& strMRTTag, const wstring& strTargetTag);

public:
	HRESULT			Bind_SRV(class CShader* pShader, const wstring& strTargetTag, const _char* pConstantName);

	HRESULT			Begin_MRT(ID3D11DeviceContext* pContext, const wstring& strMRTTag, ID3D11DepthStencilView* pDSV = nullptr);/* strMRTTag에 해당하는 list에 담겨있는 타겟들을 장치에 바인딩한다. */
	HRESULT			End_MRT(ID3D11DeviceContext* pContext, ID3D11DepthStencilView* pDSV = nullptr); /* 다시 원래 상태로 복구한다(원래 백버퍼 세팅). */

public:
	HRESULT			Ready_Debug(const wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT			Render(const wstring& strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);

public:
	const _bool&	Is_DebugRTV() const					{ return m_bDebugRTV; }
	void			Set_DebugRTV(const _bool& bSet)		{ m_bDebugRTV = bSet; }
	void			Set_DebugRTV()						{ m_bDebugRTV = !m_bDebugRTV; }

private:
	class CRenderTarget*		Find_RenderTarget(const wstring& strTargetTag);
	list<class CRenderTarget*>* Find_MRT(const wstring & strMRTTag);

private:
	map<const wstring, class CRenderTarget*>			m_RenderTargets;	

	/* 장치에 동시에 바인딩되어야하는 타겟들을 미리 묶어두겠다. */
	map<const wstring, list<class CRenderTarget*>>		m_MRTs;

	_bool												m_bDebugRTV = FALSE;

private:
	ID3D11RenderTargetView*								m_pBackBufferRTV = { nullptr };
	ID3D11DepthStencilView*								m_pDSV = { nullptr };

public:
	virtual void Free() override;
};

END