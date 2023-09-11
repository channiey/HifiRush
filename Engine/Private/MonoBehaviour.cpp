#include "..\Public\MonoBehaviour.h"

CMonoBehaviour::CMonoBehaviour(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)

{

}

CMonoBehaviour::CMonoBehaviour(const CMonoBehaviour& rhs)
	: CComponent(rhs)
{
	
}

HRESULT CMonoBehaviour::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonoBehaviour::Initialize(void * pArg)
{
	return S_OK;
}

void CMonoBehaviour::Tick(_float fTimeDelta)
{
}

void CMonoBehaviour::LateTick(_float fTimeDelta)
{
}

HRESULT CMonoBehaviour::Render()
{
	return S_OK;
}

CComponent* CMonoBehaviour::Clone(void* pArg)
{
	return nullptr;
}

void CMonoBehaviour::Free()
{
	__super::Free();
}
