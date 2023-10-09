#pragma once
#include "Base.h"

BEGIN(Engine)

class CCollision_Manager final : public CBase
{
	DECLARE_SINGLETON(CCollision_Manager)

private:
	CCollision_Manager();
	virtual ~CCollision_Manager() = default;

public:
	HRESULT Initialize();

public: 
	/* Collision Collider */




	/* Collision Ray */
	static const _bool Check_Collision_Ray(Ray& ray, class CCollider* pCollider, OUT RAYHIT_DESC& hitDesc);
	static const _bool Check_Collision_CameraRay(class CCollider* pCollider, const Matrix& matWorld, OUT RAYHIT_DESC& hitDesc);
	static const _bool Check_Collision_CameraRay(class CModel* pModel, const Matrix& matWorld, OUT RAYHIT_DESC& hitDesc, const _bool& bPreInterSphere);

public:
	/* Local */
	static Ray Create_CameraRay(const Matrix& matWorld);

public: 
	virtual void Free() override;
};

END

