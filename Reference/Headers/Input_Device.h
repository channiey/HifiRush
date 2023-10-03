/* TODO 이거 엔진 디파인에 선언하면 왜 심플매스랑 충돌나지? */
#pragma once

#include "Base.h"

BEGIN(Engine)

class CInput_Device final : public CBase
{
	DECLARE_SINGLETON(CInput_Device)

public:
	enum MOUSEKEYSTATE		{ MKS_LBUTTON, MKS_RBUTTON, MKS_WHEELBUTTON, MKS_XBUTTON, MKS_END };
	enum MOUSEMOVESTATE		{ MMS_X, MMS_Y, MMS_WHEEL, MMS_END };

private:
	CInput_Device();
	virtual ~CInput_Device() = default;
	
public:
	HRESULT	Initialize(HINSTANCE hInst, HWND hWnd);
	void Tick();

public:
	const _bool Get_PickPos_Window(_Inout_ Vec2& vPickPos);

	/* Temp */
	const _bool Get_PickPos_Terrain(class CVIBuffer_Terrain* pBuffer, Matrix matWorld, _Inout_ Vec3& vPickPos);

public:
	/* User */
	_bool Key_Up(const _int& _iKey);
	_bool Key_Down(const _int& _iKey);
	_bool Key_Pressing(const _int& _iKey);

	/* DX */
	_char Get_DIKeyState(_ubyte byKeyID) {
		if (!m_bFocus) return 0;
		return m_byKeyState[byKeyID]; 
	}
	_char Get_DIMouseState(MOUSEKEYSTATE eMouse) {
		if (!m_bFocus) return 0;
		return m_tMouseState.rgbButtons[eMouse]; 	
	}
	_long Get_DIMouseMove(MOUSEMOVESTATE eMouseState)	{
		if (!m_bFocus) return 0;
		return *(((_long*)&m_tMouseState) + eMouseState); 
	} 

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;
	LPDIRECTINPUTDEVICE8	m_pKeyboard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

private:
	_byte					m_byKeyState[256] = { 0 };
	DIMOUSESTATE			m_tMouseState;	
	_bool					m_bKeyState[VK_MAX];
	HWND					m_bFocus{};

public:
	virtual void			Free();
};
END



// 마우스 무브 인풋 사용법																											
/*

						if (dwMouse = CInput_Device::GetInstance()->Get_DIMouseMove(DIMS_Y))
						{
							// 스크린 좌표계를 기준으로 이전 프레임에서 현재프레임의 마우스 y포지션 차이를 반환한다.
							// 마우스를 올리면 -값, 내리면 +값
						}

						if (dwMouse = CInput_Device::GetInstance()->Get_DIMouseMove(DIMS_X))
						{
							// 스크린 좌표계를 기준으로 이전 프레임에서 현재프레임의 마우스 x포지션 차이를 반환한다.
							// 마우스를 오른쪽으로 하면 +값, 왼쪽으로 하면 -값
						}

						if (dwMouse = CInput_Device::GetInstance()->Get_DIMouseMove(DIMS_Z))
						{
							// 위로 스크롤 하면 120 반환
							// 아래로 스크롤 하면 -120 반환
						}

*/
