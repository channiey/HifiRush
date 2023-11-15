#pragma once

#include "Component.h"

/* 모든 카메라에게 필요한 기능 :  */
/* 카메라의 상태 행렬을 이용하여 뷰스페이스 변환행렬.*/
/* 투영행렬(fov, aspect, near, far)을 설정하는작업.  */

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CCamera final : public CComponent
{
public:
	typedef struct tagProjDesc 
	{
		_float fFovy	= 0.f;
		_float fAspect	= 0.f;
		_float fNear	= 0.f;
		_float fFar		= 0.f;

	}PROJ_DESC;

	typedef struct tagShakeDesc
	{
		_bool	bActive		= FALSE;

		_float	fAcc		= 0.f;
		_float	fShakeTime	= 0.f;

		_float	fIntensity	= 0.f;


		void Reset()
		{
			bActive = FALSE;
			fAcc = fShakeTime = fIntensity = 0.f;
		}

	}SHAKE_DESC;

protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Update(const _double fDeltaTime);

public:
	CGameObject*		Get_TargetObj() const { return m_pTargetObj; }
	CGameObject*		Get_LookAtObj() const { return m_pLookAtObj; }
	Vec4				Get_TargetOffSet() const { return m_vTargetOffset; }
	Vec4				Get_LookAtOffSet() const { return m_vLookAtOffset; }
	
	const _float&		Get_Distance() const { return m_fDistance; }
	const _float&		Get_MouseSensitiveX() const { return m_fMouseSensitiveX; }
	const _float&		Get_MouseSensitiveY() const { return m_fMouseSensitiveY; }

	const PROJ_DESC&	Get_ProjDesc() const { return m_tProjDesc; }
	const _float&		Get_Fov() const { return m_tProjDesc.fFovy; }
	
	const _uint&		Get_Key() const { return m_iKey; }

public:
	void				Set_TargetObj(CGameObject* pObj)	{ m_pTargetObj = pObj; }
	void				Set_LookAtObj(CGameObject* pObj)	{ m_pLookAtObj = pObj; }
	void				Set_TargetOffSet(const Vec4 vOffset)	{ m_vTargetOffset = vOffset; }
	void				Set_LookAtOffSet(const Vec4 vOffset) { m_vLookAtOffset = vOffset; }
	
	void				Set_Distance(const _float& fDist) { m_fDistance = fDist; }
	void				Set_MouseSensitiveX(const _float& fValue) { m_fMouseSensitiveX = fValue; }
	void				Set_MouseSensitiveY(const _float& fValue) { m_fMouseSensitiveY = fValue; }

	void				Set_LockX(const _bool& bLock) { m_bLockX = bLock; }
	void				Set_LockY(const _bool& bLock) { m_bLockY = bLock; }
	void				Set_Fovy(const _float& fFovy) { m_tProjDesc.fFovy = fFovy; }

	void				Set_Key(const _uint& iKey) { m_iKey = iKey; }

public:
	const _bool			Is_TargetObj() const { return nullptr != m_pTargetObj ? TRUE : FALSE; }
	const _bool			Is_LookAtObj() const { return nullptr != m_pLookAtObj ? TRUE : FALSE; }
	const _bool			Is_LockX() const { return m_bLockX; }
	const _bool			Is_LockY() const { return m_bLockY; }
	const _bool&		Is_Lerp_Fov() const { return m_tLerpFov.bActive; }
	const _bool&		Is_Lerp_Dist() const { return m_tLerpDist.bActive; }
	const _bool&		Is_Shake() const { return m_tShakeDesc.bActive; }

public:
	void				Lerp_Fov(const _float fStartValue, const _float& fTargetValue, const _double& fTime, const LERP_MODE& eMode = LERP_MODE::DEFAULT);
	void				Lerp_Fov(const _float& fTargetValue, const _double& fTime, const LERP_MODE& eMode = LERP_MODE::DEFAULT);
	void				Lerp_Dist(const _float fStartValue, const _float& fTargetValue, const _double& fTime, const LERP_MODE& eMode = LERP_MODE::DEFAULT);
	void				Lerp_Dist(const _float& fTargetValue, const _double& fTime, const LERP_MODE& eMode = LERP_MODE::DEFAULT);

public:
	void				Change_TargetObj(CGameObject* pObj);
	void				Change_LookAtObj(CGameObject* pObj);

	void				Shake_Camera(const _float& fShakeTime, const _float& fIntensity);

private:
	void				Update_Lerp(const _double fDeltaTime);
	void				Update_Shake(const _double fDeltaTime);

private:
	PROJ_DESC			m_tProjDesc = {};

	CGameObject*		m_pTargetObj = { nullptr };
	CGameObject*		m_pLookAtObj = { nullptr };

	Vec4				m_vTargetOffset = {};
	Vec4				m_vLookAtOffset = {};

	_bool				m_bLockX = { FALSE };
	_bool				m_bLockY = { FALSE };

	_float				m_fDistance = { 0.f };

	_float				m_fMouseSensitiveX = { 1.f };
	_float				m_fMouseSensitiveY = { 1.f };

	_uint				m_iKey = { 99 };

	LERP_FLOAT_DESC		m_tLerpFov;
	LERP_FLOAT_DESC		m_tLerpDist;

	SHAKE_DESC			m_tShakeDesc;

public:
	static CCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END