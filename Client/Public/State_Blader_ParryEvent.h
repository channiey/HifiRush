#pragma once
#include "State_Blader_Base.h"

BEGIN(Client)
class CCamera_Parry;

class CState_Blader_ParryEvent final : public CState_Blader_Base
{
	enum PROGRESS_ID { PR_ZERO, PR_SET_CAM, PR_ZOOM_IN, PR_SET_PLAYER, PR_ZOOM_OUT, PROGRESS_ID_END };

private:
	CState_Blader_ParryEvent();
	CState_Blader_ParryEvent(const CState_Blader_ParryEvent& rhs);
	virtual ~CState_Blader_ParryEvent() = default;

public:
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT			Enter() override;
	const wstring	Tick(const _double& fTimeDelta) override;
	const wstring	LateTick() override;
	void			Exit() override;

public:
	const wstring	Check_Transition() override;

private:
	void Calculate_Player_Transform();
	void Set_Camera_Transform();
	void Set_Player_Look();
	void Zoom_In();
	void Set_Player_Transform();
	void Zoom_Out();

private:
	CCamera_Parry* m_pParryCam = nullptr;

	PROGRESS_ID		m_eProgressID = PR_ZERO;
	Vec4			m_vPlayerNewPos;
	const _float	m_fDistance = 15.f;

public:
	static CState_Blader_ParryEvent* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
