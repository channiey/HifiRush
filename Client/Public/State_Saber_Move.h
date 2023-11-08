#pragma once
#include "State_Saber_Base.h"

BEGIN(Client)
class CState_Saber_Move final : public CState_Saber_Base
{
	enum class DIR_TYPE { FORWARD, BACKWARD, LEFT, RIGHT, TYPEEND };

private:
	CState_Saber_Move();
	CState_Saber_Move(const CState_Saber_Move& rhs);
	virtual ~CState_Saber_Move() = default;

public:
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT Enter() override;
	const wstring Tick(const _double& fTimeDelta) override;
	const wstring LateTick() override;
	void Exit() override;

public:
	const wstring Check_Transition() override;

private:
	void					Set_Direction();
	virtual void			Set_NextAnimation() override;

private:
	DIR_TYPE m_eDirType = DIR_TYPE::TYPEEND;
	_bool	m_bLookTarget = FALSE;

public:
	static CState_Saber_Move* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
