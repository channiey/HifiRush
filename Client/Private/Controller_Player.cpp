#include "..\Default\stdafx.h"
#include "Controller_Player.h"

#include "GameInstance.h"

#include "Chai.h"
#include "Peppermint.h"

//IMPLEMENT_SINGLETON(CController_Player)

HRESULT CController_Player::Initialize(void* pArg)
{
	return S_OK;
}

void CController_Player::Tick(_float fTimeDelta)
{
}

void CController_Player::LateTick(_float fTimeDelta)
{
}

void CController_Player::Free()
{
}
