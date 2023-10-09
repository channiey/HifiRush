#include "..\Public\Input_Device.h"

#include "GameInstance.h"

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

const _bool CInput_Device::Get_PickPos_Window(_Inout_ Vec2* vPickPos)
{
	if (!m_bFocus) return FALSE;

	POINT tMousePos{};

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	{
		HWND hWnd = pGameInstance->Get_GraphicDesc().hWnd;

		GetCursorPos(&tMousePos);
		ScreenToClient(hWnd, &tMousePos);

		(*vPickPos).x = (_float)tMousePos.x;
		(*vPickPos).y = (_float)tMousePos.y;
	}
	RELEASE_INSTANCE(CGameInstance);

	return TRUE;
}

void CInput_Device::Free()
{
	Safe_Release(m_pKeyboard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}
