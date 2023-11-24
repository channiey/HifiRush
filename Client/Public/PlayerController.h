#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CCharacter;

enum PLAYER_TYPE	{ CHAI, PEPPERMINT, MACARON, KORSICA, TYPEEND };
enum PLAYER_STATE	{ WAIT, APPEAR, COOLTIME, STATE_END };

typedef struct tagPlayerDesc
{
	CCharacter*		pPlayer		= nullptr;
	PLAYER_TYPE		eType		= PLAYER_TYPE::TYPEEND;
	PLAYER_STATE	eState		= PLAYER_STATE::STATE_END;

	const _float	fCoolTimeLimit		= 3.f;
	_float			fCoolTimeAcc		= 0.f;

}PLAYER_DESC;

class CPlayerController final : public CBase
{
	DECLARE_SINGLETON(CPlayerController)

private:
	CPlayerController();
	virtual ~CPlayerController() = default;

public:
	HRESULT			Initialize();
	void			Tick(_double fTimeDelta);
	void			LateTick(_double fTimeDelta);

public:
	HRESULT			SetOn_Player(const PLAYER_TYPE& eType);
	HRESULT			SetOff_Player(const PLAYER_TYPE& eType);
	HRESULT			Change_ControlPlayer(const PLAYER_TYPE& eType);

	CCharacter*			Get_Player(const PLAYER_TYPE& eType);
	const PLAYER_STATE	Get_PlayerState(const PLAYER_TYPE& eType);
	const PLAYER_DESC	Get_PlayerDesc(const PLAYER_TYPE& eType);

	const _bool		Is_Controll(const PLAYER_TYPE eType);
	const _bool		Is_Player(const PLAYER_TYPE eType);

	HRESULT			Add_Player(CCharacter* pCharacter, const PLAYER_TYPE& eType);

private:
	void			Update_Player(_double fTimeDelta);
	void			Play_Sound(const PLAYER_TYPE& eType);

private:
	PLAYER_DESC		m_tPlayerDesc[PLAYER_TYPE::TYPEEND];
	PLAYER_TYPE		m_eCurControlPlayerType = PLAYER_TYPE::CHAI;

public:
	virtual void	Free() override;
};

END


