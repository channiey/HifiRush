#pragma once

#include "Component.h"

/* ��� ī�޶󿡰� �ʿ��� ��� :  */
/* ī�޶��� ���� ����� �̿��Ͽ� �佺���̽� ��ȯ���.*/
/* �������(fov, aspect, near, far)�� �����ϴ��۾�.  */

BEGIN(Engine)

class ENGINE_DLL CCamera final : public CComponent
{
public:
	typedef struct tagCameraDesc 
	{
		Vec4	vEye, vAt;
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
	const CAMERA_DESC& Get_CameraDesc() const { return m_tCameraDesc; }

private:
	CAMERA_DESC m_tCameraDesc = {};

public:
	static CCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END