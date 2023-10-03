#include "..\Public\GameObject.h"
#include "GameInstance.h"

CGameObject::CGameObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pContext(rhs.m_pContext)
	, m_bActive(rhs.m_bActive)
	, m_bRender(rhs.m_bRender)
	, m_strLayer(rhs.m_strLayer)
	, m_strName(rhs.m_strName)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	return S_OK;
}

void CGameObject::Tick(_float fTimeDelta)
{
}

void CGameObject::LateTick(_float fTimeDelta)
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

CTransform* const CGameObject::Get_Transform()
{
	return dynamic_cast<CTransform*>(Get_Component(TEXT("Com_Transform")));
}

CVIBuffer* CGameObject::Get_VIBuffer()
{
	return dynamic_cast<CVIBuffer*>(Get_Component(TEXT("Com_VIBuffer")));
}

CMonoBehaviour* const CGameObject::Get_MonoBehaviour(const _uint& iIndex)
{
	if(m_MonoBehaviours.size() > iIndex)
		return nullptr;
	
	return m_MonoBehaviours[iIndex];
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const wstring & strPrototypeTag, const wstring & strComponentTag, _Inout_ CComponent** ppOut, void * pArg)
{
	/* �߰��Ϸ��� ������Ʈ�� �̹� �ִ� ��� ���� ���� */
	if (nullptr != Find_Component(strComponentTag))
		return E_FAIL;

	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* ���� ������Ʈ�� �����Ͽ� �纻 ������Ʈ�� ������. */
	CComponent*			pComponent = pGameInstance->Clone_Component(iLevelIndex, strPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	/* �˻��� ������ �ʿ� ����. */
	m_Components.emplace(strComponentTag, pComponent);

	/* �ڽ��� ����������� ����. */
	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	Safe_Release(pGameInstance);	

	return S_OK;
}

CComponent * CGameObject::Find_Component(const wstring & strComponentTag)
{
	auto	iter = m_Components.find(strComponentTag);

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;	
}

void CGameObject::Free()
{
	__super::Free();

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);

	m_Components.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
