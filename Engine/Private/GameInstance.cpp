#include "..\Public\GameInstance.h"
#include "Timer_Manager.h"
#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Collision_Manager.h"
#include "Profiler_Manager.h"
#include "Camera_Manager.h"
#include "GameObject.h"
#include "NavMesh.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pTimer_Manager(CTimer_Manager::GetInstance())
	, m_pInput_Device(CInput_Device::GetInstance())
	, m_pGraphic_Device(CGraphic_Device::GetInstance())
	, m_pLevel_Manager(CLevel_Manager::GetInstance())
	, m_pObject_Manager(CObject_Manager::GetInstance())
	, m_pComponent_Manager(CComponent_Manager::GetInstance())
	, m_pProfiler_Manager(CProfiler_Manager::GetInstance())
	, m_pPipeLine(CPipeLine::GetInstance())
	, m_pCollision_Manager(CCollision_Manager::GetInstance())
	, m_pCamera_Manager(CCamera_Manager::GetInstance())
	, m_pNavMesh(CNavMesh::GetInstance())

{
	Safe_AddRef(m_pPipeLine);
	Safe_AddRef(m_pComponent_Manager);
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pLevel_Manager);
	Safe_AddRef(m_pGraphic_Device);
	Safe_AddRef(m_pTimer_Manager);
	Safe_AddRef(m_pInput_Device);
	Safe_AddRef(m_pProfiler_Manager);
	Safe_AddRef(m_pCollision_Manager);
	Safe_AddRef(m_pCamera_Manager);
	Safe_AddRef(m_pNavMesh);
}

HRESULT CGameInstance::Initialize_Engine(_uint iNumLevels, HINSTANCE hInst, const GRAPHIC_DESC& GraphicDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext)
{
	/* 그래픽디바이스 초기화 처리. */
	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(GraphicDesc, ppDevice, ppContext)))
		return E_FAIL;

	m_pDevice = *ppDevice;
	m_pContext = *ppContext;

	/* 사운드디바이스 초기화 처리. */

	/* 입력디바이스 초기화 처리. */
	if (FAILED(m_pInput_Device->Initialize(hInst, m_pGraphic_Device->Get_GraphicDesc().hWnd)))
		return E_FAIL;

	/* 오브젝트 매니저의 예약 처리. */
	if (FAILED(m_pObject_Manager->Reserve_Manager(iNumLevels)))
		return E_FAIL;

	/* 컴포넌트 매니저의 예약 처리 */
	if (FAILED(m_pComponent_Manager->Reserve_Manager(iNumLevels)))
		return E_FAIL;

	/* 파이프 라인의 초기화 처리 */
	if (FAILED(m_pPipeLine->Initialize()))
		return E_FAIL;

	/* 콜리전 매니저의 초기화 처리 */
	if (FAILED(m_pCollision_Manager->Initialize()))
		return E_FAIL;

	/* 카메라 매니저의 초기화 처리 */
	if (FAILED(m_pCamera_Manager->Initialize()))
		return E_FAIL;

	if (FAILED(m_pNavMesh->Initialize(m_pDevice, m_pContext)))
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Tick(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager)
		return;

	if (nullptr == m_pObject_Manager)
		return;

	if (nullptr == m_pInput_Device)
		return;

	if (nullptr == m_pPipeLine)
		return;

	m_pInput_Device->Tick();

	m_pObject_Manager->Tick(fTimeDelta);
	m_pLevel_Manager->Tick(fTimeDelta);

	/* 카메라 오브젝트 월드행렬 갱신 -> 뷰행령 투영행렬 저장 -> 계산*/
	m_pCamera_Manager->Tick();
	m_pPipeLine->Tick();
}

void CGameInstance::LateTick(_float fTimeDelta)
{
	m_pLevel_Manager->LateTick(fTimeDelta);
	m_pObject_Manager->LateTick(fTimeDelta);
}

void CGameInstance::FinishTick()
{
	m_pObject_Manager->FinishTick();
	m_pProfiler_Manager->FinishTick();
}

void CGameInstance::Clear(_uint iLevelIndex)
{
	m_pObject_Manager->Clear(iLevelIndex);
}

_float CGameInstance::Compute_TimeDelta(const wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.f;

	return m_pTimer_Manager->Compute_TimeDelta(strTimerTag);	
}

HRESULT CGameInstance::Add_Timer(const wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Present();
}

ID3D11Device* CGameInstance::Get_Device()
{
	if (nullptr == m_pGraphic_Device)
		return nullptr;

	return m_pGraphic_Device->Get_Device();
}

GRAPHIC_DESC CGameInstance::Get_GraphicDesc()
{
	if (nullptr == m_pGraphic_Device)
		return GRAPHIC_DESC{};

	return m_pGraphic_Device->Get_GraphicDesc();
}

const Viewport CGameInstance::Get_ViewPort()
{
	if (nullptr == m_pGraphic_Device)
		return Viewport{};

	return m_pGraphic_Device->Get_ViewPort();
}

HRESULT CGameInstance::Open_Level(_uint iLevelIndex, CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iLevelIndex, pNewLevel);
}

const _uint CGameInstance::Get_CurLevelIndex()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Get_CurLevelIndex();
}

HRESULT CGameInstance::Add_Prototype(const wstring & strPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(strPrototypeTag, pPrototype);	
}

class CGameObject* CGameInstance::Add_GameObject(const _uint iLevelIndex, const wstring & strLayerTag, const wstring & strPrototypeTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Add_GameObject(iLevelIndex, strLayerTag, strPrototypeTag, pArg);
}

HRESULT CGameInstance::Delete_GameObject(const _uint iLevelIndex, CGameObject* pObj)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Delete_GameObject(iLevelIndex, pObj);
}
 
HRESULT CGameInstance::Reserve_Pool(const _uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, const _uint& iNumObj, void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Reserve_Pool(iLevelIndex, strLayerTag, strPrototypeTag, iNumObj, pArg);
}

CGameObject* CGameInstance::Pop_Pool(const _uint iLevelIndex, const wstring& strPrototypeTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Pop_Pool(iLevelIndex, strPrototypeTag);
}

HRESULT CGameInstance::Return_Pool(const _uint iLevelIndex, CGameObject* pObj)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Return_Pool(iLevelIndex, pObj);
}

HRESULT CGameInstance::Add_Layer(_uint iLevelIndex, const wstring& strLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Layer(iLevelIndex, strLayerTag);
}

HRESULT CGameInstance::Delete_Layer(_uint iLevelIndex, const wstring& strLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Delete_Layer(iLevelIndex, strLayerTag);
}

map<const wstring, class CLayer*>* CGameInstance::Get_All_Layer(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_All_Layer(iLevelIndex);
}

map<const wstring, class CLayer*>* CGameInstance::Get_All_Layer_CurLevel()
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_All_Layer(Get_CurLevelIndex());
}

list<class CGameObject*>* CGameInstance::Get_Layer(_uint iLevelIndex, const wstring& strLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Layer(iLevelIndex, strLayerTag);
}

CLayer* CGameInstance::Get_LayerClass(_uint iLevelIndex, const wstring& strLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_LayerClass(iLevelIndex, strLayerTag);
}

CGameObject* CGameInstance::Get_Player()
{
	return nullptr;
}

CGameObject* CGameInstance::Get_GameObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_GameObject(iLevelIndex, strLayerTag, strPrototypeTag);
}

CGameObject* CGameInstance::Get_GameObject_InCurLevel(const wstring& strLayerTag, const wstring& strPrototypeTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_GameObject(Get_CurLevelIndex(), strLayerTag, strPrototypeTag);
}

CGameObject* CGameInstance::Get_GameObject_InCurLevel_InLayerFirst(const wstring& strLayerTag)
{
	list<class CGameObject*>* pLayer = Get_Layer(Get_CurLevelIndex(), strLayerTag);
	if (nullptr == pLayer || pLayer->empty())
		return nullptr;

	return pLayer->front();
}

const _int CGameInstance::Get_ObjectIndex(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strName)
{
	if (nullptr == m_pObject_Manager)
		return -1;

	return m_pObject_Manager->Get_ObjectIndex(iLevelIndex, strLayerTag, strName);
}

map<const wstring, class CGameObject*>* CGameInstance::Get_Prototypes()
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Prototypes();
}

HRESULT CGameInstance::Add_PrototypeCom(_uint iLevelIndex, const wstring & strPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);
}

HRESULT CGameInstance::Bind_Add_PrototypeCom(function<bool(_uint iLevelIndex, const wstring& strPrototypeTag, class CComponent* pPrototype)>& other)
{
	other = std::bind(&CGameInstance::Add_PrototypeCom, this, placeholders::_1, placeholders::_2, placeholders::_3);

	return E_NOTIMPL;
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, strPrototypeTag, pArg);	
}


//HRESULT CGameInstance::Set_MultiThreading(const _uint& iNumThread)
//{
//	if (nullptr == m_pThread_Manager)
//		return E_FAIL;
//
//	return m_pThread_Manager->Set_MultiThreading(iNumThread);
//}
//
//void CGameInstance::Finish_MultiThreading()
//{
//	if (nullptr == m_pThread_Manager)
//		return;
//
//	m_pThread_Manager->Finish_MultiThreading();
//}

const PROFILER_DESC CGameInstance::Get_ProfillingData() const
{
	if (nullptr == m_pProfiler_Manager)
	{
		PROFILER_DESC desc;
		ZeroMemory(&desc, sizeof(PROFILER_DESC));

		return desc;
	}
	return m_pProfiler_Manager->Get_Status();
}

_char CGameInstance::Get_DIKState(_uchar eKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKeyState(eKeyID);
}

_char CGameInstance::Get_DIMKeyState(CInput_Device::MOUSEKEYSTATE eMouseKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseState(eMouseKeyID);
}

_long CGameInstance::Get_DIMMoveState(CInput_Device::MOUSEMOVESTATE eMouseMoveID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseMove(eMouseMoveID);
}

const _bool CGameInstance::Key_Up(const _int& _iKey)
{
	if (nullptr == m_pInput_Device)
		return FALSE;

	return m_pInput_Device->Key_Up(_iKey);
}

const _bool CGameInstance::Key_Down(const _int& _iKey)
{
	if (nullptr == m_pInput_Device)
		return FALSE;

	return m_pInput_Device->Key_Down(_iKey);
}

const _bool CGameInstance::Key_Pressing(const _int& _iKey)
{
	if (nullptr == m_pInput_Device)
		return FALSE;

	return m_pInput_Device->Key_Pressing(_iKey);
}

//const _bool CGameInstance::Get_PickPos_Window(Vec2* vPickPos)
//{
//	if (nullptr == m_pInput_Device)
//		return FALSE;
//
//	return m_pInput_Device->Get_PickPos_Window(vPickPos);
//}

const _bool CGameInstance::Is_Focus()
{
	if (nullptr == m_pInput_Device)
		return FALSE;

	return m_pInput_Device->Is_Focus();
}

HRESULT CGameInstance::Bind_TransformToShader(CShader* pShader, const char* pConstantName, const Matrix& matTransform)
{
	return pShader->Bind_Matrix(pConstantName, &matTransform);
}

HRESULT CGameInstance::Bind_TransformToShader(CShader* pShader, const char* pConstantName, CPipeLine::TRANSFORM_STATE eState)
{
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	return m_pPipeLine->Bind_TransformToShader(pShader, pConstantName, eState);
}

Matrix CGameInstance::Get_Transform(const CPipeLine::TRANSFORM_STATE& eState) const
{
	if (nullptr == m_pPipeLine)
		return Matrix();

	return m_pPipeLine->Get_Transform(eState);
}

Matrix CGameInstance::Get_Transform_Inverse(const CPipeLine::TRANSFORM_STATE& eState) const
{
	if (nullptr == m_pPipeLine)
		return Matrix();

	return m_pPipeLine->Get_Transform_Inverse(eState);
}

Vec4 CGameInstance::Get_CamPosition() const
{
	if (nullptr == m_pPipeLine)
		return Vec4();

	return m_pPipeLine->Get_CamPosition();
}

void CGameInstance::Check_Collision_Layer(const wstring& strLayerTag1, const wstring& strLayerTag2, const CCollider::TYPE& eType1, const CCollider::TYPE& eType2)
{
	if (nullptr == m_pCollision_Manager)
		return;

	return m_pCollision_Manager->Check_Collision_Layer(strLayerTag1, strLayerTag2, eType1, eType2);
}

const _bool CGameInstance::Check_Collision_Ray(Ray& ray, CCollider* pCollider, OUT RAYHIT_DESC& hitDesc)
{
	if (nullptr == m_pCollision_Manager)
		return FALSE;

	return m_pCollision_Manager->Check_Collision_Ray(ray, pCollider, hitDesc);
}

const _bool CGameInstance::Check_Collision_PickingRay(CCollider* pCollider, const Matrix& matWorld, OUT RAYHIT_DESC& hitDesc)
{
	if (nullptr == m_pCollision_Manager)
		return FALSE;

	return m_pCollision_Manager->Check_Collision_PickingRay(pCollider, matWorld, hitDesc);
}

const _bool CGameInstance::Check_Collision_PickingRay(CModel* pModel, const Matrix& matWorld, OUT RAYHIT_DESC& hitDesc, const _bool& bPreInterSphere)
{
	if (nullptr == m_pCollision_Manager)
		return FALSE;

	return m_pCollision_Manager->Check_Collision_PickingRay(pModel, matWorld, hitDesc, bPreInterSphere);
}

Ray CGameInstance::Create_PickingRay(const Matrix& matWorld)
{
	if (nullptr == m_pCollision_Manager)
		return Ray();

	return m_pCollision_Manager->Create_PickingRay(matWorld);
}

CGameObject* CGameInstance::Get_Camera(const _uint& iKey)
{
	if(nullptr == m_pCamera_Manager)
		return nullptr;

	return m_pCamera_Manager->Get_Camera(iKey);
}

CGameObject* CGameInstance::Get_CurCamera()
{
	if (nullptr == m_pCamera_Manager)
		return nullptr;

	return m_pCamera_Manager->Get_CurCamera();
}

HRESULT CGameInstance::Set_CurCamera(const _uint& iKey)
{
	if (nullptr == m_pCamera_Manager)
		return E_FAIL;

	return m_pCamera_Manager->Set_CurCamera(iKey);
}

HRESULT CGameInstance::Add_Camera(const _uint& iKey, CGameObject* pCamera)
{
	if (nullptr == m_pCamera_Manager)
		return E_FAIL;

	return m_pCamera_Manager->Add_Camera(iKey, pCamera);
}

Vec4 CGameInstance::Get_CurCamera_State(const _uint iState)
{
	if (nullptr == m_pCamera_Manager)
		return Vec4();

	return m_pCamera_Manager->Get_CurCamera_State(iState);
}

HRESULT CGameInstance::Change_Camera(const _uint& iKey)
{
	if (nullptr == m_pCamera_Manager)
		return E_FAIL;

	return m_pCamera_Manager->Change_Camera(iKey);
}

HRESULT CGameInstance::Change_Camera_Inverse()
{
	if (nullptr == m_pCamera_Manager)
		return E_FAIL;

	return m_pCamera_Manager->Change_Camera_Inverse();
}

void CGameInstance::Release_Engine()
{
	CGameInstance::GetInstance()->DestroyInstance();
	CCamera_Manager::GetInstance()->DestroyInstance();
	CLevel_Manager::GetInstance()->DestroyInstance();
	CObject_Manager::GetInstance()->DestroyInstance();
	CComponent_Manager::GetInstance()->DestroyInstance();
	CTimer_Manager::GetInstance()->DestroyInstance();		
	CInput_Device::GetInstance()->DestroyInstance();
	CGraphic_Device::GetInstance()->DestroyInstance();
	CPipeLine::GetInstance()->DestroyInstance();
	CProfiler_Manager::GetInstance()->DestroyInstance();
	CCollision_Manager::GetInstance()->DestroyInstance();
	CNavMesh::GetInstance()->DestroyInstance();
}

void CGameInstance::Free()
{
	Safe_Release(m_pCamera_Manager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pComponent_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pProfiler_Manager);
	Safe_Release(m_pCollision_Manager);
	Safe_Release(m_pNavMesh);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
