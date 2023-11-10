#include "..\Public\EngineInstance.h"
#include "Timer_Manager.h"
#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Collision_Manager.h"
#include "Profiler_Manager.h"
#include "Camera_Manager.h"
#include "GameObject.h"
#include "NavMesh.h"
#include "Sound_Manager.h"
#include "Light_Manager.h"
#include "Target_Manager.h"

IMPLEMENT_SINGLETON(CEngineInstance)

CEngineInstance::CEngineInstance()
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
	, m_pSound_Manager(CSound_Manager::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
	, m_pTarget_Manager(CTarget_Manager::GetInstance())

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
	Safe_AddRef(m_pSound_Manager);
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pTarget_Manager);
}

HRESULT CEngineInstance::Initialize_Engine(_uint iNumLevels, HINSTANCE hInst, const GRAPHIC_DESC& GraphicDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext, const char* strSoundFilePath)
{
	/* 그래픽디바이스 초기화 처리. */
	if (FAILED(m_pGraphic_Device->Ready_Graphic_Device(GraphicDesc, ppDevice, ppContext)))
		return E_FAIL;

	m_pDevice = *ppDevice;
	m_pContext = *ppContext;

	/* 사운드디바이스 초기화 처리. */
	if (FAILED(m_pSound_Manager->Initialize(strSoundFilePath)))
		return E_FAIL;

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

void CEngineInstance::Tick(_double fTimeDelta)
{
	/* I.O Update */
	{
		m_pInput_Device->Tick();
		m_pSound_Manager->Update(fTimeDelta);
	}

	/* Level, Obj Update*/
	{
		m_pLevel_Manager->Tick(fTimeDelta);
		m_pObject_Manager->Tick(fTimeDelta);
	}

	/* 카메라 오브젝트 월드행렬 갱신 -> 뷰행령 투영행렬 저장 -> 계산*/
	{
		m_pCamera_Manager->Tick();
		m_pPipeLine->Tick();
	}
}

void CEngineInstance::LateTick(_double fTimeDelta)
{
	m_pLevel_Manager->LateTick(fTimeDelta);
	m_pObject_Manager->LateTick(fTimeDelta);
}

void CEngineInstance::FinishTick()
{
	m_pObject_Manager->FinishTick();
	m_pProfiler_Manager->FinishTick();
}

void CEngineInstance::Clear(_uint iLevelIndex)
{
	m_pObject_Manager->Clear(iLevelIndex);
}

_double CEngineInstance::Compute_TimeDelta(const wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return 0.f;

	return m_pTimer_Manager->Compute_TimeDelta(strTimerTag);	
}

HRESULT CEngineInstance::Add_Timer(const wstring & strTimerTag)
{
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

HRESULT CEngineInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
}

HRESULT CEngineInstance::Clear_DepthStencil_View()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CEngineInstance::Present()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	return m_pGraphic_Device->Present();
}

ID3D11Device* CEngineInstance::Get_Device()
{
	if (nullptr == m_pGraphic_Device)
		return nullptr;

	return m_pGraphic_Device->Get_Device();
}

GRAPHIC_DESC CEngineInstance::Get_GraphicDesc()
{
	if (nullptr == m_pGraphic_Device)
		return GRAPHIC_DESC{};

	return m_pGraphic_Device->Get_GraphicDesc();
}

const Viewport CEngineInstance::Get_ViewPort()
{
	if (nullptr == m_pGraphic_Device)
		return Viewport{};

	return m_pGraphic_Device->Get_ViewPort();
}

HRESULT CEngineInstance::Open_Level(_uint iLevelIndex, CLevel * pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iLevelIndex, pNewLevel);
}

const _uint CEngineInstance::Get_CurLevelIndex()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Get_CurLevelIndex();
}

const _uint CEngineInstance::Get_CurLoadingLevel()
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Get_CurLoadingLevel();
}

void CEngineInstance::Set_CurLoadingLevel(const _uint iLevel)
{
	if (nullptr == m_pLevel_Manager)
		return;

	return m_pLevel_Manager->Set_CurLoadingLevel(iLevel);
}

HRESULT CEngineInstance::Add_Prototype(const wstring & strPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Prototype(strPrototypeTag, pPrototype);	
}

class CGameObject* CEngineInstance::Add_GameObject(const _uint iLevelIndex, const wstring & strLayerTag, const wstring & strPrototypeTag, void * pArg)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Add_GameObject(iLevelIndex, strLayerTag, strPrototypeTag, pArg);
}

HRESULT CEngineInstance::Delete_GameObject(const _uint iLevelIndex, CGameObject* pObj)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Delete_GameObject(iLevelIndex, pObj);
}
 
HRESULT CEngineInstance::Reserve_Pool(const _uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag, const _uint& iNumObj, void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Reserve_Pool(iLevelIndex, strLayerTag, strPrototypeTag, iNumObj, pArg);
}

CGameObject* CEngineInstance::Pop_Pool(const _uint iLevelIndex, const wstring& strPrototypeTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Pop_Pool(iLevelIndex, strPrototypeTag);
}

HRESULT CEngineInstance::Return_Pool(const _uint iLevelIndex, CGameObject* pObj)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Return_Pool(iLevelIndex, pObj);
}

HRESULT CEngineInstance::Add_Layer(_uint iLevelIndex, const wstring& strLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_Layer(iLevelIndex, strLayerTag);
}

HRESULT CEngineInstance::Delete_Layer(_uint iLevelIndex, const wstring& strLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Delete_Layer(iLevelIndex, strLayerTag);
}

map<const wstring, class CLayer*>* CEngineInstance::Get_All_Layer(_uint iLevelIndex)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_All_Layer(iLevelIndex);
}

map<const wstring, class CLayer*>* CEngineInstance::Get_All_Layer_CurLevel()
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_All_Layer(Get_CurLevelIndex());
}

list<class CGameObject*>* CEngineInstance::Get_Layer(_uint iLevelIndex, const wstring& strLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Layer(iLevelIndex, strLayerTag);
}

CLayer* CEngineInstance::Get_LayerClass(_uint iLevelIndex, const wstring& strLayerTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_LayerClass(iLevelIndex, strLayerTag);
}

CGameObject* CEngineInstance::Get_Player()
{
	return nullptr;
}

CGameObject* CEngineInstance::Get_GameObject(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strPrototypeTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_GameObject(iLevelIndex, strLayerTag, strPrototypeTag);
}

CGameObject* CEngineInstance::Get_GameObject_InCurLevel(const wstring& strLayerTag, const wstring& strPrototypeTag)
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_GameObject(Get_CurLevelIndex(), strLayerTag, strPrototypeTag);
}

CGameObject* CEngineInstance::Get_GameObject_InCurLevel_InLayerFirst(const wstring& strLayerTag)
{
	list<class CGameObject*>* pLayer = Get_Layer(Get_CurLevelIndex(), strLayerTag);
	if (nullptr == pLayer || pLayer->empty())
		return nullptr;

	return pLayer->front();
}

const _int CEngineInstance::Get_ObjectIndex(_uint iLevelIndex, const wstring& strLayerTag, const wstring& strName)
{
	if (nullptr == m_pObject_Manager)
		return -1;

	return m_pObject_Manager->Get_ObjectIndex(iLevelIndex, strLayerTag, strName);
}

map<const wstring, class CGameObject*>* CEngineInstance::Get_Prototypes()
{
	if (nullptr == m_pObject_Manager)
		return nullptr;

	return m_pObject_Manager->Get_Prototypes();
}

HRESULT CEngineInstance::Add_PrototypeCom(_uint iLevelIndex, const wstring & strPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent_Manager)
		return E_FAIL;

	return m_pComponent_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);
}

HRESULT CEngineInstance::Bind_Add_PrototypeCom(function<bool(_uint iLevelIndex, const wstring& strPrototypeTag, class CComponent* pPrototype)>& other)
{
	other = std::bind(&CEngineInstance::Add_PrototypeCom, this, placeholders::_1, placeholders::_2, placeholders::_3);

	return E_NOTIMPL;
}

CComponent * CEngineInstance::Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void* pArg)
{
	if (nullptr == m_pComponent_Manager)
		return nullptr;

	return m_pComponent_Manager->Clone_Component(iLevelIndex, strPrototypeTag, pArg);	
}


//HRESULT CEngineInstance::Set_MultiThreading(const _uint& iNumThread)
//{
//	if (nullptr == m_pThread_Manager)
//		return E_FAIL;
//
//	return m_pThread_Manager->Set_MultiThreading(iNumThread);
//}
//
//void CEngineInstance::Finish_MultiThreading()
//{
//	if (nullptr == m_pThread_Manager)
//		return;
//
//	m_pThread_Manager->Finish_MultiThreading();
//}

const PROFILER_DESC CEngineInstance::Get_ProfillingData() const
{
	if (nullptr == m_pProfiler_Manager)
	{
		PROFILER_DESC desc;
		ZeroMemory(&desc, sizeof(PROFILER_DESC));

		return desc;
	}
	return m_pProfiler_Manager->Get_Status();
}

_char CEngineInstance::Get_DIKState(_uchar eKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIKeyState(eKeyID);
}

_char CEngineInstance::Get_DIMKeyState(CInput_Device::MOUSEKEYSTATE eMouseKeyID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseState(eMouseKeyID);
}

_long CEngineInstance::Get_DIMMoveState(CInput_Device::MOUSEMOVESTATE eMouseMoveID)
{
	if (nullptr == m_pInput_Device)
		return 0;

	return m_pInput_Device->Get_DIMouseMove(eMouseMoveID);
}

const _bool CEngineInstance::Key_Up(const _int& _iKey)
{
	if (nullptr == m_pInput_Device)
		return FALSE;

	return m_pInput_Device->Key_Up(_iKey);
}

const _bool CEngineInstance::Key_Down(const _int& _iKey)
{
	if (nullptr == m_pInput_Device)
		return FALSE;

	return m_pInput_Device->Key_Down(_iKey);
}

const _bool CEngineInstance::Key_Pressing(const _int& _iKey)
{
	if (nullptr == m_pInput_Device)
		return FALSE;

	return m_pInput_Device->Key_Pressing(_iKey);
}

//const _bool CEngineInstance::Get_PickPos_Window(Vec2* vPickPos)
//{
//	if (nullptr == m_pInput_Device)
//		return FALSE;
//
//	return m_pInput_Device->Get_PickPos_Window(vPickPos);
//}

const _bool CEngineInstance::Is_Focus()
{
	if (nullptr == m_pInput_Device)
		return FALSE;

	return m_pInput_Device->Is_Focus();
}

HRESULT CEngineInstance::Bind_TransformToShader(CShader* pShader, const char* pConstantName, const Matrix& matTransform)
{
	return pShader->Bind_Matrix(pConstantName, &matTransform);
}

HRESULT CEngineInstance::Bind_TransformToShader(CShader* pShader, const char* pConstantName, CPipeLine::TRANSFORM_STATE eState)
{
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	return m_pPipeLine->Bind_TransformToShader(pShader, pConstantName, eState);
}

Matrix CEngineInstance::Get_Transform(const CPipeLine::TRANSFORM_STATE& eState) const
{
	if (nullptr == m_pPipeLine)
		return Matrix();

	return m_pPipeLine->Get_Transform(eState);
}

Matrix CEngineInstance::Get_Transform_Inverse(const CPipeLine::TRANSFORM_STATE& eState) const
{
	if (nullptr == m_pPipeLine)
		return Matrix();

	return m_pPipeLine->Get_Transform_Inverse(eState);
}

Vec4 CEngineInstance::Get_CamPosition() const
{
	if (nullptr == m_pPipeLine)
		return Vec4();

	return m_pPipeLine->Get_CamPosition();
}

void CEngineInstance::Check_Collision_Layer(const wstring& strLayerTag1, const wstring& strLayerTag2, const CCollider::TYPE& eType1, const CCollider::TYPE& eType2)
{
	if (nullptr == m_pCollision_Manager)
		return;

	return m_pCollision_Manager->Check_Collision_Layer(strLayerTag1, strLayerTag2, eType1, eType2);
}

RAYHIT_DESC CEngineInstance::Check_ScreenRay(const wstring& strLayerTag, const _bool& bSnap)
{
	if (nullptr == m_pCollision_Manager)
		return RAYHIT_DESC();

	return m_pCollision_Manager->Check_ScreenRay(strLayerTag, bSnap);
}

CCell* CEngineInstance::Check_ScreenRay()
{
	if (nullptr == m_pCollision_Manager)
		return nullptr;

	return m_pCollision_Manager->Check_ScreenRay();
}

const Ray CEngineInstance::Create_ScreenRayLocal(Matrix matWorld)
{
	if (nullptr == m_pCollision_Manager)
		return Ray();

	return m_pCollision_Manager->Create_ScreenRayLocal(matWorld);
}

CGameObject* CEngineInstance::Get_Camera(const _uint& iKey)
{
	if(nullptr == m_pCamera_Manager)
		return nullptr;

	return m_pCamera_Manager->Get_Camera(iKey);
}

CGameObject* CEngineInstance::Get_CurCamera()
{
	if (nullptr == m_pCamera_Manager)
		return nullptr;

	return m_pCamera_Manager->Get_CurCamera();
}

HRESULT CEngineInstance::Set_CurCamera(const _uint& iKey)
{
	if (nullptr == m_pCamera_Manager)
		return E_FAIL;

	return m_pCamera_Manager->Set_CurCamera(iKey);
}

HRESULT CEngineInstance::Add_Camera(const _uint& iKey, CGameObject* pCamera)
{
	if (nullptr == m_pCamera_Manager)
		return E_FAIL;

	return m_pCamera_Manager->Add_Camera(iKey, pCamera);
}

Vec4 CEngineInstance::Get_CurCamera_State(const _uint iState)
{
	if (nullptr == m_pCamera_Manager)
		return Vec4();

	return m_pCamera_Manager->Get_CurCamera_State(iState);
}

HRESULT CEngineInstance::Change_Camera(const _uint& iKey)
{
	if (nullptr == m_pCamera_Manager)
		return E_FAIL;

	return m_pCamera_Manager->Change_Camera(iKey);
}

HRESULT CEngineInstance::Change_Camera_Inverse()
{
	if (nullptr == m_pCamera_Manager)
		return E_FAIL;

	return m_pCamera_Manager->Change_Camera_Inverse();
}

const map<_uint, CGameObject*>* CEngineInstance::Get_Cameras()
{
	if (nullptr == m_pCamera_Manager)
		return nullptr;

	return m_pCamera_Manager->Get_Cameras();
}

void CEngineInstance::Play_Sound(_uint eSoundID, _uint eChannelID, float fVolume)
{
	if (nullptr == m_pSound_Manager)
		return;

	return m_pSound_Manager->PlaySound(eSoundID, eChannelID, fVolume);
}

HRESULT CEngineInstance::Register_BGM(_uint eSoundID, _uint eChannelID, float fVolume)
{
	if (nullptr == m_pSound_Manager)
		return E_FAIL;

	return m_pSound_Manager->Register_BGM(eSoundID, eChannelID, fVolume);
}

HRESULT CEngineInstance::Play_BGM()
{
	if(nullptr == m_pSound_Manager)
		return E_FAIL;

	return m_pSound_Manager->Play_BGM();
}

const _uint CEngineInstance::Get_BPM()
{
	if (nullptr == m_pSound_Manager)
		return E_FAIL;

	return m_pSound_Manager->Get_BPM();
}

const _bool CEngineInstance::Is_PlayBGM()
{
	if (nullptr == m_pSound_Manager)
		return FALSE;

	return m_pSound_Manager->Is_PlayBGM();
}

void CEngineInstance::Lerp_BGMSound(const _float& fTargetValue, const _double& fTime, const LERP_MODE& eMode)
{
	if (nullptr == m_pSound_Manager)
		return;

	return m_pSound_Manager->Lerp_BGMSound(fTargetValue, fTime, eMode);
}

const LIGHT_DESC* CEngineInstance::Get_LightDesc(_uint iLightIndex)
{
	if (nullptr == m_pLight_Manager)
		return nullptr ;

	return m_pLight_Manager->Get_LightDesc(iLightIndex);
}

HRESULT CEngineInstance::Add_Light(const LIGHT_DESC& LightDesc)
{
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return m_pLight_Manager->Add_Light(LightDesc);
}

const _bool CEngineInstance::Is_DebugRTV() const
{
	if (nullptr == m_pTarget_Manager)
		return FALSE;

	return m_pTarget_Manager->Is_DebugRTV();
}

void CEngineInstance::Set_DebugRTV(const _bool& bSet)
{
	if (nullptr == m_pTarget_Manager)
		return;

	return m_pTarget_Manager->Set_DebugRTV(bSet);
}

void CEngineInstance::Set_DebugRTV()
{
	if (nullptr == m_pTarget_Manager)
		return;

	return m_pTarget_Manager->Set_DebugRTV();
}

void CEngineInstance::Release_Engine()
{
	CEngineInstance::GetInstance()->DestroyInstance();
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
	CSound_Manager::GetInstance()->DestroyInstance();
	CLight_Manager::GetInstance()->DestroyInstance();
	CTarget_Manager::GetInstance()->DestroyInstance();
}

void CEngineInstance::Free()
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
	Safe_Release(m_pSound_Manager);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pTarget_Manager);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
