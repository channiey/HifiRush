#pragma once

#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CCollider_Sphere final : public CCollider
{
protected:
	CCollider_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider_Sphere(const CCollider_Sphere& rhs);
	virtual ~CCollider_Sphere() = default;

public:
	BoundingSphere Get_Collider() { return *m_pSphere; }

public:
	virtual HRESULT		Initialize_Prototype(CCollider::TYPE eColliderType);
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Update(Matrix TransformMatrix);

public:
	virtual _bool		Check_Collision(CCollider* pTargetCollider);
	//virtual _bool		Check_Collision(Ray& ray, OUT RAYHIT_DESC& pHitDesc);

public:
	virtual void		Set_Collider_Offset(const Vec3 vCenter) override;
	virtual void		Set_Collider_Size(const _float vSize) override;
	virtual void		Set_ColliderDesc(const COLLIDERDESC tDesc) override;


#ifdef _DEBUG
public:
	virtual HRESULT		Render();
#endif // _DEBUG

private:
	BoundingSphere*		m_pSphere = nullptr;
	BoundingSphere*		m_pOriginal_Sphere = nullptr;

public:
	static CCollider_Sphere* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CCollider::TYPE eColliderType);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END