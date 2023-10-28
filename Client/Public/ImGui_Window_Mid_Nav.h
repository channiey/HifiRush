#pragma once
#ifdef _DEBUG

#include "ImGui_Window.h"

BEGIN(Engine)

class CCell;
END

BEGIN(Client)

class CImGui_Window_Mid_Nav final : public CImGui_Window
{
protected:
	CImGui_Window_Mid_Nav();
	virtual~CImGui_Window_Mid_Nav() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void	Show_Window() override;

public:
	virtual void	Clear_Reference_Data() override;

private:
	HRESULT			Clear();
	HRESULT			Bake();
	HRESULT			Save_NavData();
	HRESULT			Load_NavData();

public:
	void			Edit_Update();
	HRESULT			Create_Cell(Vec3 vPoint);
	HRESULT			Delete_Cell();
	HRESULT			Delete_Point();

private:
	HRESULT			Create_Cells(vector<CCell*>& Cells);
	HRESULT			Set_Neighbors();

private:
	void			Render_PopUp_Clear();
	void			Render_PopUp_Save();
	void			Render_PopUp_Load();

private:
	void			Render_Sphere();

private:
	_float			m_fMaxSlope		= 80.f;
	_float			m_fMinArea		= 0.5f;


	_bool			m_bPopUp_Clear = FALSE;
	_bool			m_bPopUp_Save = FALSE;
	_bool			m_bPopUp_Load = FALSE;

	_bool			m_bEditing = FALSE;

	vector<Vec3>	m_CellPointsCache;

	vector<class CCell*> m_CellPicked;

private:
	BoundingSphere* m_Shperes[3];
	PrimitiveBatch<VertexPositionColor>* m_pBatch = nullptr;
	BasicEffect* m_pEffect = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;

public:
	static CImGui_Window_Mid_Nav* Create();
	virtual void Free() override;
};
END

#endif // _DEBUG