#include "Collision_Manager.h"

#include "GameInstance.h"

#include "GameObject.h"
#include "Model.h"
#include "Mesh.h"

#include "Collider_Sphere.h"
#include "Collider_AABB.h"
#include "Collider_OBB.h"

IMPLEMENT_SINGLETON(CCollision_Manager)

CCollision_Manager::CCollision_Manager()
{
	
}

HRESULT CCollision_Manager::Initialize()
{
	return S_OK;
}

void CCollision_Manager::Check_Collision_Layer(const wstring& strLayerTag1, const wstring& strLayerTag2, const CCollider::TYPE& eType1, const CCollider::TYPE& eType2)
{
	/* 레이어 가져오기 */
	_uint iCurLevel = CGameInstance::GetInstance()->Get_CurLevelIndex();

	list<CGameObject*>* pLayer1 = CGameInstance::GetInstance()->Get_Layer(iCurLevel, strLayerTag1);
	list<CGameObject*>* pLayer2 = CGameInstance::GetInstance()->Get_Layer(iCurLevel, strLayerTag2);

	if (nullptr == pLayer1 || nullptr == pLayer2 || pLayer1->empty() || pLayer2->empty()) return;

	map<_ulonglong, _bool>::iterator	iter;

	/* Loop */
	for (auto& pObj1 : *pLayer1)
	{
		for (auto& pObj2 : *pLayer2)
		{
			/* 콜라이더 가져오기 */
			if (nullptr == pObj1 || nullptr == pObj2 || pObj1 == pObj2) continue;

			CCollider* pCollider1 = nullptr;
			CCollider* pCollider2 = nullptr;

			switch (eType1)
			{
			case Engine::CCollider::AABB:
				pCollider1 = pObj1->Get_Collider_AABB();
				break;
			case Engine::CCollider::OBB:
				pCollider1 = pObj1->Get_Collider_OBB();
				break;
			case Engine::CCollider::SPHERE:
				pCollider1 = pObj1->Get_Collider_Sphere();
				break;
			default:
				break;
			}
			switch (eType2)
			{
			case Engine::CCollider::AABB:
				pCollider2 = pObj2->Get_Collider_AABB();
				break;
			case Engine::CCollider::OBB:
				pCollider2 = pObj2->Get_Collider_OBB();
				break;
			case Engine::CCollider::SPHERE:
				pCollider2 = pObj2->Get_Collider_Sphere();
				break;
			default:
				break;
			}

			if (nullptr == pCollider1 || nullptr == pCollider2 || pCollider1 == pCollider2) continue;

			/* 충돌 정보 세팅 */
			Set_Info(iter, pCollider1, pCollider2);

			/* 충돌 여부 검사 */
			if (pCollider1->Check_Collision(pCollider2))
			{
				if (iter->second) // 이전에도 충돌 
				{
					if (!pObj1->Is_Active() || !pObj2->Is_Active()) // 둘 중 하나 삭제 예정
					{
						pCollider1->OnCollision_Exit(pCollider2);
						pCollider2->OnCollision_Exit(pCollider1);
						iter->second = FALSE;
					}
					else // 삭제 예정 없음
					{
						pCollider1->OnCollision_Stay(pCollider2);
						pCollider2->OnCollision_Stay(pCollider1);
					}
				}
				else // 이번에 처음 충돌 
				{
					if (pObj1->Is_Active() && pObj2->Is_Active())
					{
						pCollider1->OnCollision_Enter(pCollider2);
						pCollider2->OnCollision_Enter(pCollider1);
						iter->second = TRUE;
					}
				}
			}
			else
			{
				if (iter->second) // 이전에 충돌
				{
					pCollider1->OnCollision_Exit(pCollider2);
					pCollider2->OnCollision_Exit(pCollider1);
					iter->second = FALSE;
				}
			}
		}
	}
}

const _bool CCollision_Manager::Check_Collision_Ray(Ray& ray, CCollider* pCollider, OUT RAYHIT_DESC& hitDesc)
{
	if (nullptr == pCollider)
		return FALSE;

	return pCollider->Check_Collision(ray, hitDesc);
}

const _bool CCollision_Manager::Check_Collision_PickingRay(CCollider* pCollider, const Matrix& matWorld, OUT RAYHIT_DESC& hitDesc)
{
	if (nullptr == pCollider) return FALSE;

	Ray ray = Create_PickingRay(matWorld);
	if (ray == Ray()) return FALSE;

	switch (pCollider->Get_Type())
	{
	case Engine::CCollider::AABB:
	{
		CCollider_AABB* pAABBCollider = dynamic_cast<CCollider_AABB*>(pCollider);

		if (nullptr == pAABBCollider)
			return FALSE;

		if (pAABBCollider->Check_Collision(ray, hitDesc))
		{
			hitDesc.pGameObject = (void*)pAABBCollider->Get_Owner();
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
		break;
	case Engine::CCollider::OBB:
	{
		CCollider_OBB* pOBBCollider = dynamic_cast<CCollider_OBB*>(pCollider);

		if (nullptr == pCollider)
			return FALSE;

		if (pOBBCollider->Check_Collision(ray, hitDesc))
		{
			hitDesc.pGameObject = (void*)pOBBCollider->Get_Owner();
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
		break;
	case Engine::CCollider::SPHERE:
	{
		CCollider_Sphere* pSphereCollider = dynamic_cast<CCollider_Sphere*>(pCollider);

		if (nullptr == pSphereCollider)
			return FALSE;

		if (pSphereCollider->Check_Collision(ray, hitDesc))
		{
			hitDesc.pGameObject = (void*)pSphereCollider->Get_Owner();
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
		break;
	default:
		break;
	}


	return FALSE;
}

const _bool CCollision_Manager::Check_Collision_PickingRay(class CModel* pModel, const Matrix& matWorld, OUT RAYHIT_DESC& hitDesc, const _bool& bPreInterSphere)
{
	if (nullptr == pModel || !CGameInstance::GetInstance()->Is_Focus()) return FALSE;

	Ray ray = Create_PickingRay2(matWorld);
	if (ray == Ray()) return FALSE;

	/* 메시 콜라이더 충돌 검사전, 스피어 콜라이더 충돌 검사 우선 진행 (최적화)*/
	if (bPreInterSphere)
	{
		if (!Check_Collision_PickingRay(pModel->Get_Owner()->Get_Collider_Sphere(), matWorld, hitDesc))
			return FALSE;
		else
			ZeroMemory(&hitDesc, sizeof(RAYHIT_DESC));
		cout << "Sphere\n";
	}

	/* 메시 콜라이더 충돌 검사 */
	vector<CMesh*>* pMeshes = pModel->Get_Meshes();
	if (nullptr == pMeshes) return FALSE;
	
	for (auto& pMesh : *pMeshes)
	{
		Vec3*	pVerticesPos = pMesh->Get_VerticesPos();
		FACEINDICES32* pIndices = pMesh->Get_Indices();

		if (nullptr == pVerticesPos || nullptr == pIndices) continue;

		for (size_t i = 0; i < pMesh->Get_NumPrimitives(); ++i)
		{
			if (ray.Intersects(pVerticesPos[pIndices[i]._0], pVerticesPos[pIndices[i]._1], pVerticesPos[pIndices[i]._2], hitDesc.fDistance))
			{
				int k = 0;
				cout << "!@# Mesh !@# \n";
			}
		}
	}
	
	return TRUE;
}

Ray CCollision_Manager::Create_PickingRay(const Matrix& matWorld)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(CGameInstance::GetInstance()->Get_GraphicDesc().hWnd, &pt);

	Matrix matP = CGameInstance::GetInstance()->Get_Transform(CPipeLine::STATE_PROJ);

	// Compute picking ray in view space.
	float vx = (+2.0f * pt.x / CGameInstance::GetInstance()->Get_GraphicDesc().iWinSizeX - 1.0f) / matP(0, 0); // P(0, 0);
	float vy = (-2.0f * pt.y / CGameInstance::GetInstance()->Get_GraphicDesc().iWinSizeY + 1.0f) / matP(1, 1); // P(1, 1);

	// Ray definition in view space.
	XMVECTOR rayOrigin = ::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rayDir = ::XMVectorSet(vx, vy, 1.0f, 0.0f);

	// Tranform ray to local space of Mesh.
	Matrix matVI = CGameInstance::GetInstance()->Get_Transform_Inverse(CPipeLine::STATE_VIEW);
	Matrix matWI = matWorld.Invert();
	Matrix matToLocal = XMMatrixMultiply(matVI, matWI);

	rayOrigin = ::XMVector3TransformCoord(rayOrigin, matToLocal);
	rayDir = ::XMVector3TransformNormal(rayDir, matToLocal);

	// Make the ray direction unit length for the intersection tests.
	rayDir = ::XMVector3Normalize(rayDir);

	return Ray(rayOrigin, rayDir);

	///* Unprojection */
	//const Matrix matW = matWorld;
	//const Matrix matV = CGameInstance::GetInstance()->Get_Transform(CPipeLine::STATE_VIEW);
	//const Matrix matP = CGameInstance::GetInstance()->Get_Transform(CPipeLine::STATE_PROJ);
	//const Viewport viewPort = CGameInstance::GetInstance()->Get_ViewPort();

	//Vec2 vPickWindowPos;
	//if (!CGameInstance::GetInstance()->Get_PickPos_Window(&vPickWindowPos))
	//	return Ray();

	//Vec3 n = viewPort.Unproject(Vec3(vPickWindowPos.x, vPickWindowPos.y, 0.f), matP, matV, matW);
	//Vec3 f = viewPort.Unproject(Vec3(vPickWindowPos.x, vPickWindowPos.y, 1.f), matP, matV, matW);

	///* Create Ray */
	//Vec3 vOrigin = n;
	//Vec3 vDir = f - n;
	//vDir.Normalize();

	//return Ray(vOrigin, vDir);
}

Ray CCollision_Manager::Create_PickingRay2(const Matrix& matWorld)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(CGameInstance::GetInstance()->Get_GraphicDesc().hWnd, &pt);

	_vector vMousePos = XMVectorSet(
		_float(pt.x / (CGameInstance::GetInstance()->Get_GraphicDesc().iWinSizeX * 0.5f) - 1.f),
		_float(pt.y / (CGameInstance::GetInstance()->Get_GraphicDesc().iWinSizeY * -0.5f) + 1.f),
		0.f, 0.f);

	Matrix matPI = CGameInstance::GetInstance()->Get_Transform_Inverse(CPipeLine::STATE_PROJ);
	Matrix matVI = CGameInstance::GetInstance()->Get_Transform_Inverse(CPipeLine::STATE_VIEW);
	
	vMousePos = XMVector3TransformCoord(vMousePos, matPI);

	XMVECTOR vRayDir, vRayPosition;

	vRayPosition = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	vRayDir = vMousePos - vRayPosition;

	vRayPosition = XMVector3TransformCoord(vRayPosition, matVI);
	vRayDir = XMVector3TransformNormal(vRayDir, matVI);
	
	vRayPosition = XMVector3TransformCoord(vMousePos, matWorld.Invert());
	vRayDir = XMVector3TransformNormal(vRayDir, matWorld.Invert());

	vRayDir = XMVector3Normalize(vRayDir);

	Ray ray(vRayPosition, vRayDir);

	return ray;
}

void CCollision_Manager::Set_Info(map<_ulonglong, _bool>::iterator& iter, CCollider* pCollider1, CCollider* pCollider2)
{
	COLLIDER_ID ID;

	ID.iLeft_ID = pCollider1->Get_ID();
	ID.iRight_ID = pCollider2->Get_ID();

	// 이전에 충돌 검사 여부 조사
	iter = m_mapColInfo.find(ID.ID);

	// 없다면 새로 추가
	if (m_mapColInfo.end() == iter)
	{
		m_mapColInfo.insert({ ID.ID, FALSE });
		iter = m_mapColInfo.find(ID.ID);
	}
}

void CCollision_Manager::Free()
{
	__super::Free();
}
