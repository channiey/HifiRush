#pragma once

#include "Client_Defines.h"

#include "Blackboard.h"

#include "Saber.h"

BEGIN(Client)

class CBlackboard_Saber final : public CBlackboard
{
private:
	CBlackboard_Saber(class CGameObject* pGameObject);
	CBlackboard_Saber(const CBlackboard_Saber& rhs);
	virtual ~CBlackboard_Saber() = default;

public:
	CSaber*				m_pSaber			= { nullptr };
	
	const _float		m_fCanBattleDist	= { 10.f };
	const _float		m_fCanAttackDist	= { 3.f };
	_float				m_fCurTargetDist	= { 0.f };

	_float				m_fAttackFreqTime	= { 5.f };
	_float				m_fAttackAccTime	= { 0.f };

	ANIM_SA				m_ePrevAttackAnim	= ANIM_SA::ATTACK_JUMP_CUT;
	ANIM_SA				m_ePrevEscapeAnim	= ANIM_SA::DOUBLE_STEP_LEFT_00;

	ANIM_SA				m_eCurAttackAnim	= ANIM_SA::ATTACK_JUMP_CUT;
	ANIM_SA				m_eCurEscapeAnim	= ANIM_SA::DOUBLE_STEP_LEFT_00;

public:
	static CBlackboard_Saber* Create(CGameObject* pGameObject);
	virtual void Free();
};

END
