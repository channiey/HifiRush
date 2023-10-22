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
	
	const _float		m_fCanAttackDist	= { 5.f };

	_float				m_fAttackFreqTime	= { 1.f };
	_float				m_fAttackAccTime	= { 0.f };

	/* 초반 아이들 리셋시 사용 */
	_bool				m_bResetIdle		= FALSE;
	_float				m_fResetTime		= { 1.f };
	_float				m_fResetAccTime		= { 0.f };

public:
	static CBlackboard_Saber* Create(CGameObject* pGameObject);
	virtual void Free();
};

END
