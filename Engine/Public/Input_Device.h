/* TODO �̰� ���� �����ο� �����ϸ� �� ���øŽ��� �浹����? */
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
