#pragma once
#include "State_Chai_Base.h"

BEGIN(Client)
class CState_Chai_Attack final : public CState_Chai_Base
{ 
	enum class ATTACK_TYPE { LIGHT, STRONG, THROW, SPECIAL, TYPEEND };

	typedef struct tagAttackDesc
	{
		_bool		bFirstAttack	= FALSE;
		_bool		bSet			= FALSE;
		_int		iAnimIndex		= -1;
		_double		dTimePerFrame	= 0.f;
		_float		fTweenTime		= 0.f;

		ATTACK_TYPE				eAttackType		= ATTACK_TYPE::TYPEEND;
		CModel::ANIM_PROGRESS	eAnimCheckType	= CModel::ANIM_PROGRESS::PROGRESS_END;

		void Reset()
		{
			bFirstAttack	= FALSE;
			bSet			= FALSE;
			iAnimIndex		= -1;
			dTimePerFrame	= 0.f;
			fTweenTime		= 0.f;
		
			eAttackType		= ATTACK_TYPE::TYPEEND;
			eAnimCheckType	= CModel::ANIM_PROGRESS::PROGRESS_END;
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
	const wstring&		Tick(const _double& fTimeDelta) override;
	const wstring&		LateTick() override;
	void				Exit() override;

private:
	virtual void		OnCollision_Enter(CGameObject* pGameObject) override;
	virtual void		OnCollision_Stay(CGameObject* pGameObject) override;
	virtual void		OnCollision_Exit(CGameObject* pGameObject) override;

public:
	const wstring&		Check_Transition() override;

private:
	void				Set_AttackDesc();

private:
	ATTACK_DESC			m_tAttackDesc	= {};


public:
	static CState_Chai_Attack* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
