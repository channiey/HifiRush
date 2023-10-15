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
	typedef struct tagCameraDesc 
	{
		_float	fFovy = 0.f, fAspect = 0.f, fNear = 0.f, fFar = 0.f;
	}CAMERA_DESC;

protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	CGameObject* Get_TargetObj() const { return m_pTargetObj; }
	CGameObject* Get_LookAtObj() const { return m_pLookAtObj; }
	const CAMERA_DESC& Get_CameraDesc() const { return m_tCameraDesc; }
	const Vec4& Get_TargetOffSet() const { return m_vTargetOffset; }
	const Vec4& Get_LookAtOffSet() const { return m_vLookAtOffset; }

public:
	void Set_TargetObj(CGameObject* pObj)	{ m_pTargetObj = pObj; }
	void Set_LookAtObj(CGameObject* pObj)	{ m_pLookAtObj = pObj; }
	void Set_TargetOffSet(const Vec4& vOffset)	{ m_vTargetOffset = vOffset; }
	void Set_LookAtOffSet(const Vec4& vOffset) { m_vLookAtOffset = vOffset; }


public:
	const _bool Is_TargetObj() const { return nullptr != m_pTargetObj ? TRUE : FALSE; }
	const _bool Is_LookAtObj() const { return nullptr != m_pLookAtObj ? TRUE : FALSE; }

public:
	void Change_Target(CGameObject* pObj);

private:
	CAMERA_DESC		m_tCameraDesc = {};

	CGameObject*	m_pTargetObj = { nullptr };
	CGameObject*	m_pLookAtObj = { nullptr };

	Vec4			m_vTargetOffset = {};
	Vec4			m_vLookAtOffset = {};

public:
	static CCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END