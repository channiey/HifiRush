#pragma once
#include "State_Macaron_Base.h"

BEGIN(Client)
class CState_Macaron_Gimmick final : public CState_Macaron_Base
{
	enum PROGRESS_ID 
	{
		APPEAR,
		TIMMING,
		SUCCESS_DISAPPEAR,
		FAILURE_DISAPPEAR,
		PROGRESS_END
	};

	enum class RESULT_TYPE
	{
		SUCCESS,
		FAILURE,
		NONE,
		TYPEEND 
	};

private:
	CState_Macaron_Gimmick();
	CState_Macaron_Gimmick(const CState_Macaron_Gimmick& rhs);
	virtual ~CState_Macaron_Gimmick() = default;

public:
	virtual HRESULT		Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT				Enter() override;
	const wstring		Tick(const _double& fTimeDelta) override;
	const wstring		LateTick() override;
	void				Exit() override;

public:
	const wstring		Check_Transition() override;

protected:
	virtual void		Check_Progress(const _double& fTimeDelta) override;
	virtual void		Set_Transform() override;
	virtual void		Play_Sound() override;

private:
	void				Set_UI(const _bool& bActive);

private:
	PROGRESS_ID			m_eProgressID = PROGRESS_ID::PROGRESS_END;
	const _float		m_fTimeLimit = 4.f;
	_float				m_fAcc = 0.f;

	RESULT_TYPE			m_eResultType = RESULT_TYPE::NONE;
	

public:
	static CState_Macaron_Gimmick* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
