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
	/* ��Ʋ Ʈ���� ���� Ŭ���� ���� */
	/* Ű�� ��ƲƮ���Ÿ� ���� �ϴ� ���� ������ �ִ´�. */
	/* ���� ��Ʋ ������, ��Ʋ�̶�� � Ʈ���Ű� �۵��ϰ� �ִ��� ���´�.*/
	/* ��Ʋ Ʈ���Ŵ� �÷��̾�� �浹�� ����� �޽����� ������. */

public:
	virtual void	Free() override;
};

END


