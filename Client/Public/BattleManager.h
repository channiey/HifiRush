#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CBattleManager final : public CBase
{
	DECLARE_SINGLETON(CBattleManager)

private:
	CBattleManager();
	virtual ~CBattleManager() = default;

public:
	HRESULT			Initialize();

public:




private:
	/* 배틀 트리거 별도 클래스 생성 */
	/* 키와 배틀트리거를 페어로 하는 맵을 가지고 있는다. */
	/* 현재 배틀 중인지, 배틀이라면 어떤 트리거가 작동하고 있는지 갖는다.*/
	/* 배틀 트리거는 플레이어와 충돌시 여기로 메시지를 보낸다. */

public:
	virtual void	Free() override;
};

END


