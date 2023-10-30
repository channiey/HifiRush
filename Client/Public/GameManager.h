#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CGameManager final : public CBase
{
	DECLARE_SINGLETON(CGameManager)

private:
	CGameManager();
	virtual ~CGameManager() = default;

public:
	HRESULT			Initialize();

public:
	virtual void	Free() override;
};

END


