#pragma once
#include "Base.h"

#include "Collider.h"

union COLLIDER_ID
{
	struct
	{
		_uint iLeft_ID;
		_uint iRight_ID;
	};
	_ulonglong ID;
};

BEGIN(Engine)

class CGameObject;
class CCell;

class CCollision_Manager final : public CBase
{
	DECLARE_SINGLETON(CCollision_Manager)

public:

private:
	CCollision_Manager();
	virtual ~CCollision_Manager() = default;

public:
	HRESULT Initialize();

public: 
	void				Check_Collision_Layer(const wstring& strLayerTag1, 
												const wstring& strLayerTag2,
												const CCollider::TYPE& eType1, 
												const CCollider::TYPE& eType2);
public:
	RAYHIT_DESC			Check_ScreenRay(const wstring& strLayerTag, const _bool& bSnap = FALSE);
	CCell*				Check_ScreenRay();

	const Ray			Create_ScreenRayLocal(Matrix matWorld);
	const Ray			Create_ScreenRayWorld();

private:
	void				Set_Info(map<_ulonglong, _bool>::iterator& iter, class CCollider* pCollider1, class CCollider* pCollider2);

private:
	map<_ulonglong, _bool>	m_mapColInfo;		// 충돌체 간 이전 프레임 충돌 정보

public: 
	virtual void Free() override;
};

END

