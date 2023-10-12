#pragma once

#include "Base.h"
#include "Client_Defines.h"

BEGIN(Engine)


END

BEGIN(Client)
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
    //CGameObject* m_Players[TYPEEND];
    //CGameObject* m_pCurPlayer = { nullptr };

public:
    virtual void Free() override;
};

END