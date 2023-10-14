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
	//, m_bActive(rhs.m_bActive)
	, m_eState(rhs.m_eState)
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
	Compute_CamZ(Get_Transform()->Get_State(CTransform::STATE_POSITION));
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

CTransform* CGameObject::Get_Transform()
{
	return dynamic_cast<CTransform*>(Get_Component(TEXT("Com_Transform")));
}

CVIBuffer* CGameObject::Get_VIBuffer()
{
	return dynamic_cast<CVIBuffer*>(Get_Component(TEXT("Com_VIBuffer")));
}

CCollider_Sphere* CGameObject::Get_Collider_Sphere()
{
	return dynamic_cast<CCollider_Sphere*>(Get_Component(TEXT("Com_Collider_Sphere")));
}

CCollider_AABB* CGameObject::Get_Collider_AABB()
{
	return dynamic_cast<CCollider_AABB*>(Get_Component(TEXT("Com_Collider_AABB")));
}

CCollider_OBB* CGameObject::Get_Collider_OBB()
{
	return dynamic_cast<CCollider_OBB*>(Get_Component(TEXT("Com_Collider_OBB")));
}

CStateMachine* CGameObject::Get_StateMachine()
{
	return dynamic_cast<CStateMachine*>(Get_Component(TEXT("Com_StateMachine")));
}

CCamera* CGameObject::Get_Camera()
{
	return dynamic_cast<CCamera*>(Get_Component(TEXT("Com_Camera")));
}

CModel* CGameObject::Get_Model()
{
	return dynamic_cast<CModel*>(Get_Component(TEXT("Com_Model")));
}

CShader* CGameObject::Get_Shader()
{
	return dynamic_cast<CShader*>(Get_Component(TEXT("Com_Shader")));
}

CMonoBehaviour* const CGameObject::Get_MonoBehaviour(const _uint& iIndex)
{
	if(m_MonoBehaviours.size() > iIndex)
		return nullptr;
	
	return m_MonoBehaviours[iIndex];
}

void CGameObject::Set_State(const OBJ_STATE& eState)
{
	for (auto& pChild : m_Children)
		pChild->Set_State(eState);

	m_eState = eState;
}

void CGameObject::Set_Render(const _bool& bRender)
{
	for (auto& pChild : m_Children)
		pChild->Set_Render(bRender);

	m_bRender = bRender;
}

HRESULT CGameObject::Remove_Child(const _uint& iIndex)
{
	if (m_Children.size() <= iIndex)
		return E_FAIL;

	return S_OK;
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const wstring & strPrototypeTag, const wstring & strComponentTag, _Inout_ CComponent** ppOut, void * pArg)
{
	/* 추가하려는 컴포넌트가 이미 있는 경우 실패 리턴 */
	if (nullptr != Find_Component(strComponentTag))
		return E_FAIL;

	CComponent* pComponent = GAME_INSTNACE->Clone_Component(iLevelIndex, strPrototypeTag, pArg);

	if (nullptr == pComponent)
		return E_FAIL;

	pComponent->Set_Owner(this);

	/* 검색이 가능한 맵에 저장. */
	m_Components.emplace(strComponentTag, pComponent);

	/* 자식의 멤버변수에도 저장. */
	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	return S_OK;
}

CComponent * CGameObject::Find_Component(const wstring & strComponentTag)
{
	auto	iter = m_Components.find(strComponentTag);

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;	
}

HRESULT CGameObject::Compute_CamZ(_fvector vWorldPos)
{
	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);
	{
		_fvector		vCamPos = pPipeLine->Get_CamPosition();

		m_fCamDistance = XMVectorGetX(XMVector3Length(vWorldPos - vCamPos));
	}
	RELEASE_INSTANCE(CPipeLine);
	return S_OK;
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
