#include "..\Default\stdafx.h"
#include "Input.h"

#include "GameInstance.h"

CInput::CInput()
{
}

void CInput::Update()
{
	if (GAME_INSTNACE->Key_Down(VK_SPACE)) Space = TRUE;
	else Space = FALSE;

	if (GAME_INSTNACE->Key_Down(VK_CONTROL)) Ctrl = TRUE;
	else Ctrl = FALSE;

	if (GAME_INSTNACE->Key_Down(VK_SHIFT)) Shift = TRUE;
	else Shift = FALSE;


	if (GAME_INSTNACE->Key_Down(VK_LBUTTON)) LBtn = TRUE;
	else LBtn = FALSE;

	if (GAME_INSTNACE->Key_Down(VK_RBUTTON)) RBtn = TRUE;
	else RBtn = FALSE;


	if (GAME_INSTNACE->Key_Down('W')) Up = TRUE;
	else Up = FALSE;

	if (GAME_INSTNACE->Key_Down('S')) Down = TRUE;
	else Down = FALSE;

	if (GAME_INSTNACE->Key_Down('A')) Left = TRUE;
	else Left = FALSE;

	if (GAME_INSTNACE->Key_Down('D')) Right = TRUE;
	else Right = FALSE;
}

CInput* CInput::Create()
{
	CInput* pInstance = new CInput();

	if (nullptr == pInstance)
	{
		MSG_BOX("Failed to Created : CInput");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInput::Free()
{
	__super::Free();
}
