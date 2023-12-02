#pragma once
#include "State_Chai_Base.h"

BEGIN(Client)
class CState_Chai_Attack final : public CState_Chai_Base
{ 
	enum class ATTACK_TYPE { LIGHT, HEAVY, THROW, SPECIAL, TYPEEND };

	typedef struct tagAttackDesc
	{
		_bool		bFirstAttack = FALSE;
		_bool		bSet = FALSE;
		_int		iAnimIndex = -1;
		_double		dTimePerFrame = 0.f;
		_float		fTweenTime = 0.f;

		ATTACK_TYPE				eAttackType = ATTACK_TYPE::TYPEEND;
		_double					dPrevAnimCheckFrame = -1.f;

		CModel::SoundEventDesc tSoundEventDesc = {};

		void Reset()
		{
			bFirstAttack	= FALSE;
			bSet			= FALSE;
			iAnimIndex		= -1;
			dTimePerFrame	= 0.f;
			fTweenTime		= 0.f;
		
			eAttackType		= ATTACK_TYPE::TYPEEND;
			dPrevAnimCheckFrame = -1.f;

			tSoundEventDesc.Clear();
		}

	}ATTACK_DESC;

private:
	CState_Chai_Attack();
	CState_Chai_Attack(const CState_Chai_Attack& rhs);
	virtual ~CState_Chai_Attack() = default;

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
	virtual HRESULT		Ready_Effect() override;
	virtual HRESULT		Set_Effect() override;

private:
	void				Set_AttackDesc();
	void				Detect_AttackCollision();
	void				KnockBack(CCharacter* pTarget);

	void				PlayThrowSound();
	void				LockOn();

private:
	ATTACK_DESC			m_tAttackDesc	= {};
	ATTACK_DESC			m_tAttackDescForCol = {};

	_bool				m_bThrowGuitar = FALSE;
	_bool				m_bThrowSound = FALSE;


public:
	static CState_Chai_Attack* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
