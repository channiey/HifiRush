#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CEffect;

class CEffectManager final : public CBase
{
	DECLARE_SINGLETON(CEffectManager)

public:
	enum EFFECT_ID { TRAIL_SWORD_CHAI, ID_END };

private:
	CEffectManager();
	virtual ~CEffectManager() = default;

public:
	HRESULT			Initialize();

public:
	CEffect*		Get_Effect(const EFFECT_ID& eID);
	HRESULT			Add_Effect(CEffect* pEffect);

private:
	CEffect*		Find_Effect(const EFFECT_ID& eID);

private:
	map<EFFECT_ID, CEffect*>	m_Effects;

public:
	virtual void	Free() override;
};

END


