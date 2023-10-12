#pragma once

#include "Base.h"
#include "Client_Defines.h"


BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

typedef struct tagInputDesc
{
	Vec2	vMovement;

	_bool	bJump = FALSE;
	_bool	bDoubleJump = FALSE;
	_bool	bDash = FALSE;

	_bool	bWeakAttack = FALSE;
	_bool	bStrongAttack = FALSE;

	_bool	bBockInputs = FALSE;

}INPUT_DESC;

typedef struct tagTransDesc
{
    _float  fMaxForwardSpeed;

    _float  fJumpSpeed;
    _float  fGravity;
    
    _float  fMinTurnSpeed;
    _float  fMaxTurnSpeed;

    _bool   bIsGround;
    _bool   bIsJump;
    _bool   bIsDoubleJump;

    _float  fGroundedRayDistance;

    _float  fGroundAcceleration;
    _float  fGroundDeceleration;

}TRANS_DESC;

typedef struct tagFightDesc
{
    _bool   bCanAttack;
    _bool   bInAttack;
    _bool   bInCombo;

    _bool   bIsInvulneralbe;

}FIGHT_DESC;

class CController_Player final : public CBase
{
    //DECLARE_SINGLETON(CController_Player);

    enum PLAYER_TYPE { CHAI, PEPPERMINT, TYPEEND };

private:
    CController_Player();
    virtual ~CController_Player() = default;

public:
    virtual HRESULT		Initialize(void* pArg);
    virtual void		Tick(_float fTimeDelta);
    virtual void		LateTick(_float fTimeDelta);

private:
    CGameObject* m_Players[TYPEEND];
    CGameObject* m_pCurPlayer = { nullptr };

private:
    TRANS_DESC  m_TransDesc;
    INPUT_DESC  m_InputDesc;
    FIGHT_DESC  m_FightDesc;

public:
    virtual void Free() override;
};

END