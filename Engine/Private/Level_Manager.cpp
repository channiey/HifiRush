#include "..\Public\Level_Manager.h"
#include "Level.h"
#include "EngineInstance.h"

IMPLEMENT_SINGLETON(CLevel_Manager)

CLevel_Manager::CLevel_Manager()
{
}

void CLevel_Manager::Tick(_double fTimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return;

	m_pCurrentLevel->Tick(fTimeDelta);
}

void CLevel_Manager::LateTick(_double fTimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return;

	m_pCurrentLevel->LateTick(fTimeDelta);
}

HRESULT CLevel_Manager::Open_Level(_uint iLevelIndex, CLevel * pNewLevel)
{
	if (nullptr != m_pCurrentLevel)
	{
		CEngineInstance*	pGameInstance = CEngineInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		/* ������ �߰��Ǿ��ִ� ������ �ڿ��� �����Ѵ�. */
		pGameInstance->Clear(m_iCurrentLevelIndex);

		Safe_Release(pGameInstance);
	}

	/* �������� ��ü�� �Ăt����. */
	Safe_Release(m_pCurrentLevel);

	m_pCurrentLevel = pNewLevel;

	m_iCurrentLevelIndex = iLevelIndex;

	return S_OK;
}

void CLevel_Manager::Free()
{
	Safe_Release(m_pCurrentLevel);
}
