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
	return S_OK;
}

const Vec3 CTransform::Get_Scale()
{
	Vec3 vScale, vTemp;
	Quaternion quatTemp;

	m_WorldMatrix.Decompose(vScale, quatTemp, vTemp);

	return vScale;
}

const Vec3 CTransform::Get_Rotation()
{
	Vec3 vTemp;
	Quaternion quatTemp;

	m_WorldMatrix.Decompose(vTemp, quatTemp, vTemp);

	return ToEulerAngles(quatTemp);
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
	for (_int i = 0; i < 3; ++i)
	{
		Vec3 v(m_WorldMatrix.m[i][0], m_WorldMatrix.m[i][1], m_WorldMatrix.m[i][2]);
		v.Normalize();

		for (_int j = 0; j < 3; ++j)
			m_WorldMatrix.m[i][j] = *(((_float*)&v) + j) * *(((_float*)&vScale) + j);
	}

	//Vec4		vRight = Get_Right();
	//Vec4		vUp = Get_Up();
	//Vec4		vLook = Get_Forward();

	//Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
	//Set_State(STATE_UP, XMVector3Normalize(vUp) * vScale.y);
	//Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
}

void CTransform::Rotate(Vec3& vEulers)
{
	Matrix matRotation = Matrix::Identity;
	Quaternion quat = Quaternion::Identity;

	/* CreateFromAxisAngle(Vector3 axis, float angle) */
	/* axis = ȸ�� �߽����� ����� ���� ���� */
	/* angle = �� ���͸� �߽����� ȸ���� ����(����) */

	if (0.f != vEulers.y)
	{
		/* �� y���� �������� ���Ϸ� y��ŭ ȸ���� ���ʹϾ��� ���Ѵ�. */
		Vec3 v(m_WorldMatrix.m[1]);
		quat = Quaternion::CreateFromAxisAngle(v, XMConvertToRadians(vEulers.y));
	}
	if (0.f != vEulers.x)
	{
		/* �� x���� �������� ���Ϸ� x��ŭ ȸ���� ���ʹϾ��� ���� ���� ���ʹϾ� ���Ѵ�. */

		Vec3 v(m_WorldMatrix.m[0]);
		quat *= Quaternion::CreateFromAxisAngle(v, XMConvertToRadians(vEulers.x));
	}
	if (0.f != vEulers.z)
	{
		Vec3 v(m_WorldMatrix.m[2]);
		quat *= Quaternion::CreateFromAxisAngle(v, XMConvertToRadians(vEulers.z));
	}

	/* ȸ���� �ݿ��� ���ʹϾ����κ��� ȸ������� �����. */
	matRotation = Matrix::CreateFromQuaternion(quat);

	for (_uint i = 0; i < 3; ++i)
	{
		/* ȸ�� ������κ��� �̹� ���� ���⺤�͸� ���Ѵ�. */
		Vec3 v(m_WorldMatrix.m[i]);
		v = Vec3::TransformNormal(v, matRotation);

		/* �� ���⺤�͸� ���� ��Ŀ� �����Ѵ�. */
		for (_uint j = 0; j < 3; ++j)
			m_WorldMatrix.m[i][j] = *((_float*)&v + j);
	}
}

void CTransform::Translate(const Vec3 vTranslation)
{
	for (_uint i = 0; i < 3; ++i)
		*((_float*)(&m_WorldMatrix.m[3]) + i) += *((_float*)&vTranslation + i);
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
