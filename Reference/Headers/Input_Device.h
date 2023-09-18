/* TODO �̰� ���� �����ο� �����ϸ� �� ���øŽ��� �浹����? */
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
	/* �� ������Ʈ���� ��ǲ ���¸� �����Ѵ�. */
	void					Tick(void);

public:
	// API �� ����ϴ� Ű ��ǲ �Լ�
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

	// ���콺 ���� ��ǲ ����																											
	/*
							
							if (dwMouse = CInput_Device::GetInstance()->Get_DIMouseMove(DIMS_Y))
							{
								// ��ũ�� ��ǥ�踦 �������� ���� �����ӿ��� ������������ ���콺 y������ ���̸� ��ȯ�Ѵ�.
								// ���콺�� �ø��� -��, ������ +��
							}

							if (dwMouse = CInput_Device::GetInstance()->Get_DIMouseMove(DIMS_X))
							{
								// ��ũ�� ��ǥ�踦 �������� ���� �����ӿ��� ������������ ���콺 x������ ���̸� ��ȯ�Ѵ�.
								// ���콺�� ���������� �ϸ� +��, �������� �ϸ� -��
							}

							if (dwMouse = CInput_Device::GetInstance()->Get_DIMouseMove(DIMS_Z))
							{
								// ���� ��ũ�� �ϸ� 120 ��ȯ
								// �Ʒ��� ��ũ�� �ϸ� -120 ��ȯ
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
