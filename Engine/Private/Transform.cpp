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
}

void CTransform::Set_Rotation(const Vec3& vEulers, const _bool& bWorld)
{
	// �� ȸ�� �࿡ ���� ���ʹϾ��� �ʱ�ȭ�մϴ�.
	Quaternion quatX = Quaternion::Identity;
	Quaternion quatY = Quaternion::Identity;
	Quaternion quatZ = Quaternion::Identity;

	// ���Ϸ� ������ 90�� ���� ���� �����մϴ�.
	Vec3 clampedEulers = vEulers;
	clampedEulers.x = fmodf(clampedEulers.x, 360.0f);
	clampedEulers.y = fmodf(clampedEulers.y, 360.0f);
	clampedEulers.z = fmodf(clampedEulers.z, 360.0f);

	// ���ʹϾ��� ������� �����մϴ�.
	Quaternion finalQuat = Quaternion::Identity;

	// X �� ȸ���� �߰��մϴ�.
	if (clampedEulers.x != 0.0f)
	{
		Vec3 xAxis = bWorld ? Vec3::Right : static_cast<Vec3>(m_WorldMatrix.m[0]);
		quatX = Quaternion::CreateFromAxisAngle(xAxis, XMConvertToRadians(clampedEulers.x));
		finalQuat *= quatX;
	}

	// Y �� ȸ���� �߰��մϴ�.
	if (clampedEulers.y != 0.0f)
	{
		Vec3 yAxis = bWorld ? Vec3::Up : static_cast<Vec3>(m_WorldMatrix.m[1]);
		quatY = Quaternion::CreateFromAxisAngle(yAxis, XMConvertToRadians(clampedEulers.y));
		finalQuat *= quatY;
	}

	// Z �� ȸ���� �߰��մϴ�.
	if (clampedEulers.z != 0.0f)
	{
		Vec3 zAxis = bWorld ? Vec3::Forward : static_cast<Vec3>(m_WorldMatrix.m[2]);
		quatZ = Quaternion::CreateFromAxisAngle(zAxis, XMConvertToRadians(clampedEulers.z));
		finalQuat *= quatZ;
	}

	// ������ �����ϰ� ��ġ ������ �����մϴ�.
	Vec3 scale = Get_Scale();
	Vec3 position = Get_Position().ToVec3();

	// ���ʹϾ��� ȸ�� ��ķ� ��ȯ�մϴ�.
	Matrix matRotation = Matrix::CreateFromQuaternion(finalQuat);

	// ������ �� ��ġ ������ ������ ���� ��ȯ ����� �����մϴ�.
	matRotation.m[3][0] = position.x;
	matRotation.m[3][1] = position.y;
	matRotation.m[3][2] = position.z;
	matRotation.m[0][0] *= scale.x;
	matRotation.m[1][1] *= scale.y;
	matRotation.m[2][2] *= scale.z;

	// ���� ����� ���� ��ȯ ��ķ� �����մϴ�.
	m_WorldMatrix = matRotation;
}

void CTransform::Set_Rotation(const Vec3& vAxis, const _float fRad)
{
	_float3		vScaled = Get_Scale();

	/* �׵���� �������� ������ ������ŭ ȸ�����ѳ��´�. */
	/* Right, Up, Look�� ȸ����Ų��. */
	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScaled.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScaled.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScaled.z;

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, XMConvertToRadians(fRad));

	vRight = XMVector4Transform(vRight, RotationMatrix);
	vUp = XMVector4Transform(vUp, RotationMatrix);
	vLook = XMVector4Transform(vLook, RotationMatrix);
	/*XMVector3TransformNormal();
	XMVector3TransformCoord();*/

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Rotate(const Vec3& vEulers, const _bool& bWorld)
{
	Matrix matRotation = Matrix::Identity;
	Quaternion quat = Quaternion::Identity;
	Vec3 vAxis;

	if (0.f != vEulers.y)
	{
		Vec3 vAxis = bWorld ? Vec3::Up : static_cast<Vec3>(m_WorldMatrix.m[1]);
		quat = Quaternion::CreateFromAxisAngle(vAxis, XMConvertToRadians(vEulers.y)); /* �Ű����� : ȸ�� �߽����� ����� ����, ȸ�� ����(����) */
	}
	if (0.f != vEulers.x)
	{
		Vec3 vAxis = bWorld ? Vec3::Right : static_cast<Vec3>(m_WorldMatrix.m[0]);
		quat *= Quaternion::CreateFromAxisAngle(vAxis, XMConvertToRadians(vEulers.x));
	}
	if (0.f != vEulers.z)
	{
		Vec3 vAxis = bWorld ? Vec3::Forward : static_cast<Vec3>(m_WorldMatrix.m[2]);
		quat *= Quaternion::CreateFromAxisAngle(vAxis, XMConvertToRadians(vEulers.z));
	}

	matRotation = Matrix::CreateFromQuaternion(quat);

	for (_uint i = 0; i < 3; ++i)
	{
		Vec3 v(m_WorldMatrix.m[i]);
		v = Vec3::TransformNormal(v, matRotation);

		for (_uint j = 0; j < 3; ++j)
			m_WorldMatrix.m[i][j] = *((_float*)&v + j);
	}
}

void CTransform::Rotate(const Vec4& vAxis, const _float& fRad)
{
	/* ���� ���±��� ������ ������ŭ ȸ�����ѳ��´�. */
	_vector		vRight = Get_State(STATE_RIGHT);
	_vector		vUp = Get_State(STATE_UP);
	_vector		vLook = Get_State(STATE_LOOK);

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRad);

	vRight = XMVector4Transform(vRight, RotationMatrix);
	vUp = XMVector4Transform(vUp, RotationMatrix);
	vLook = XMVector4Transform(vLook, RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Translate(const Vec3& vTranslation)
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
	if (-0.f == angles.x) angles.x = 0.f;
	// pitch (y-axis rotation)

	/**/
	/*double sinp = std::sqrt(1 + 2 * (quat.w * quat.y - quat.x * quat.z));
	double cosp = std::sqrt(1 - 2 * (quat.w * quat.y - quat.x * quat.z));
	angles.y = 2 * std::atan2(sinp, cosp) - XM_PI / 2;*/

	/* GPT ��� ���� */
	/*
		atan2 �Լ��� ��� ������ - �𿡼� �� �Ǵ� - 180���� 180���Դϴ�.
		�׷��� �� �ڵ忡���� sinp�� cosp ���� ����Ͽ� atan2 �Լ��κ��� ���� ���� 2���ϰ� XM_PI / 2�� �� ���� angles.y�� �����Ϸ��� �մϴ�.
		�̷��� �ϸ� ��ȿ�� ���Ϸ� ������ �ƴ� �� ������ ���ϴ� ����� ���� �� ���� �� �ֽ��ϴ�.
	*/
	
	angles.y = std::atan2(2 * (quat.w * quat.y + quat.x * quat.z), 1 - 2 * (quat.y * quat.y + quat.z * quat.z));
	if (-0.f == angles.y) angles.y = 0.f;

	// yaw (z-axis rotation)
	double siny_cosp = 2 * (quat.w * quat.z + quat.x * quat.y);
	double cosy_cosp = 1 - 2 * (quat.y * quat.y + quat.z * quat.z);
	angles.z = std::atan2(siny_cosp, cosy_cosp);
	if (-0.f == angles.z) angles.z = 0.f;

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
