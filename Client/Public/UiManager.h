#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CUi;
class CUi_Dialouge;

class CUiManager final : public CBase
{
	DECLARE_SINGLETON(CUiManager)

private:
	CUiManager();
	virtual ~CUiManager() = default;

public:
	HRESULT	Initialize();

public:
	CUi*			Get_UI(const UI_ID& eID);
	HRESULT			Add_UI(const UI_ID& eID, CUi* pUi);

public:
	HRESULT			On_Dialouge(_uint eCharacterType, const wstring& strText);

private:
	CUi*			Find_UI(const UI_ID& eID);
private:
	map<UI_ID, CUi*>	m_UIs;
	CUi_Dialouge*		m_pDialouge = { nullptr };

public:
	virtual void	Free() override;
};

END


