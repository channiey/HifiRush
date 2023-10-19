#include "..\Default\stdafx.h"
#include "Input.h"

#include "GameInstance.h"

_bool Input::bLeft = FALSE;
_bool Input::bRight = TRUE;
_bool Input::bCenter = TRUE;

Input::Input()
{
}

const _bool Input::Space()
{
	return GAME_INSTNACE->Key_Down(VK_SPACE);
}

const _bool Input::Shift()
{
	return GAME_INSTNACE->Key_Down(VK_SHIFT);
}

const _bool Input::Ctrl()
{
	return GAME_INSTNACE->Key_Down(VK_CONTROL);
}

const _bool Input::LBtn()
{
	return bLeft;
}

const _bool Input::RBtn()
{
	return bRight;
}

const _bool Input::MBtn()
{
	return bCenter;
}

const _bool Input::Up()
{
	return GAME_INSTNACE->Key_Pressing('W');
}

const _bool Input::Down()
{
	return GAME_INSTNACE->Key_Pressing('S');
}

const _bool Input::Left()
{
	return GAME_INSTNACE->Key_Pressing('A');
}

const _bool Input::Right()
{
	return GAME_INSTNACE->Key_Pressing('D');
}

const _bool Input::Parry()
{
	return GAME_INSTNACE->Key_Down('E');
}

const _bool Input::Interact()
{
	return GAME_INSTNACE->Key_Down('D');
}

const _bool Input::Magnet()
{
	return GAME_INSTNACE->Key_Down('Q');
}

const _bool Input::Move()
{
	if (Up() || Down() || Left() || Right())
		return TRUE;

	return FALSE;
}

const _bool Input::Attack()
{
	bLeft = bRight = bCenter = FALSE;

	if (GAME_INSTNACE->Key_Down(VK_LBUTTON))
		bLeft = TRUE;
	else if (GAME_INSTNACE->Key_Down(VK_RBUTTON))
		bRight = TRUE;
	else if (GAME_INSTNACE->Key_Down(VK_MBUTTON))
		bCenter = TRUE;
	else
		return FALSE;

	return TRUE;
}

const _bool Input::Jump()
{
	if (Space())
		return TRUE;

	return FALSE;
}

void Input::Free()
{
	__super::Free();
}
