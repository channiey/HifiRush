#pragma once
#include "State_Blader_Base.h"

BEGIN(Client)
class CState_Blader_Dead final : public CState_Blader_Base
{
private:
	CState_Blader_Dead();
	CState_Blader_Dead(const CState_Blader_Dead& rhs);
	virtual ~CState_Blader_Dead() = default;

public:
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT Enter() override;
	const wstring Tick(const _double& fTimeDelta) override;
	const wstring LateTick() override;
	void Exit() override;

public:
	const wstring Check_Transition() override;

public:
	static CState_Blader_Dead* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
