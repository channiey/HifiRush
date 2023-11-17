#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CUi;

class CUiManager final : public CBase
{
	DECLARE_SINGLETON(CUiManager)

private:
	CUiManager();
	virtual ~CUiManager() = default;

public:
	HRESULT	Initialize();

public:
	CUi*	Get_UI(const UI_ID& eID);
	HRESULT Add_UI(const UI_ID& eID, CUi* pUi);

private:
	CUi*	Find_UI(const UI_ID& eID);

private:
	map<UI_ID, CUi*> m_UIs;

public:
	virtual void	Free() override;
};

END


