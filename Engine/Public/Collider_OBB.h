#pragma once

#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CCollider_OBB final : public CCollider
{
public:
	typedef struct tagOBBDesc
	{
		_float3		vCenter;
		_float3		vCenterAxis[3];
		_float3		vAlignAxis[3];
	}OBBDESC;
protected:
	CCollider_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider_OBB(const CCollider_OBB& rhs);
	virtual ~CCollider_OBB() = default;

public:
	BoundingOrientedBox Get_Collider() {
		return *m_pOBB;
	}

public:
	virtual HRESULT Initialize_Prototype(CCollider::TYPE eColliderType);
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(Matrix TransformMatrix);

public:
	virtual _bool Check_Collision(CCollider* pTargetCollider);
	virtual _bool Check_Collision(Ray& ray, OUT RAYHIT_DESC& pHitDesc);


public:
	_bool Collision_OBB(CCollider* pTargetCollider);

#ifdef _DEBUG
public:
	virtual HRESULT Render();
#endif // _DEBUG

private:
	BoundingOrientedBox*	m_pOBB = nullptr;
	BoundingOrientedBox*	m_pOriginal_OBB = nullptr;

private:
	OBBDESC Compute_OBBDesc();
	

public:
	static CCollider_OBB* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCollider::TYPE eColliderType);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;


};

END