#include "..\Public\Layer.h"
#include "GameObject.h"

#include "GameInstance.h"
#include "StringUtils.h"

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
		if (strName == StringUtils::Remove_LastNumChar(iter->Get_Name(), CLONE_PIN_MAX_DIGIT))
		{
			/* �̸��� ���ٸ� ������ȣ�� ����ͼ� ��Һ񱳸� ���� �ִ񰪿� ���������� �����Ѵ�. */
			/* ����ٸ� �����ϱ� ���� - 1�� �Ѵ�. */
			_int iPinNum = stoi(StringUtils::Get_LastNumChar(iter->Get_Name(), CLONE_PIN_MAX_DIGIT - 1));

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
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject && pGameObject->Is_Active())
			pGameObject->Tick(fTimeDelta);
	}
}

void CLayer::LateTick(_float fTimeDelta)
{
	//if (3 < m_GameObjects.size())
	//	CThread_Manager::GetInstance()->Set_MultiThreading(3);
	
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject && pGameObject->Is_Active())
		{
			//if (3 < m_GameObjects.size())
			//{
			//	CThread_Manager::GetInstance()->Add_Command(
			//		[&pGameObject, fTimeDelta]() { pGameObject->LateTick(fTimeDelta); }
			//	);
			//}
			//else
				pGameObject->LateTick(fTimeDelta);
			/*CThread_Manager::GetInstance()->Add_Command(std::bind(&CGameObject::LateTick, &pGameObject, (_float)fTimeDelta));
			pGameObject->LateTick(fTimeDelta);*/
		}
	}
	//if (23 < m_GameObjects.size())
	//	CThread_Manager::GetInstance()->Finish_MultiThreading();
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
