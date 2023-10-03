#include "..\Public\Input_Device.h"

#include "GameInstance.h"

/* TODO Picking Class */
#include "Graphic_Device.h"
#include "GameObject.h"
#include "Transform.h"
#include "VIBuffer_Terrain.h"
#include "Layer.h"
#include "Collider_Sphere.h"

IMPLEMENT_SINGLETON(CInput_Device)

CInput_Device::CInput_Device()
{

}

_bool CInput_Device::Key_Pressing(const _int& _iKey)
{
	if (!m_bFocus) return FALSE;

	if (GetAsyncKeyState(_iKey) & 0x8000)
		return true;

	return false;
}

_bool CInput_Device::Key_Down(const _int& _iKey)
{
	if (!m_bFocus) return FALSE;

	if (!m_bKeyState[_iKey] && (GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];
		return true;
	}

	for (int i = 0; i < VK_MAX; ++i)
	{
		if (m_bKeyState[i] && !(GetAsyncKeyState(i) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
	}

	return false;
}

_bool CInput_Device::Key_Up(const _int& _iKey)
{
	if (!m_bFocus) return FALSE;

	if (m_bKeyState[_iKey] && !(GetAsyncKeyState(_iKey) & 0x8000))
	{
		m_bKeyState[_iKey] = !m_bKeyState[_iKey];
		return true;
	}

	for (int i = 0; i < VK_MAX; ++i)
	{
		if (!m_bKeyState[_iKey] && (GetAsyncKeyState(_iKey) & 0x8000))
			m_bKeyState[i] = !m_bKeyState[i];
	}

	return false;
}

HRESULT CInput_Device::Initialize(HINSTANCE hInstance, HWND hWnd)
{
	/* m_pInputSDK */
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, nullptr)))
		return E_FAIL;

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, nullptr)))
		return E_FAIL;

	if (FAILED(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return E_FAIL;
	if (FAILED(m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE /*| DISCL_NOWINKEY*/)))
		return E_FAIL;
	if (FAILED(m_pKeyboard->Acquire()))
		return E_FAIL;

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		return E_FAIL;
	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE /*| DISCL_NOWINKEY*/)))
		return E_FAIL;
	if (FAILED(m_pMouse->Acquire()))
		return E_FAIL;

	return S_OK;
}

void CInput_Device::Tick()
{
	m_bFocus = GetFocus();
	if (m_bFocus)
	{
		m_pKeyboard->GetDeviceState(256, m_byKeyState);
		m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);
	}
}

CGameObject* CInput_Device::Get_Pick_Object()
{
	if (!m_bFocus) return nullptr;

	CGameObject* pPickedObj = nullptr;
	_float		 fMinDist;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	{
		typedef map<const wstring, class CLayer*> LAYERS;

		LAYERS* pLayers = pGameInstance->Get_All_Layer_CurLevel();
	
		if (nullptr == pLayers)
		{
			RELEASE_INSTANCE(CGameInstance);
			return nullptr;
		}

		for (auto& Pair : *pLayers)
		{
			list<class CGameObject*>* pLayer = Pair.second->Get_Objects();
			if (nullptr == pLayer)
				continue;

			for (auto& pObj : *pLayer)
			{
				if (nullptr == pObj)
					continue;

				CCollider_Sphere* pCollider = pObj->Get_Collider_Sphere();
				
				if (nullptr == pCollider)
					continue;

				/* Create Ray */
				Ray ray;
				{
					Matrix		    matW, matV, matP;
					Viewport		viewPort;

					matW = Matrix::Identity;
					matV = pGameInstance->Get_Transform(CPipeLine::STATE_VIEW);
					matP = pGameInstance->Get_Transform(CPipeLine::STATE_PROJ);
					viewPort = pGameInstance->Get_ViewPort();

					Vec2 vPickWindow;
					if (!Get_PickPos_Window(vPickWindow))
					{
						RELEASE_INSTANCE(CGameInstance);
						return FALSE;
					}

					Vec3 n = viewPort.Unproject(Vec3(vPickWindow.x, vPickWindow.y, 0.f), matP, matV, matW);
					Vec3 f = viewPort.Unproject(Vec3(vPickWindow.x, vPickWindow.y, 1.f), matP, matV, matW);

					Vec3 vOrigin = n;
					Vec3 vDirection = f - n;
					vDirection.Normalize();

					ray.position = vOrigin;
					ray.direction = vDirection;
				}

				/* Is Intersect ? */
				_float fDistance;
				if (ray.Intersects(pCollider->Get_Collider(), OUT fDistance))
				{
					if (nullptr == pPickedObj)
						pPickedObj = pObj;

					pPickedObj = (pPickedObj->Get_CamDistance() < pObj->Get_CamDistance()) ? pPickedObj : pObj;
				}
			}
		}
	}
	RELEASE_INSTANCE(CGameInstance);
	return pPickedObj;
}

const _bool CInput_Device::Get_PickPos_Window(_Inout_ Vec2& vPickPos)
{
	if (!m_bFocus) return FALSE;

	POINT tMousePos{};

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	{
		HWND hWnd = pGameInstance->Get_GraphicDesc().hWnd;

		GetCursorPos(&tMousePos);
		ScreenToClient(hWnd, &tMousePos);

		vPickPos.x = (_float)tMousePos.x;
		vPickPos.y = (_float)tMousePos.y;
	}
	RELEASE_INSTANCE(CGameInstance);

	return TRUE;
}

const _bool CInput_Device::Get_PickPos_Terrain(class CVIBuffer_Terrain* pBuffer, Matrix matWorld, _Inout_ Vec3& vPickPos)
{
	if (!m_bFocus || nullptr == pBuffer)
		return FALSE;

	/* UnProjection */
	Matrix		    matW, matV, matP;
	Viewport		viewPort;
	_float			fDistance;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	{
		matW = matWorld;
		matV = pGameInstance->Get_Transform(CPipeLine::STATE_VIEW);
		matP = pGameInstance->Get_Transform(CPipeLine::STATE_PROJ);
		viewPort = pGameInstance->Get_ViewPort();
	}	
	RELEASE_INSTANCE(CGameInstance);

	Vec2 vPickWindow;
	if (!Get_PickPos_Window(vPickWindow)) 
		return FALSE;

	Vec3 n = viewPort.Unproject(Vec3(vPickWindow.x, vPickWindow.y, 0.f), matP, matV, matW);
	Vec3 f = viewPort.Unproject(Vec3(vPickWindow.x, vPickWindow.y, 1.f), matP, matV, matW);

	/* Ray */
	Vec3 vOrigin = n;
	Vec3 vDir = f - n;
	vDir.Normalize();

	Ray ray(vOrigin, vDir);

	/* Search */
	_uint		iNumVerticesZ = pBuffer->Get_VerticesZ();
	_uint		iNumVerticesX = pBuffer->Get_VerticesX();
	_float3*	pVerticesPos = pBuffer->Get_VerticesPos();

	for (_uint i = 0; i < iNumVerticesZ; i++)
	{
		for (_uint j = 0; j < iNumVerticesX; j++)
		{
			_uint		iIndices[4] = {
				(i + 1) * iNumVerticesX + j,		//2
				(i + 1) * iNumVerticesX + j + 1,	//3
				i * iNumVerticesX + j + 1,			//1
				i * iNumVerticesX + j				//0
			};

			Vec3 p[4];
			for (_uint i = 0; i < 4; i++)
			{
				_uint index = iIndices[i];
				if (iNumVerticesX * iNumVerticesZ < index)
					return FALSE;
				p[i] = pVerticesPos[index];
			}

			if (ray.Intersects(p[1], p[2], p[0], OUT fDistance))
			{
				vPickPos = ray.position + ray.direction * fDistance;
				if (isnan(vPickPos.x) || isnan(vPickPos.y) || isnan(vPickPos.z) || isnan(fDistance))
					return FALSE;
				return TRUE;
			}

			if (ray.Intersects(p[3], p[0], p[2], OUT fDistance))
			{
				vPickPos = ray.position + ray.direction * fDistance;
				if (isnan(vPickPos.x) || isnan(vPickPos.y) || isnan(vPickPos.z) || isnan(fDistance))
					return FALSE;
				return TRUE;
			}
		}
	}
	
	
	return TRUE;
}

void CInput_Device::Free()
{
	Safe_Release(m_pKeyboard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}

/*

	»ç¿ë¹ý 

	_long	dwMouse = 0;

	if (dwMouse = Engine::Get_DIMouseMove(DIMS_Z))
	{
		int a = 0;
	}

*/