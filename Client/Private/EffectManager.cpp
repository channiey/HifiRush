#include "../Default/stdafx.h"
#include "EffectManager.h"

#include "EngineInstance.h"
#include "Effect.h"

#include "Effect_Trail_Sword.h"
#include "Effect_Damaged_Enemy.h"


IMPLEMENT_SINGLETON(CEffectManager)

CEffectManager::CEffectManager()
{
}

HRESULT CEffectManager::Initialize()
{
	if (FAILED(Ready_Effect()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffectManager::Ready_Effect()
{
	CGameObject*	pObject = nullptr; 
	CEffect*		pEffect = nullptr;

	/* For.Effect_Trail_Sword */
	{
		pObject = ENGINE_INSTANCE->Add_GameObject(ENGINE_INSTANCE->Get_CurLoadingLevel(), LayerNames[LAYER_EFFECT], L"Effect_Trail_Sword");
		if (nullptr == pObject)
			return E_FAIL;

		pEffect = dynamic_cast<CEffect*>(pObject);
		if (nullptr == pEffect)
			return E_FAIL;

		m_Effects.emplace(EFFECT_ID(pEffect->Get_EffectID()), pEffect);
	}

	/* For.Effect_Damaged_Enemy */
	{
		if (FAILED(ENGINE_INSTANCE->Reserve_Pool(LEVEL_ID::LV_STAGE_01, LayerNames[LAYER_EFFECT], L"Effect_Damaged_Enemy", 5)))
			return E_FAIL;
	}
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

