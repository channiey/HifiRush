#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class Input final : public CBase
{
public:
	Input();
	virtual ~Input() = default;

public:
	/* Key */
	static const _bool Space();
	static const _bool Shift();
	static const _bool Ctrl();
	static const _bool LBtn();
	static const _bool RBtn();
	static const _bool Up();
	static const _bool Down();
	static const _bool Left();
	static const _bool Right();

	/* Action */
	static const _bool Move();
	static const _bool Jump();

	static const _bool Attack();
	static const _bool Parry();
	static const _bool Interact();
	static const _bool Magnet();


public:
	virtual void Free() override;
};

END

