#include "..\Public\Collider_OBB.h"
#include "Collider_AABB.h"
#include "Collider_Sphere.h"

CCollider_OBB::CCollider_OBB(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCollider(pDevice, pContext)
{

}

CCollider_OBB::CCollider_OBB(const CCollider_OBB & rhs)
	: CCollider(rhs)
{

}

HRESULT CCollider_OBB::Initialize_Prototype(CCollider::TYPE eColliderType)
{
	if (FAILED(__super::Initialize_Prototype(eColliderType)))
		return E_FAIL;

	m_eColliderType = CCollider::OBB;

	return S_OK;
}

HRESULT CCollider_OBB::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/* 복제될때 셋팅하고자하는 상태로 갱신한다. */
	m_pOriginal_OBB = new BoundingOrientedBox(m_ColliderDesc.vCenter, _float3(m_ColliderDesc.vSize.x * 0.5f,
		m_ColliderDesc.vSize.y * 0.5f,
		m_ColliderDesc.vSize.z * 0.5f), _float4(0.f, 0.f, 0.f, 1.f));

	/* 회전에 대한 상태. */
	_matrix		RotationMatrix = XMMatrixRotationX(m_ColliderDesc.vRotation.x) *
		XMMatrixRotationY(m_ColliderDesc.vRotation.y) *
		XMMatrixRotationZ(m_ColliderDesc.vRotation.z);

	m_pOriginal_OBB->Transform(*m_pOriginal_OBB, RotationMatrix);

	m_pOBB = new BoundingOrientedBox(*m_pOriginal_OBB);

	return S_OK;
}

void CCollider_OBB::Update(Matrix TransformMatrix)
{
	m_pOriginal_OBB->Transform(*m_pOBB, TransformMatrix);
}

_bool CCollider_OBB::Check_Collision(CCollider * pTargetCollider)
{
	CCollider::TYPE		eType = pTargetCollider->Get_Type();

	_bool bCollision = FALSE;

	switch (eType)
	{
	case CCollider::AABB:
		bCollision = m_pOBB->Intersects(((CCollider_AABB*)pTargetCollider)->Get_Collider());
		break;

	case CCollider::OBB:
		bCollision = m_pOBB->Intersects(((CCollider_OBB*)pTargetCollider)->Get_Collider());
		break;

	case CCollider::SPHERE:
		bCollision = m_pOBB->Intersects(((CCollider_Sphere*)pTargetCollider)->Get_Collider());
		break;
	}

	return bCollision;
}

//_bool CCollider_OBB::Check_Collision(Ray& ray, OUT RAYHIT_DESC& pHitDesc)
//{
//	return m_pOBB->Intersects(ray.position, ray.direction, pHitDesc.fDistance);
//}

_bool CCollider_OBB::Collision_OBB(CCollider * pTargetCollider)
{
	if (CCollider::SPHERE == pTargetCollider->Get_Type())
		return FALSE;

	_bool bCollision = FALSE;

	OBBDESC			OBBDesc[2] = {
		Compute_OBBDesc(),
		((CCollider_OBB*)pTargetCollider)->Compute_OBBDesc(),
	};

	for (_uint i = 0; i < 2; ++i)
	{	
		for (_uint j = 0; j < 3; ++j)
		{
			_vector			vCenterDir = XMLoadFloat3(&OBBDesc[1].vCenter) - XMLoadFloat3(&OBBDesc[0].vCenter);

			_float			fDistance[3] = { 0.0f };
			fDistance[0] = fabs(XMVectorGetX(XMVector3Dot(vCenterDir, XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));
	
			fDistance[1] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[0]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[1]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterAxis[2]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			fDistance[2] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[0]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[1]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterAxis[2]), XMLoadFloat3(&OBBDesc[i].vAlignAxis[j]))));

			if (fDistance[0] > fDistance[1] + fDistance[2])
				return bCollision;

		}
	}

	bCollision = true;

	return bCollision;
}

HRESULT CCollider_OBB::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pBatch->Begin();

	DX::Draw(m_pBatch, *m_pOBB, XMLoadFloat4(&m_vColor));

	m_pBatch->End();

	return S_OK;
}

CCollider_OBB::OBBDESC CCollider_OBB::Compute_OBBDesc()
{
	OBBDESC			OBBDesc;

	_float3			vPoints[8];

	m_pOBB->GetCorners(vPoints);

	XMStoreFloat3(&OBBDesc.vCenter, (XMLoadFloat3(&vPoints[2]) + XMLoadFloat3(&vPoints[4])) * 0.5f);

	XMStoreFloat3(&OBBDesc.vCenterAxis[0], (XMLoadFloat3(&vPoints[2]) + XMLoadFloat3(&vPoints[5])) * 0.5f - 
		XMLoadFloat3(&OBBDesc.vCenter));
	XMStoreFloat3(&OBBDesc.vCenterAxis[1], (XMLoadFloat3(&vPoints[2]) + XMLoadFloat3(&vPoints[7])) * 0.5f -
		XMLoadFloat3(&OBBDesc.vCenter));
	XMStoreFloat3(&OBBDesc.vCenterAxis[2], (XMLoadFloat3(&vPoints[2]) + XMLoadFloat3(&vPoints[0])) * 0.5f -
		XMLoadFloat3(&OBBDesc.vCenter));

	XMStoreFloat3(&OBBDesc.vAlignAxis[0], XMVector3Normalize(XMLoadFloat3(&OBBDesc.vCenterAxis[0])));
	XMStoreFloat3(&OBBDesc.vAlignAxis[1], XMVector3Normalize(XMLoadFloat3(&OBBDesc.vCenterAxis[1])));
	XMStoreFloat3(&OBBDesc.vAlignAxis[2], XMVector3Normalize(XMLoadFloat3(&OBBDesc.vCenterAxis[2])));

	return OBBDesc;
}

CCollider_OBB * CCollider_OBB::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CCollider::TYPE eColliderType)
{
	CCollider_OBB*			pInstance = new CCollider_OBB(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eColliderType)))
	{
		MSG_BOX("Failed To Created : CCollider_OBB");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CCollider_OBB::Clone(void * pArg)
{
	CCollider_OBB*			pInstance = new CCollider_OBB(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CCollider_OBB");
		Safe_Release(pInstance);
	}
	return pInstance;
}


void CCollider_OBB::Free()
{
	__super::Free();

	Safe_Delete(m_pOriginal_OBB);
	Safe_Delete(m_pOBB);

}
