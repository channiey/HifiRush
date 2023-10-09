#pragma once
#include "Base.h"

//#include "Collider.h"

union COLLIDER_ID
{
	struct
	{
		_uint iLeft_id;
		_uint iRight_id;
	};
	_ulonglong ID;
};

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
	void	Check_Collision_Layer(const wstring& strLayerTag1, const wstring& strLayerTag2,
									const CCollider::TYPE& eType1, const CCollider::TYPE& eType2);
	



public:
	/* Collision Ray */
	static const _bool Check_Collision_Ray(Ray& ray, class CCollider* pCollider, OUT RAYHIT_DESC& hitDesc);
	static const _bool Check_Collision_CameraRay(class CCollider* pCollider, const Matrix& matWorld, OUT RAYHIT_DESC& hitDesc);
	static const _bool Check_Collision_CameraRay(class CModel* pModel, const Matrix& matWorld, OUT RAYHIT_DESC& hitDesc, const _bool& bPreInterSphere);

public:
	/* Local */
	static Ray			Create_CameraRay(const Matrix& matWorld);

private:
	void				Set_Info(map<_ulonglong, _bool>::iterator& iter, class CCollider* pCollider1, class CCollider* pCollider2);

private:
	map<_ulonglong, _bool>	m_mapColInfo;		// 충돌체 간 이전 프레임 충돌 정보

public: 
	virtual void Free() override;
};

END

