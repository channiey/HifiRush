#include "../Default/stdafx.h"
#include "EffcetManager.h"

#include "EngineInstance.h"
#include "Effect.h"

IMPLEMENT_SINGLETON(CEffectManager)

CEffectManager::CEffectManager()
{
}

HRESULT CEffectManager::Initialize()
{
	return S_OK;
}

CEffect* CEffectManager::Get_Effect(const EFFECT_ID& eID)
{
	return Find_Effect(eID);
}

HRESULT CEffectManager::Add_Effect(CEffect* pEffect)
{
	if (nullptr == pEffect)
		return E_FAIL;

	_uint iID = pEffect->Get_EffectID();

	if (CEffectManager::EFFECT_ID::ID_END <= iID)
		return E_FAIL;

	if (nullptr != Find_Effect((CEffectManager::EFFECT_ID)iID))
		return E_FAIL;

	m_Effects.emplace((CEffectManager::EFFECT_ID)iID, pEffect);

	return S_OK;
}

CEffect* CEffectManager::Find_Effect(const EFFECT_ID& eID)
{
	auto iter = m_Effects.find(eID);

	if (iter == m_Effects.end())
		return nullptr;

	return iter->second;
}

void CEffectManager::Free()
{
	__super::Free();
}

