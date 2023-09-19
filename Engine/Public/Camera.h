#pragma once

#include "GameObject.h"
#include "Transform.h"

/* Ŭ���̾�Ʈ���� ���� ī�޶� ���� �θ� �ȴ�. */
/* ��� ī�޶󿡰� �ʿ��� ����� ���������� �����Ͽ� �ڽĿ��� ��ӳ����ֱ����ؼ�. */

/* ��� ī�޶󿡰� �ʿ��� ��� :  */
/* ī�޶��� ���� ����� �̿��Ͽ� �佺���̽� ��ȯ���.*/
/* �������(fov, aspect, near, far)�� �����ϴ��۾�.  */

BEGIN(Engine)

class ENGINE_DLL CCamera abstract : public CGameObject
{

public:
	typedef struct tagCameraDesc 
	{
		Vec4	vEye, vAt;
		_float	fFovy, fAspect, fNear, fFar;

	} CAMERA_DESC;

protected:
	CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

protected:
	CAMERA_DESC m_tCamDesc = {};

protected:
	class CTransform*		m_pTransformCom = { nullptr };
	class CPipeLine*		m_pPipeLine = { nullptr };

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END