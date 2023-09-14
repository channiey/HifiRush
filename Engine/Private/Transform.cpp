#include "..\Public\Transform.h"
#include "Shader.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{

}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{
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

HRESULT CTransform::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		TRANSFORM_DESC	TransformDesc;

		memmove(&TransformDesc, pArg, sizeof TransformDesc);

		m_tTrans.fSpeedPerSec = TransformDesc.fSpeedPerSec;
		m_tTrans.fRotRadPerSec = TransformDesc.fRotRadPerSec;
	}

	return S_OK;
}

const _float3 CTransform::Get_Scale()
{
	/* XMVector3Length : 인자로 받은 벡터의 길이를 리턴 벡터 원소 모두에 똑같이 할당한다. */
	/* 벡터의 길이가 2라면 2, 2, 2 벡터를 반환한다. */

	return _float3(XMVectorGetX(XMVector3Length(Get_State(STATE_RIGHT))),
		XMVectorGetX(XMVector3Length(Get_State(STATE_UP))),
		XMVectorGetX(XMVector3Length(Get_State(STATE_LOOK))));
}

void CTransform::Set_State(STATE eState, _fvector vState)
{
	_matrix		StateMatrix;

	StateMatrix = XMLoadFloat4x4(&m_WorldMatrix);

	StateMatrix.r[eState] = vState;

	XMStoreFloat4x4(&m_WorldMatrix, StateMatrix);
}

void CTransform::Set_Scale(const _float3& vScale)
{
	_vector		vRight = Get_State(STATE_RIGHT);
	_vector		vUp = Get_State(STATE_UP);
	_vector		vLook = Get_State(STATE_LOOK);

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

HRESULT CTransform::Bind_ShaderResources(CShader* pShader, const char* pConstantName)
{
	/* 셰이더에 월드 행렬을 바인딩한다. */

	return pShader->Bind_Matrix(pConstantName, &m_WorldMatrix);
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

void CTransform::Set_Rotation(_fvector vAxis, _float fRadian)
{
	_float3		vScaled = Get_Scale();

	/* 항등상태 기준으로 정해준 각도만큼 회전시켜놓는다. */
	/* Right, Up, Look를 회전시킨다. */
	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScaled.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScaled.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScaled.z;

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	vRight	= XMVector4Transform(vRight, RotationMatrix);
	vUp		= XMVector4Transform(vUp, RotationMatrix);
	vLook	= XMVector4Transform(vLook, RotationMatrix);
	/*XMVector3TransformNormal();
	XMVector3TransformCoord();*/

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Roatate(_fvector vAxis, _float fTimeDelta)
{
	/* 현재 상태기준 정해준 각도만큼 회전시켜놓는다. */
	_vector		vRight = Get_State(STATE_RIGHT);
	_vector		vUp = Get_State(STATE_UP);
	_vector		vLook = Get_State(STATE_LOOK);

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, m_tTrans.fRotRadPerSec * fTimeDelta);

	vRight	= XMVector4Transform(vRight, RotationMatrix);
	vUp		= XMVector4Transform(vUp, RotationMatrix);
	vLook	= XMVector4Transform(vLook, RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::LookAt(_fvector vPoint)
{
	_float3		vScaled = Get_Scale();

	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = XMVector3Normalize(vPoint - vPosition) * vScaled.z;
	_vector		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)) * vScaled.x;
	_vector		vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * vScaled.y;

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Chase(_fvector vPoint, _float fTimeDelta, _float fMargin)
{
	_vector		vPosition = Get_State(STATE_POSITION);

	_vector		vDir = vPoint - vPosition;

	if (XMVectorGetX(XMVector3Length(vDir)) > fMargin)
		vPosition += XMVector3Normalize(vDir) * m_tTrans.fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
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
