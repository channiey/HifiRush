#pragma once
#include "State_Chai_Base.h"

BEGIN(Client)
class CState_Chai_Run final : public CState_Chai_Base
{
private:
	CState_Chai_Run();
	CState_Chai_Run(const CState_Chai_Run& rhs);
	virtual ~CState_Chai_Run() = default;

public:
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT Enter() override;
	const wstring& Tick(const _float& fTimeDelta) override;
	const wstring& LateTick() override;
	void Exit() override;

public:
	const wstring& Check_Transition() override;


private:
	void Move(const _float& fTimeDelta);

	Vec3 m_vScale = {};

public:
	static CState_Chai_Run* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
