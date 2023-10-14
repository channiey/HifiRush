#include "..\Public\Layer.h"
#include "GameObject.h"

#include "GameInstance.h"
#include "Util_String.h"

CLayer::CLayer()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	m_pGameInstance->AddRef();
}

HRESULT CLayer::Initialize(const wstring& strLayerTag)
{
	m_strName = strLayerTag;

	return S_OK;
}

const wstring CLayer::Get_CloneNameWithPin(const wstring& strName)
{
	_int CloneMaxNum = -1;
	wstring strPin = L"";

	for (auto& iter : m_GameObjects)
	{
		if (nullptr == iter) continue;

		/* 클론 고유 번호를 자른 이름과 비교한다. */
		if (strName == Util_String::Remove_LastNumChar(iter->Get_Name(), CLONE_PIN_MAX_DIGIT))
		{
			/* 이름이 같다면 고유번호만 갖고와서 대소비교를 통해 최댓값에 저장할지를 결정한다. */
			/* 언더바를 제외하기 위해 - 1을 한다. */
			_int iPinNum = stoi(Util_String::Get_LastNumChar(iter->Get_Name(), CLONE_PIN_MAX_DIGIT - 1));

			if (CloneMaxNum < iPinNum)
				CloneMaxNum = iPinNum;
		}
	}

	/* 최댓값이 -1이라는 것은 해당 이름과 같은 클론이 없으므로 고유번호를 000으로 세팅한다. */
	if (-1 == CloneMaxNum)
		return strName + L"_000";
	else if(0 == CloneMaxNum) /* 0이라는 것은 클론이 딱 하나 존재한다는 것*/
		return strName + L"_001";
	else /* 아니라면 최댓갑에 + 1을 하여 고유 번호로 세팅한다. */
	{
		_int iPinNum = CloneMaxNum + 1;

		if (0 == iPinNum / 10)
			strPin = L"_00" + to_wstring(iPinNum);
		else if (0 == iPinNum / 100)
			strPin = L"_0" + to_wstring(iPinNum);
		else
			strPin = L"_" + to_wstring(iPinNum);

		return strName + strPin;
	}
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

HRESULT CLayer::Delete_GameObject(CGameObject* pGameObject)
{
	auto iter = find(m_GameObjects.begin(), m_GameObjects.end(), pGameObject);

	if (m_GameObjects.end() == iter)
		return E_FAIL;

	m_GameObjects.erase(iter);
	Safe_Release(pGameObject); /* TODO : iter는 Safe_Release()가 안되네? */

	return S_OK;
}

HRESULT CLayer::Erase_GameObject(CGameObject* pGameObject)
{
	auto iter = find(m_GameObjects.begin(), m_GameObjects.end(), pGameObject);

	if (m_GameObjects.end() == iter)
		return E_FAIL;

	m_GameObjects.erase(iter);

	return S_OK;
}

HRESULT CLayer::Push_GameObject(CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

void CLayer::Tick(_float fTimeDelta)
{
	/* 1차 자식에 대한 업데이트는 부모의 업데이트 이후 진행된다. (2차, 3차까지는 아직 미구현) */

	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr == pGameObject || !pGameObject->Is_Active() || pGameObject->Is_Parent()) continue;

		/* 부모 갱신*/
		pGameObject->Tick(fTimeDelta);

		/* 1차 자식 갱신 */
		if (pGameObject->Is_Child())
		{
			const vector<CGameObject*> Children = pGameObject->Get_Children();

			for (auto& pChild : Children)
			{
				if (nullptr == pGameObject || !pGameObject->Is_Active()) continue;

				pChild->Tick(fTimeDelta);
			}
		}
	}
}

void CLayer::LateTick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr == pGameObject || !pGameObject->Is_Active() || pGameObject->Is_Parent()) continue;

		pGameObject->LateTick(fTimeDelta);

		if (pGameObject->Is_Child())
		{
			const vector<CGameObject*> Children = pGameObject->Get_Children();

			for (auto& pChild : Children)
			{
				if (nullptr == pGameObject || !pGameObject->Is_Active()) continue;

				pChild->LateTick(fTimeDelta);
			}
		}
	}
}

CGameObject* CLayer::Get_GameObject(const wstring& strPrototypeTag)
{
	for (CGameObject* pObj : m_GameObjects)
	{
		if (pObj->Get_Name() == strPrototypeTag)
			return pObj;
	}
	return nullptr;
}

const _int CLayer::Get_ObjectIndex(const wstring& strName)
{
	int iIndex = 0;

	for (CGameObject* pObj : m_GameObjects)
	{
		++iIndex;

		if (pObj->Get_Name() == strName)
			return iIndex;
	}

	return -1;
}

CLayer * CLayer::Create(const wstring& strLayerTag)
{
	CLayer*	pInstance = new CLayer();

	if (FAILED(pInstance->Initialize(strLayerTag)))
	{
		MSG_BOX("Failed to Created : CLayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLayer::Free()
{
	__super::Free();

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();
	m_pGameInstance->Release();
}
