#pragma once
#include "State_Saber_Base.h"

BEGIN(Client)
class CState_Saber_Damaged final : public CState_Saber_Base
{
private:
	CState_Saber_Damaged();
	CState_Saber_Damaged(const CState_Saber_Damaged& rhs);
	virtual ~CState_Saber_Damaged() = default;

public:
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT Enter() override;
	const wstring Tick(const _double& fTimeDelta) override;
	const wstring LateTick() override;
	void Exit() override;

public:
	const wstring Check_Transition() override;

	void Damaged();
	
private :
	void PlayEffect();

	_bool			m_bPlayEffect = FALSE;


public:
	static CState_Saber_Damaged* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
