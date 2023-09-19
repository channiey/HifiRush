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
	m_WorldMatrix = Matrix::Identity;

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

const Vec3 CTransform::Get_Scale()
{
	return _float3(m_WorldMatrix.Right().Length(), m_WorldMatrix.Up().Length(), m_WorldMatrix.Forward().Length());
}

const Vec3 CTransform::Get_Rotation()
{
	Quaternion quat{};

	Vec3 vScale = Get_Scale();
	Vec3 vPos = { m_WorldMatrix._41, m_WorldMatrix._42, m_WorldMatrix._43 };

	m_WorldMatrix.Decompose(vScale, quat, vPos);

	return ToEulerAngles(quat);
}

void CTransform::Set_State(STATE eState, Vec4 vState)
{
	_matrix		StateMatrix;

	StateMatrix = XMLoadFloat4x4(&m_WorldMatrix);

	StateMatrix.r[eState] = vState;

	XMStoreFloat4x4(&m_WorldMatrix, StateMatrix);
}

void CTransform::Set_Scale(const Vec3& vScale)
{
	Vec4		vRight = Get_State(STATE_RIGHT);
	Vec4		vUp = Get_State(STATE_UP);
	Vec4		vLook = Get_State(STATE_LOOK);

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	Set_State(STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

HRESULT CTransform::Bind_ShaderResources(CShader* pShader, const char* pConstantName)
{
	/* ���̴��� ���� ����� ���ε��Ѵ�. */

	return pShader->Bind_Matrix(pConstantName, &m_WorldMatrix);
}

const Vec3 CTransform::ToEulerAngles(Quaternion quat)
{
	Vec3 angles;

	// roll (x-axis rotation)
	double sinr_cosp = 2 * (quat.w * quat.x + quat.y * quat.z);
	double cosr_cosp = 1 - 2 * (quat.x * quat.x + quat.y * quat.y);
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = std::sqrt(1 + 2 * (quat.w * quat.y - quat.x * quat.z));
	double cosp = std::sqrt(1 - 2 * (quat.w * quat.y - quat.x * quat.z));
	angles.y = 2 * std::atan2(sinp, cosp) - 3.14159f / 2;

	// yaw (z-axis rotation)
	double siny_cosp = 2 * (quat.w * quat.z + quat.x * quat.y);
	double cosy_cosp = 1 - 2 * (quat.y * quat.y + quat.z * quat.z);
	angles.z = std::atan2(siny_cosp, cosy_cosp);

	return angles;
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

void CTransform::Set_Rotation(Vec4 vAxis, _float fRadian)
{
	/* �׵���� �������� ������ ������ŭ ȸ�����ѳ��´�. */
	Vec3		vScaled = Get_Scale();

	/* Right, Up, Look�� ȸ����Ų��. */
	Vec4		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScaled.x;
	Vec4		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScaled.y;
	Vec4		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScaled.z;

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

void CTransform::Roatate(Vec4 vAxis, _float fTimeDelta)
{
	/* ���� ���±��� ������ ������ŭ ȸ�����ѳ��´�. */
	Vec4		vRight = Get_State(STATE_RIGHT);
	Vec4		vUp = Get_State(STATE_UP);
	Vec4		vLook = Get_State(STATE_LOOK);

	Matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, m_tTrans.fRotRadPerSec * fTimeDelta);

	vRight	= XMVector4Transform(vRight, RotationMatrix);
	vUp		= XMVector4Transform(vUp, RotationMatrix);
	vLook	= XMVector4Transform(vLook, RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Rotate_World(Vec4 vAxis, _float fRadian)
{
	/* �׵���� �������� ������ ������ŭ ȸ�����ѳ��´�. */
	Vec3		vScaled = Get_Scale();

	/* Right, Up, Look�� ȸ����Ų��. */
	Vec4		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScaled.x;
	Vec4		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScaled.y;
	Vec4		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScaled.z;

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	vRight = XMVector4Transform(vRight, RotationMatrix);
	vUp = XMVector4Transform(vUp, RotationMatrix);
	vLook = XMVector4Transform(vLook, RotationMatrix);


	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Rotate_Local(Vec4 vAxis, _float fRadian)
{
	Vec4		vRight = Get_State(STATE_RIGHT);
	Vec4		vUp = Get_State(STATE_UP);
	Vec4		vLook = Get_State(STATE_LOOK);

	Matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	vRight = XMVector4Transform(vRight, RotationMatrix);
	vUp = XMVector4Transform(vUp, RotationMatrix);
	vLook = XMVector4Transform(vLook, RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::LookAt(Vec4 vPoint)
{
	Vec3		vScaled = Get_Scale();

	Vec4		vPosition = Get_State(STATE_POSITION);
	Vec4		vLook = XMVector3Normalize(vPoint - vPosition) * vScaled.z;
	Vec4		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)) * vScaled.x;
	Vec4		vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * vScaled.y;

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::LookAt_LandObj(Vec3 vPoint)
{
	Vec4		vLook = vPoint - Get_State(CTransform::STATE_POSITION);

	Vec3		vScale = Get_Scale();

	Vec4		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)) * vScale.x;

	vLook = XMVector3Normalize(XMVector3Cross(vRight, Get_State(CTransform::STATE_UP))) * vScale.z;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Chase(Vec3 vPoint, _float fTimeDelta, _float fMargin)
{
	Vec4		vPosition = Get_State(STATE_POSITION);

	Vec3		vDir = vPoint - vPosition;

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
