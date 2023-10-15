#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CInput final : public CBase
{
private:
	CInput();
	virtual ~CInput() = default;

public:
	void Update();

public:
	_bool	Space	= FALSE;
	_bool	Shift	= FALSE;
	_bool   Ctrl	= FALSE;

	_bool	LBtn	= FALSE;
	_bool	RBtn	= FALSE;

	_bool   Up		= FALSE;
	_bool   Down	= FALSE;
	_bool   Left	= FALSE;
	_bool   Right	= FALSE;

	_bool	Block	= FALSE;

public:
	static CInput* Create();
	virtual void Free() override;
};

END

