#include "Collision_Manager.h"

#include "GameInstance.h"

#include "GameObject.h"
#include "Model.h"
#include "Mesh.h"

#include "Collider_Sphere.h"
#include "Collider_AABB.h"
#include "Collider_OBB.h"

#include "NavMesh.h"
#include "Cell.h"

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
	/* ���̾� �������� */
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
			/* �ݶ��̴� �������� */
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

			if (nullptr == pCollider1 || nullptr == pCollider2 || pCollider1 == pCollider2
				|| !pCollider1->Is_Active() || !pCollider2->Is_Active()) continue;

			/* �浹 ���� ���� */
			Set_Info(iter, pCollider1, pCollider2);

			/* �浹 ���� �˻� */
			if (pCollider1->Check_Collision(pCollider2))
			{
				if (iter->second) // �������� �浹 
				{
					if (!pObj1->Is_Active() || !pObj2->Is_Active()) // �� �� �ϳ� ���� ����
					{
						pCollider1->OnCollision_Exit(pCollider2);
						pCollider2->OnCollision_Exit(pCollider1);
						iter->second = FALSE;
					}
					else // ���� ���� ����
					{
						pCollider1->OnCollision_Stay(pCollider2);
						pCollider2->OnCollision_Stay(pCollider1);
					}
				}
				else // �̹��� ó�� �浹 
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
				if (iter->second) // ������ �浹
				{
					pCollider1->OnCollision_Exit(pCollider2);
					pCollider2->OnCollision_Exit(pCollider1);
					iter->second = FALSE;
				}
			}
		}
	}
}

RAYHIT_DESC CCollision_Manager::Check_ScreenRay(const wstring& strLayerTag)
{
	list<class CGameObject*>* pGameObjects = GAME_INSTNACE->Get_Layer(GAME_INSTNACE->Get_CurLevelIndex(), strLayerTag);

	if(nullptr == pGameObjects) return RAYHIT_DESC();

	_float			fMinDist = 1000.f;
	_float			fDist = 0.f;
	RAYHIT_DESC		tHit;

	for (auto& pGameObject : *pGameObjects)
	{
		if (nullptr == pGameObject) continue;

		CModel* pModel = pGameObject->Get_Model();
		if (nullptr == pModel) continue;

		vector<class CMesh*>* pMeshes = pModel->Get_Meshes();
		if (nullptr == pMeshes) continue;

		Matrix matWorld = pGameObject->Get_Transform()->Get_WorldMat();

		const Ray ray = Create_ScreenRayLocal(matWorld);

		for (auto& pMesh : *pMeshes)
		{
			const _uint&		iPrimitives = pMesh->Get_NumPrimitives(); 
			Vec3*				VerticesPos = pMesh->Get_VerticesPos();
			FACEINDICES32*		Indices		= pMesh->Get_Indices();

			for (size_t i = 0; i < (size_t)iPrimitives; i++)
			{
				if (ray.Intersects(VerticesPos[Indices[i]._0], VerticesPos[Indices[i]._1], VerticesPos[Indices[i]._2], fDist))
				{
					if (fDist < fMinDist)
					{
						fMinDist = fDist;

						Vec3 rayPosition = XMVector3TransformCoord(ray.position, matWorld);
						Vec3 rayDirection = XMVector3TransformNormal(ray.direction, matWorld) * fMinDist;

						Vec3 vHitWorldPos = rayPosition + rayDirection;
						{
							if (vHitWorldPos.y < 0.f)
								vHitWorldPos.y = floorf(vHitWorldPos.y + 0.5f);
						}

						tHit.pGameObject = pGameObject;
						tHit.vHitPoint = vHitWorldPos;
						tHit.fDistance = rayDirection.Length();
					}
				}
			}
		}
	}

	return tHit;
}

CCell* CCollision_Manager::Check_ScreenRay()
{
	const vector<CCell*>& Cells = CNavMesh::GetInstance()->Get_Cells();

	const Ray ray = Create_ScreenRayWorld();

	CCell* pPickedCell = nullptr; 

	_float fMinDist = 1000.f;
	_float fDist = 0.f;

	for (auto& pCell : Cells)
	{
		const Vec3* Points = pCell->Get_Points();

		if (ray.Intersects(Points[CCell::POINT_A], Points[CCell::POINT_B], Points[CCell::POINT_C], fDist))
		{
			if (fDist < fMinDist)
			{
				fMinDist = fDist;

				pPickedCell = pCell;
			}
		}
	}
	
	return pPickedCell;
}

const Ray CCollision_Manager::Create_ScreenRayLocal(Matrix matWorld)
{
	const Matrix	matPI = GAME_INSTNACE->Get_Transform_Inverse(CPipeLine::STATE_PROJ);
	const Matrix	matVI = GAME_INSTNACE->Get_Transform_Inverse(CPipeLine::STATE_VIEW);
	const Matrix	matWI = matWorld.Invert();
	const Viewport	viewPort = GAME_INSTNACE->Get_ViewPort();

	POINT pt;
	{
		GetCursorPos(&pt);
		ScreenToClient(GAME_INSTNACE->Get_GraphicDesc().hWnd, &pt);
	}

	Vec3 vMousePos;
	{
		/* ����Ʈ -> ���� �����̽� */
		vMousePos.x = pt.x / (viewPort.width * 0.5f) - 1.f;
		vMousePos.y = pt.y / -(viewPort.height * 0.5f) + 1.f;
		vMousePos.z = 1.f;

		/* ���� �����̽� -> �� �����̽� */
		vMousePos = XMVector3TransformCoord(vMousePos, matPI);
	}

	Ray ray;
	{
		ray.direction = vMousePos - ray.position;

		/* �� �����̽� -> ���� �����̽� */
		ray.position = XMVector3TransformCoord(ray.position, matVI);
		ray.direction = XMVector3TransformNormal(ray.direction, matVI);

		/* ���� �����̽� -> ���� �����̽� */
		ray.position = XMVector3TransformCoord(ray.position, matWI);
		ray.direction = XMVector3TransformNormal(ray.direction, matWI);
		ray.direction.Normalize();
	}

	return ray;
}

const Ray CCollision_Manager::Create_ScreenRayWorld()
{
	const Matrix	matPI = GAME_INSTNACE->Get_Transform_Inverse(CPipeLine::STATE_PROJ);
	const Matrix	matVI = GAME_INSTNACE->Get_Transform_Inverse(CPipeLine::STATE_VIEW);
	const Viewport	viewPort = GAME_INSTNACE->Get_ViewPort();

	POINT pt;
	{
		GetCursorPos(&pt);
		ScreenToClient(GAME_INSTNACE->Get_GraphicDesc().hWnd, &pt);
	}

	Vec3 vMousePos;
	{
		/* ����Ʈ -> ���� �����̽� */
		vMousePos.x = pt.x / (viewPort.width * 0.5f) - 1.f;
		vMousePos.y = pt.y / -(viewPort.height * 0.5f) + 1.f;
		vMousePos.z = 1.f;

		/* ���� �����̽� -> �� �����̽� */
		vMousePos = XMVector3TransformCoord(vMousePos, matPI);
	}

	Ray ray;
	{
		ray.direction = vMousePos - ray.position;

		/* �� �����̽� -> ���� �����̽� */
		ray.position = XMVector3TransformCoord(ray.position, matVI);
		ray.direction = XMVector3TransformNormal(ray.direction, matVI);
		ray.direction.Normalize();
	}

	return ray;
}

void CCollision_Manager::Set_Info(map<_ulonglong, _bool>::iterator& iter, CCollider* pCollider1, CCollider* pCollider2)
{
	COLLIDER_ID ID;

	ID.iLeft_ID = pCollider1->Get_ID();
	ID.iRight_ID = pCollider2->Get_ID();

	// ������ �浹 �˻� ���� ����
	iter = m_mapColInfo.find(ID.ID);

	// ���ٸ� ���� �߰�
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
