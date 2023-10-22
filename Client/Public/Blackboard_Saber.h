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

	_float				m_fAttackFreqTime	= { 5.f };
	_float				m_fAttackAccTime	= { 0.f };

public:
	static CBlackboard_Saber* Create(CGameObject* pGameObject);
	virtual void Free();
};

END
