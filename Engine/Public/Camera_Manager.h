#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CPipeLine;

class CCamera_Manager final : public CBase
{
	DECLARE_SINGLETON(CCamera_Manager)

private:
	CCamera_Manager();
	virtual ~CCamera_Manager() = default;

public:
	HRESULT Initialize();
	void Tick();

public:
	CGameObject*	Get_Camera(const _uint& iKey);
	const map<_uint, CGameObject*>* Get_Cameras() const { return &m_Cameras; }

	CGameObject*	Get_CurCamera() const { return m_pCurCamera; }
	const Vec4		Get_CurCamera_State(const _uint iState);

public:
	HRESULT			Set_CurCamera(const _uint& iKey);

	HRESULT			Add_Camera(const _uint& iKey, CGameObject* pCamera);
	HRESULT			Change_Camera(const _uint& iKey);
	HRESULT			Change_Camera_Inverse();


private:
	CGameObject*	Find_Camera(const _uint& iKey);

private:
	CGameObject*				m_pPreCamera = { nullptr };
	CGameObject*				m_pCurCamera = { nullptr };
	
	CPipeLine*					m_pPipeLine = { nullptr };

	map<_uint, CGameObject*>	m_Cameras;

	_bool						m_bCameraChange = FALSE;

public:
	virtual void Free() override;
};

END

