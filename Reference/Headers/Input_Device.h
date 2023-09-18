/* TODO 이거 엔진 디파인에 선언하면 왜 심플매스랑 충돌나지? */
#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>

#include "Base.h"


BEGIN(Engine)

class CInput_Device final : public CBase
{
	DECLARE_SINGLETON(CInput_Device)

private:
	CInput_Device(void);
	virtual ~CInput_Device(void);
	
public:
	HRESULT					Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	/* 매 업데이트마다 인풋 상태를 갱신한다. */
	void					Tick(void);

public:
	// API 때 사용하던 키 인풋 함수
	_bool Key_Up(const _int& _iKey);
	_bool Key_Down(const _int& _iKey);
	_bool Key_Pressing(const _int& _iKey);

	_byte Get_DIKeyState(_ubyte byKeyID) { 
		return m_byKeyState[byKeyID]; 
	}
	_byte Get_DIMouseState(MOUSEKEYSTATE eMouse) { 
		return m_tMouseState.rgbButtons[eMouse]; 	
	}
	_long Get_DIMouseMove(MOUSEMOVESTATE eMouseState)	{ 
		return *(((_long*)&m_tMouseState) + eMouseState); 
	} 

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

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;	

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;	
	LPDIRECTINPUTDEVICE8	m_pMouse	= nullptr;	

private:
	_byte					m_byKeyState[256];		
	DIMOUSESTATE			m_tMouseState;	

	bool					m_bKeyState[VK_MAX];

public:
	virtual void			Free(void);

};
END
