#include "..\Public\Collider.h"
#include "PipeLine.h"

#include "GameObject.h"

_uint CCollider::g_iNextID = 0;

CCollider::CCollider(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
	//, m_bCollision(rhs.m_bCollision)
	, m_eColliderType(rhs.m_eColliderType)
#ifdef _DEBUG
	, m_pBatch(rhs.m_pBatch)
	, m_pEffect(rhs.m_pEffect)
	, m_pInputLayout(rhs.m_pInputLayout)
#endif // _DEBUG
{
#ifdef _DEBUG
	Safe_AddRef(m_pInputLayout);
#endif // _DEBUG
}

HRESULT CCollider::Initialize_Prototype(TYPE eColliderType)
{
	m_eColliderType = eColliderType;

#ifdef _DEBUG

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);

	m_pEffect->SetVertexColorEnabled(true);

	const void*		pShaderByteCodes = nullptr;
	size_t			iLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCodes, &iLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCodes, iLength, &m_pInputLayout)))
		return E_FAIL;
#endif

	return S_OK;
}

HRESULT CCollider::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	m_iID = g_iNextID++;

	memcpy(&m_ColliderDesc, pArg, sizeof(COLLIDERDESC));

	return S_OK;
}

void CCollider::OnCollision_Enter(CGameObject* pGameObject)
{
	++m_iCollison;
	pGameObject->OnCollision_Enter(pGameObject);
}

void CCollider::OnCollision_Stay(CGameObject* pGameObject)
{
	pGameObject->OnCollision_Stay(pGameObject);
}

void CCollider::OnCollision_Exit(CGameObject* pGameObject)
{
	--m_iCollison;
	pGameObject->OnCollision_Exit(pGameObject);
}

_matrix CCollider::Remove_Rotation(_fmatrix Matrix)
{
	_matrix	ResultMatrix = Matrix;

	ResultMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVector3Length(Matrix.r[0]);
	ResultMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVector3Length(Matrix.r[1]);
	ResultMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVector3Length(Matrix.r[2]);

	return ResultMatrix;
}

void CCollider::Free()
{
	__super::Free();

#ifdef _DEBUG
	if (false == m_bClone)
	{
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pEffect);
	}

	Safe_Release(m_pInputLayout);
	
#endif // _DEBUG

}

#ifdef _DEBUG

HRESULT CCollider::Render()
{
	m_vColor = Is_Collision() == false ? _float4(0.f, 1.f, 0.f, 1.f) : _float4(1.f, 0.f, 0.f, 1.f);

	m_pEffect->SetWorld(XMMatrixIdentity());

	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	m_pEffect->SetView(pPipeLine->Get_Transform(CPipeLine::STATE_VIEW));
	m_pEffect->SetProjection(pPipeLine->Get_Transform(CPipeLine::STATE_PROJ));

	RELEASE_INSTANCE(CPipeLine);

	m_pEffect->Apply(m_pContext);

	m_pContext->IASetInputLayout(m_pInputLayout);

	return S_OK;
}
#endif // _DEBUG
