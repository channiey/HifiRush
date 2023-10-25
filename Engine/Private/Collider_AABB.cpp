#include "..\Public\Collider_AABB.h"
#include "PipeLine.h"

#include "Collider_OBB.h"
#include "Collider_Sphere.h"

CCollider_AABB::CCollider_AABB(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollider(pDevice, pContext)
{

}

CCollider_AABB::CCollider_AABB(const CCollider_AABB & rhs)
	: CCollider(rhs)
	, m_pAABB(rhs.m_pAABB)
{

}

HRESULT CCollider_AABB::Initialize_Prototype(CCollider::TYPE eColliderType)
{
	if (FAILED(__super::Initialize_Prototype(eColliderType)))
		return E_FAIL;
	m_eColliderType = CCollider::AABB;
	return S_OK;
}

HRESULT CCollider_AABB::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;



	/* 복제될때 셋팅하고자하는 상태로 갱신한다. */
	m_pOriginal_AABB = new BoundingBox(m_ColliderDesc.vCenter, _float3(m_ColliderDesc.vSize.x * 0.5f,
		m_ColliderDesc.vSize.y * 0.5f, 
		m_ColliderDesc.vSize.z * 0.5f));

	m_pAABB = new BoundingBox(*m_pOriginal_AABB);

	return S_OK;
}

void CCollider_AABB::Update(Matrix TransformMatrix)
{
	m_pOriginal_AABB->Transform(*m_pAABB, Remove_Rotation(TransformMatrix));
}

_bool CCollider_AABB::Check_Collision(CCollider * pTargetCollider)
{
	CCollider::TYPE		eType = pTargetCollider->Get_Type();

	_bool bCollision = FALSE;

	switch (eType)
	{
	case CCollider::AABB:
		bCollision = m_pAABB->Intersects(((CCollider_AABB*)pTargetCollider)->Get_Collider());
		break;

	case CCollider::OBB:
		bCollision = m_pAABB->Intersects(((CCollider_OBB*)pTargetCollider)->Get_Collider());
		break;

	case CCollider::SPHERE:
		bCollision = m_pAABB->Intersects(((CCollider_Sphere*)pTargetCollider)->Get_Collider());
		break;
	}	

	return bCollision;
}

//_bool CCollider_AABB::Check_Collision(Ray& ray, OUT RAYHIT_DESC& pHitDesc)
//{
//	return m_pAABB->Intersects(ray.position, ray.direction, pHitDesc.fDistance);
//}

_bool CCollider_AABB::Collision_AABB(CCollider * pTargetCollider)
{
	if (CCollider::AABB != pTargetCollider->Get_Type())
		return FALSE;

	CCollider_AABB*		pTargetAABB = (CCollider_AABB*)pTargetCollider;

	_float3		vSourMin, vSourMax;
	_float3		vDestMin, vDestMax;

	vSourMin = Compute_Min();
	vSourMax = Compute_Max();

	vDestMin = pTargetAABB->Compute_Min();
	vDestMax = pTargetAABB->Compute_Max();

	_bool bCollision = FALSE;

	/* 너비로 겹쳤는지?! */
	if (max(vSourMin.x, vDestMin.x) > min(vSourMax.x, vDestMax.x))
		return bCollision;

	/* 높이로 */
	if (max(vSourMin.y, vDestMin.y) > min(vSourMax.y, vDestMax.y))
		return bCollision;

	/* 깊이로 */
	if (max(vSourMin.z, vDestMin.z) > min(vSourMax.z, vDestMax.z))
		return bCollision;

	bCollision = true;

	return bCollision;
}

#ifdef _DEBUG

HRESULT CCollider_AABB::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pBatch->Begin();
	
	DX::Draw(m_pBatch, *m_pAABB, XMLoadFloat4(&m_vColor));

	m_pBatch->End();

	return S_OK;
}

#endif // _DEBUG

_float3 CCollider_AABB::Compute_Min()
{
	return _float3(m_pAABB->Center.x - m_pAABB->Extents.x, 
		m_pAABB->Center.y - m_pAABB->Extents.y, 
		m_pAABB->Center.z - m_pAABB->Extents.z);
}

_float3 CCollider_AABB::Compute_Max()
{
	return _float3(m_pAABB->Center.x + m_pAABB->Extents.x,
		m_pAABB->Center.y + m_pAABB->Extents.y,
		m_pAABB->Center.z + m_pAABB->Extents.z);
}

CCollider_AABB * CCollider_AABB::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CCollider::TYPE eColliderType)
{
	CCollider_AABB*			pInstance = new CCollider_AABB(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eColliderType)))
	{
		MSG_BOX("Failed To Created : CCollider_AABB");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CCollider_AABB::Clone(void * pArg)
{
	CCollider_AABB*			pInstance = new CCollider_AABB(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CCollider_AABB");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CCollider_AABB::Free()
{
	__super::Free();

	Safe_Delete(m_pOriginal_AABB);
	Safe_Delete(m_pAABB);
}
