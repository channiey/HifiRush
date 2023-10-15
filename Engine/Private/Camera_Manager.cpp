#include "Camera_Manager.h"
#include "GameInstance.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CCamera_Manager)

CCamera_Manager::CCamera_Manager()
	: m_pPipeLine(CPipeLine::GetInstance())
{
}

HRESULT CCamera_Manager::Initialize()
{
	return S_OK;
}

void CCamera_Manager::Tick()
{
	if (nullptr == m_pPipeLine || nullptr == m_pCurCamera)
		return;

	/* 현재 카메라 오브젝트에서 필요한 상태변환(ex 월드 변환행렬)을 진행 한 후에 해당 Tick()으로 들어온다. */

	/* 파이프 라인에, 뷰 스페이스 변환 행렬(카메라 월드 변환 역행렬) 저장 */
	m_pPipeLine->Set_Transform(CPipeLine::STATE_VIEW, m_pCurCamera->Get_Transform()->Get_WorldMat().Invert());

	/* 파이프 라인에, 투영 변환 행렬 저장 */
	const CCamera::PROJ_DESC& desc = m_pCurCamera->Get_Camera()->Get_ProjDesc();

	m_pPipeLine->Set_Transform(CPipeLine::STATE_PROJ, XMMatrixPerspectiveFovLH(desc.fFovy, desc.fAspect, desc.fNear, desc.fFar));
}

CGameObject* CCamera_Manager::Get_Camera(const _uint& iKey)
{
	if (m_Cameras.empty() || m_Cameras.size() <= iKey)
		return nullptr;

	return Find_Camera(iKey);
}

const Vec4 CCamera_Manager::Get_CurCamera_State(const _uint iState)
{
	if (nullptr == m_pCurCamera)
		return Vec4();

	switch (iState)
	{
	case Engine::CTransform::STATE_RIGHT:
		return m_pCurCamera->Get_Transform()->Get_Right();
		break;
	case Engine::CTransform::STATE_UP:
		return m_pCurCamera->Get_Transform()->Get_Up();
		break;
	case Engine::CTransform::STATE_LOOK:
		return m_pCurCamera->Get_Transform()->Get_Forward();
		break;
	default:
		return Vec4();
		break;
	}
	return Vec4();
}

HRESULT CCamera_Manager::Set_CurCamera(const _uint& iKey)
{
	if (m_Cameras.empty() || m_Cameras.size() <= iKey)
		return E_FAIL;

	m_pPreCamera = m_pCurCamera;

	m_pCurCamera = Find_Camera(iKey);

	return S_OK;
}

HRESULT CCamera_Manager::Add_Camera(const _uint& iKey, CGameObject* pCamera)
{
	if(nullptr == pCamera|| nullptr != Find_Camera(iKey))
		return E_FAIL;

	m_Cameras.insert({ iKey, pCamera });

	if (nullptr == m_pCurCamera)
		m_pCurCamera = pCamera;

	return S_OK;
}

CGameObject* CCamera_Manager::Find_Camera(const _uint& iKey)
{
	auto iter = m_Cameras.find(iKey);

	if(iter == m_Cameras.end())
		return nullptr;

	return iter->second;
}

void CCamera_Manager::Free()
{
	__super::Free();

	for (auto& Pair : m_Cameras)
	{
		Safe_Release(Pair.second);
	}
}
