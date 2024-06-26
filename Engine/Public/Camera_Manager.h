#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CPipeLine;

class CCamera_Manager final : public CBase
{
	DECLARE_SINGLETON(CCamera_Manager)

	typedef map<_uint, CGameObject*>				CAMERAS;

private:
	CCamera_Manager();
	virtual ~CCamera_Manager() = default;

public:
	HRESULT Initialize();
	void Tick(_double fTimeDelta);

public:
	CGameObject*	Get_Camera(const _uint& iKey);
	const CAMERAS*	Get_Cameras() const { return &m_Cameras; }
	CGameObject*	Get_CurCamera() const { return m_pCurCamera; }
	const Vec4		Get_CurCamera_State(const _uint iState);

public:
	HRESULT			Set_CurCamera(const _uint& iKey);

public:
	const _bool&	Is_Lerp() const { return m_bCameraChange; }

public:
	HRESULT			Add_Camera(const _uint& iKey, CGameObject* pCamera);
	HRESULT			Change_Camera(const _uint& iKey, const _float& fLerpTime, const LERP_MODE& eLerpMode);
	void			Shake_Camera(const _float& fShakeTime, const _int& iIntensity);

private:
	CGameObject*	Find_Camera(const _uint& iKey);
	void			Lerp_Camera(_double fTimeDelta);

private:
	/* PipeLine */
	CPipeLine*		m_pPipeLine = { nullptr };

	/* Cameras */
	CAMERAS			m_Cameras;
	CGameObject*	m_pCurCamera = { nullptr };
	CGameObject*	m_pNextCamera = { nullptr };

	/* Lerp */
	_bool			m_bCameraChange = FALSE;
	LERP_TIME_DESC	m_tLerpTimeDesc;
	Matrix			m_matWILerp;
	LERP_FLOAT_DESC	m_tLerpFovDesc;

public:
	virtual void Free() override;
};

END

