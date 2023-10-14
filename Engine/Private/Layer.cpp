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

		/* Ŭ�� ���� ��ȣ�� �ڸ� �̸��� ���Ѵ�. */
		if (strName == Util_String::Remove_LastNumChar(iter->Get_Name(), CLONE_PIN_MAX_DIGIT))
		{
			/* �̸��� ���ٸ� ������ȣ�� ����ͼ� ��Һ񱳸� ���� �ִ񰪿� ���������� �����Ѵ�. */
			/* ����ٸ� �����ϱ� ���� - 1�� �Ѵ�. */
			_int iPinNum = stoi(Util_String::Get_LastNumChar(iter->Get_Name(), CLONE_PIN_MAX_DIGIT - 1));

			if (CloneMaxNum < iPinNum)
				CloneMaxNum = iPinNum;
		}
	}

	/* �ִ��� -1�̶�� ���� �ش� �̸��� ���� Ŭ���� �����Ƿ� ������ȣ�� 000���� �����Ѵ�. */
	if (-1 == CloneMaxNum)
		return strName + L"_000";
	else if(0 == CloneMaxNum) /* 0�̶�� ���� Ŭ���� �� �ϳ� �����Ѵٴ� ��*/
		return strName + L"_001";
	else /* �ƴ϶�� �ִ񰩿� + 1�� �Ͽ� ���� ��ȣ�� �����Ѵ�. */
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
	Safe_Release(pGameObject); /* TODO : iter�� Safe_Release()�� �ȵǳ�? */

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
	/* 1�� �ڽĿ� ���� ������Ʈ�� �θ��� ������Ʈ ���� ����ȴ�. (2��, 3�������� ���� �̱���) */

	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr == pGameObject || !pGameObject->Is_Active() || pGameObject->Is_Parent()) continue;

		/* �θ� ����*/
		pGameObject->Tick(fTimeDelta);

		/* 1�� �ڽ� ���� */
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
