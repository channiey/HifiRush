#include "..\Public\Target_Manager.h"
#include "RenderTarget.h"

IMPLEMENT_SINGLETON(CTarget_Manager)

CTarget_Manager::CTarget_Manager()
{

}

HRESULT CTarget_Manager::Add_RenderTarget(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring & strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vColor)
{
	if (nullptr != Find_RenderTarget(strTargetTag))
		return E_FAIL;

	CRenderTarget*	pRenderTarget = CRenderTarget::Create(pDevice, pContext, iSizeX, iSizeY, ePixelFormat, vColor);
	
	if (nullptr == pRenderTarget)
		return E_FAIL;

	m_RenderTargets.emplace(strTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CTarget_Manager::Add_MRT(const wstring & strMRTTag, const wstring & strTargetTag)
{
	/* 렌더 타겟을 찾는다 (없으면 생성) */
	CRenderTarget*	pRenderTarget = Find_RenderTarget(strTargetTag);
	
	if (nullptr == pRenderTarget)
		return E_FAIL;

	/* MRT를 찾는다 (없으면 생성) */
	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);

	if (nullptr == pMRTList)
	{
		list<CRenderTarget*>		MRTList;
		MRTList.push_back(pRenderTarget);

		m_MRTs.emplace(strMRTTag, MRTList);
	}
	else
		pMRTList->push_back(pRenderTarget);

	Safe_AddRef(pRenderTarget);

	return	S_OK;
}

HRESULT CTarget_Manager::Bind_SRV(CShader * pShader, const wstring & strTargetTag, const _char * pConstantName)
{
	CRenderTarget*		pRenderTarget = Find_RenderTarget(strTargetTag);
	
	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Bind_SRV(pShader, pConstantName);
}

HRESULT CTarget_Manager::Begin_MRT(ID3D11DeviceContext* pContext, const wstring & strMRTTag, ID3D11DepthStencilView* pDSV)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(strMRTTag);

	if (nullptr == pMRTList)
		return E_FAIL;
		
	pContext->OMGetRenderTargets(1, &m_pBackBufferRTV, &m_pDSV);

	ID3D11RenderTargetView*		pRenderTargets[8] = {};

	_uint			iNumRTVs = 0;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTargets[iNumRTVs++] = pRenderTarget->Get_RTV();
		pRenderTarget->Clear();
	}

	if (!pDSV)
		pContext->OMSetRenderTargets(iNumRTVs, pRenderTargets, m_pDSV);
	else
		pContext->OMSetRenderTargets(iNumRTVs, pRenderTargets, pDSV);

	return	S_OK;
}

HRESULT CTarget_Manager::End_MRT(ID3D11DeviceContext* pContext, ID3D11DepthStencilView* pDSV)
{
	pContext->OMSetRenderTargets(1, &m_pBackBufferRTV, m_pDSV);

	if (!pDSV)
		Safe_Release(pDSV);
	else
		Safe_Release(m_pDSV);

	Safe_Release(m_pBackBufferRTV);

	return	S_OK;
}

HRESULT CTarget_Manager::Ready_Debug(const wstring & strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CRenderTarget*		pRenderTarget = Find_RenderTarget(strTargetTag);

	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Ready_Debug(fX, fY, fSizeX, fSizeY);	
}

HRESULT CTarget_Manager::Render(const wstring & strMRTTag, CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(strMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Render(pShader, pVIBuffer);
	}

	return S_OK;
}

CRenderTarget * CTarget_Manager::Find_RenderTarget(const wstring & strTargetTag)
{
	auto	iter = m_RenderTargets.find(strTargetTag);

	if (iter == m_RenderTargets.end())
		return nullptr;

	return iter->second;
}

list<class CRenderTarget*>* CTarget_Manager::Find_MRT(const wstring & strMRTTag)
{
	auto	iter = m_MRTs.find(strMRTTag);

	if (iter == m_MRTs.end())
		return nullptr;

	return &iter->second;
}

void CTarget_Manager::Free()
{
	for (auto& Pair : m_MRTs)
	{
		for (auto& pRenderTarget : Pair.second)
		{
			Safe_Release(pRenderTarget);
		}
		Pair.second.clear();
	}
	m_MRTs.clear();
		

	for (auto& Pair : m_RenderTargets)	
		Safe_Release(Pair.second);

	m_RenderTargets.clear();
}
