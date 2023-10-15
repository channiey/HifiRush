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
		_float	fFovy = 0.f, fAspect = 0.f, fNear = 0.f, fFar = 0.f;
	}PROJ_DESC;

protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	CGameObject*		Get_TargetObj() const { return m_pTargetObj; }
	CGameObject*		Get_LookAtObj() const { return m_pLookAtObj; }
	const Vec4&			Get_TargetOffSet() const { return m_vTargetOffset; }
	const Vec4&			Get_LookAtOffSet() const { return m_vLookAtOffset; }
	
	const _float&		Get_Distance() const { return m_fDistance; }
	const _float&		Get_MouseSensitiveX() const { return m_fMouseSensitiveX; }
	const _float&		Get_MouseSensitiveY() const { return m_fMouseSensitiveY; }

	const PROJ_DESC&	Get_ProjDesc() const { return m_tProjDesc; }

public:
	void Set_TargetObj(CGameObject* pObj)	{ m_pTargetObj = pObj; }
	void Set_LookAtObj(CGameObject* pObj)	{ m_pLookAtObj = pObj; }
	void Set_TargetOffSet(const Vec4& vOffset)	{ m_vTargetOffset = vOffset; }
	void Set_LookAtOffSet(const Vec4& vOffset) { m_vLookAtOffset = vOffset; }
	
	void Set_Distance(const _float& fDist) { m_fDistance = fDist; }
	void Set_MouseSensitiveX(const _float& fValue) { m_fMouseSensitiveX = fValue; }
	void Set_MouseSensitiveY(const _float& fValue) { m_fMouseSensitiveY = fValue; }

	void Set_LockX(const _bool& bLock) { m_bLockX = bLock; }
	void Set_LockY(const _bool& bLock) { m_bLockY = bLock; }
	void Set_Fovy(const _float& fFovy) { m_tProjDesc.fFovy = fFovy; }

public:
	const _bool Is_TargetObj() const { return nullptr != m_pTargetObj ? TRUE : FALSE; }
	const _bool Is_LookAtObj() const { return nullptr != m_pLookAtObj ? TRUE : FALSE; }
	const _bool Is_LockX() const { return m_bLockX; }
	const _bool Is_LockY() const { return m_bLockY; }

public:
	void Change_TargetObj(CGameObject* pObj);
	void Change_LookAtObj(CGameObject* pObj);

private:
	PROJ_DESC		m_tProjDesc = {};

	CGameObject*	m_pTargetObj = { nullptr };
	CGameObject*	m_pLookAtObj = { nullptr };

	Vec4			m_vTargetOffset = {};
	Vec4			m_vLookAtOffset = {};

	_bool			m_bLockX = FALSE;
	_bool			m_bLockY = FALSE;

	_float			m_fDistance = { 0.f };
	_float			m_fMouseSensitiveX = { 1.f };
	_float			m_fMouseSensitiveY = { 1.f };

public:
	static CCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END