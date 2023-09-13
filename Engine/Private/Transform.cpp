#include "..\Public\Transform.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
{
}

void CTransform::Set_State(STATE eState, _fvector vState)
{
	_matrix		StateMatrix;

	StateMatrix = XMLoadFloat4x4(&m_WorldMatrix);

	StateMatrix.r[eState] = vState;

	XMStoreFloat4x4(&m_WorldMatrix, StateMatrix);
}

HRESULT CTransform::Initialize_Prototype()
{
	/* XMFloat4x4 -> XMMatrix*/
	/*XMLoadFloat4x4(&m_WorldMatrix);*/

	/* XMMatrix -> XMFloat4x4*/
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());	

	/*_float3		vTmp;
	_vector		vTmp1;

	vTmp1 = XMLoadFloat3(&vTmp);

	XMStoreFloat3(&vTmp, vTmp1);*/


	return S_OK;
}

HRESULT CTransform::Initialize(void * pArg)
{
	if (nullptr != pArg)
		memmove(&m_tTrans, pArg, sizeof TRANSFORM_DESC);

	return S_OK;
}

void CTransform::Move_Forward(_float fTimeDelta)
{
	_vector		vLook = Get_State(STATE_LOOK);

	_vector		vPosition = Get_State(STATE_POSITION);

	vPosition += XMVector3Normalize(vLook) * m_tTrans.fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Move_Backward(_float fTimeDelta)
{
	_vector		vLook = Get_State(STATE_LOOK);

	_vector		vPosition = Get_State(STATE_POSITION);

	vPosition -= XMVector3Normalize(vLook) * m_tTrans.fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Move_Left(_float fTimeDelta)
{
	_vector		vRight = Get_State(STATE_RIGHT);

	_vector		vPosition = Get_State(STATE_POSITION);

	vPosition -= XMVector3Normalize(vRight) * m_tTrans.fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Move_Right(_float fTimeDelta)
{
	_vector		vRight = Get_State(STATE_RIGHT);

	_vector		vPosition = Get_State(STATE_POSITION);

	vPosition += XMVector3Normalize(vRight) * m_tTrans.fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Fix_Rotation(_fvector vAxis, _float fRadian)
{
}

void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
}

CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTransform*	pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform*	pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();

}
