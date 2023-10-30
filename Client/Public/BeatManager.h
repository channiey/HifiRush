#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CBeatManager final : public CBase
{
	DECLARE_SINGLETON(CBeatManager)

private:
	CBeatManager();
	virtual ~CBeatManager() = default;

public:
	HRESULT			Initialize();

public:
	virtual void	Free() override;
};

END


