#include "Camera_Manager.h"
#include "EngineInstance.h"
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

void CCamera_Manager::Tick(_double fTimeDelta)
{
	if (nullptr == m_pPipeLine || nullptr == m_pCurCamera)
		return;

	Matrix matWI;
	_float fFov;
	const CCamera::PROJ_DESC& desc = m_pCurCamera->Get_Camera()->Get_ProjDesc();

	/* WI, fov 설정 */
	if (m_bCameraChange) /* 카메라 변경 이벤트 */
	{
		m_tLerpTimeDesc.Update(fTimeDelta);
		m_tLerpFovDesc.Update(fTimeDelta);

		Lerp_Camera(fTimeDelta);

		matWI = m_matWILerp;
		fFov = m_tLerpFovDesc.fCurValue;
	}
	else
	{
		matWI = m_pCurCamera->Get_Transform()->Get_WorldMat().Invert();
		fFov = desc.fFovy;
	}

	/* 현재 카메라 오브젝트에서 필요한 상태변환(ex 월드 변환행렬)을 진행 한 후에 해당 Tick()으로 들어온다. */
	
	/* 파이프 라인에, 뷰 스페이스 변환 행렬(카메라 월드 변환 역행렬) 저장 */
	m_pPipeLine->Set_Transform(CPipeLine::STATE_VIEW, matWI);

	/* 파이프 라인에, 투영 변환 행렬 저장 */
	m_pPipeLine->Set_Transform(CPipeLine::STATE_PROJ, XMMatrixPerspectiveFovLH(fFov, desc.fAspect, desc.fNear, desc.fFar));
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

	auto iter = m_Cameras.find(iKey);

	if (iter == m_Cameras.end())
		return E_FAIL;

	for (auto& Pair : m_Cameras)
		Pair.second->Set_State(CGameObject::OBJ_STATE::STATE_UNACTIVE);

	m_pCurCamera = iter->second;

	m_pCurCamera->Set_State(CGameObject::OBJ_STATE::STATE_ACTIVE);

	return S_OK;
}

HRESULT CCamera_Manager::Add_Camera(const _uint& iKey, CGameObject* pCamera)
{
	if(nullptr == pCamera|| nullptr != Find_Camera(iKey))
		return E_FAIL;

	pCamera->Get_Camera()->Set_Key(iKey);

	m_Cameras.insert({ iKey, pCamera });

	return S_OK;
}

HRESULT CCamera_Manager::Change_Camera(const _uint& iKey, const _float& fLerpTime, const LERP_MODE& eLerpMode)
{
	auto iter = m_Cameras.find(iKey);

	if (iter == m_Cameras.end())
		return E_FAIL;

	m_pNextCamera = iter->second;

	if (m_pCurCamera != m_pNextCamera)
		m_bCameraChange = TRUE;

	m_pNextCamera->Set_State(CGameObject::STATE_ACTIVE);
	
	m_tLerpTimeDesc.Start(fLerpTime, eLerpMode);
	m_tLerpFovDesc.Start(m_pCurCamera->Get_Camera()->Get_ProjDesc().fFovy, m_pNextCamera->Get_Camera()->Get_ProjDesc().fFovy, fLerpTime, eLerpMode);

	return S_OK;
}

void CCamera_Manager::Shake_Camera(const _float& fShakeTime, const _int& iIntensity)
{
	if (nullptr == m_pCurCamera || m_pCurCamera->Get_Camera()->Is_Shake()) return;

	m_pCurCamera->Get_Camera()->Shake_Camera(fShakeTime, iIntensity);
}

CGameObject* CCamera_Manager::Find_Camera(const _uint& iKey)
{
	auto iter = m_Cameras.find(iKey);

	if(iter == m_Cameras.end())
		return nullptr;

	return iter->second;
}

void CCamera_Manager::Lerp_Camera(_double fTimeDelta)
{
	m_matWILerp = Matrix::Lerp(m_pCurCamera->Get_Transform()->Get_WorldMat().Invert(), 
								 m_pNextCamera->Get_Transform()->Get_WorldMat().Invert(), 
								 m_tLerpTimeDesc.fLerpTime);

	if (!m_tLerpTimeDesc.bActive)
	{
		m_matWILerp = m_pNextCamera->Get_Transform()->Get_WorldMat().Invert();

		m_bCameraChange = FALSE;

		m_pCurCamera->Set_State(CGameObject::STATE_UNACTIVE);

		m_pCurCamera = m_pNextCamera;
		m_pNextCamera = nullptr;
	}
}

void CCamera_Manager::Free()
{
	__super::Free();

	for (auto& Pair : m_Cameras)
	{
		Safe_Release(Pair.second);
	}
}
