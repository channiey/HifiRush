#pragma once
#include "State_Chai_Base.h"

BEGIN(Client)

class CUi_SpecialAttack;

class CState_Chai_SpecialAttack final : public CState_Chai_Base
{
	enum class SPC_ATK_TYPE
	{ 
		HIBIKI, 
		POWER_CHORD,
		TYPEEND
	};

private:
	CState_Chai_SpecialAttack();
	CState_Chai_SpecialAttack(const CState_Chai_SpecialAttack& rhs);
	virtual ~CState_Chai_SpecialAttack() = default;

public:
	virtual HRESULT		Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT				Enter() override;
	const wstring		Tick(const _double& fTimeDelta) override;
	const wstring		LateTick() override;
	void				Exit() override;

public:
	const wstring		Check_Transition() override;

private:
	void				KnockBack(CCharacter* pTarget);
	void				Update_Camera(const _double& fTimeDelta);
	void				Update_Sound();
	HRESULT				Set_UI();

	void				LockOn();

private:
	SPC_ATK_TYPE		m_eAtkType = SPC_ATK_TYPE::TYPEEND;
	CUi_SpecialAttack*	m_pUI = nullptr;

	_bool				m_bSoundHibiki = FALSE;
	_bool				m_bSoundPowerChord = FALSE;

public:
	static CState_Chai_SpecialAttack* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
