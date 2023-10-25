#pragma once

#include "Base.h"

BEGIN(Engine)
class CCell;
class CShader;

class ENGINE_DLL CNavMesh final : public CBase
{
	DECLARE_SINGLETON(CNavMesh)

private:
	CNavMesh();
	virtual ~CNavMesh() = default;

public:
	HRESULT	Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT Render();
	HRESULT Render_Cell(const _int& iInedx);


public:
	void			Set_RenderRange(const _float fRange) { m_fRenderRange = fRange; }
	void			Set_Render(const _bool& bRender) { m_bRender = bRender; }
	void			Set_Render() { m_bRender = !m_bRender; }

	const _float&	Get_RenderRange() const { return m_fRenderRange; }
	const size_t	Get_CountCells() const { return m_Cells.size(); }

	const _bool&	Is_Render() const { return m_bRender; }
	const _bool		Is_EmptyCells() const { return m_Cells.empty(); }

public:
	const _int		Find_Cell(Vec3 vWorldPos);

public:
	HRESULT			Set_NavDate(vector<CCell*>& Cells);
	HRESULT			Save_NavData(const wstring& strFilePath);
	HRESULT			Load_NavData(const wstring& strFilePath);
	HRESULT			Clear_NavDate();


public:
	const _bool		Can_Move(_fvector vPoint, _int& iCurIndex);

private:
	HRESULT			Bind_ShaderResources();

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	
	vector<CCell*>	m_Cells;

	CShader*		m_pShader = { nullptr };
	_float			m_fRenderRange = 10.f;
	_bool			m_bRender = FALSE;

public:
	virtual void			Free();
};
END

