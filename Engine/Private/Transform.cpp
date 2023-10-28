#include "..\Public\Transform.h"
#include "GameObject.h"
#include "Shader.h"
#include "NavMeshAgent.h"

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
	XMMATRIX   rotationMatrix = m_RotMatrix;
	float pitch, yaw, roll;

	// Extract sin and cos values to avoid Gimbal lock
	if (rotationMatrix.r[2].m128_f32[1] < 1.0f)
	{
		if (rotationMatrix.r[2].m128_f32[1] > -1.0f)
		{
			yaw = atan2(rotationMatrix.r[0].m128_f32[2], rotationMatrix.r[2].m128_f32[2]);
			pitch = asin(-rotationMatrix.r[1].m128_f32[2]);
			roll = atan2(rotationMatrix.r[1].m128_f32[0], rotationMatrix.r[1].m128_f32[1]);
		}
		else
		{
			// Gimbal lock: pitch = -90 degrees
			yaw = -atan2(-rotationMatrix.r[0].m128_f32[0], rotationMatrix.r[0].m128_f32[2]);
			pitch = -DirectX::XM_PI / 2;
			roll = 0.0f;
		}
	}
	else
	{
		// Gimbal lock: pitch = 90 degrees
		yaw = atan2(-rotationMatrix.r[0].m128_f32[0], rotationMatrix.r[0].m128_f32[2]);
		pitch = DirectX::XM_PI / 2;
		roll = 0.0f;
	}

	// Convert radians to degrees
	pitch = DirectX::XMConvertToDegrees(pitch);
	yaw = DirectX::XMConvertToDegrees(yaw);
	roll = DirectX::XMConvertToDegrees(roll);

	// Clamp the values to 0-360 range
	pitch = fmodf(pitch, 360.0f);
	yaw = fmodf(yaw, 360.0f);
	roll = fmodf(roll, 360.0f);

	return Vec3(pitch, yaw, roll);

	////////////////////////////////////////////////
	Vec3 vTemp;
	Quaternion quatTemp;

	m_WorldMatrix.Decompose(vTemp, quatTemp, vTemp);

	return ToEulerAngles(quatTemp);
}

const Vec4 CTransform::Get_FinalPosition()
{
	const Matrix matFinal = Get_FinalMat();

	return Vec4(matFinal.m[3]);
}

void CTransform::Set_Position(Vec3 vPos, _bool bNotAgent)
{ 
	/* Check Not Apply Agent Filtering (for RootAnimation) */
	if (bNotAgent)
	{
		memcpy(m_WorldMatrix.m[STATE_POSITION], &vPos, sizeof(Vec3));
		return;
	}

	/* Check NavMeshAgent */
	if (nullptr != m_pNavMeshAgentCom && m_pNavMeshAgentCom->Is_Active())
	{
		Vec3 vTemp = vPos + m_vRootPos;
		if (!m_pNavMeshAgentCom->Can_Move(vTemp))
			return;
	}

	/* Apply */
	memcpy(m_WorldMatrix.m[STATE_POSITION], &vPos, sizeof(Vec3)); 

	/* Agent Height */
	if (nullptr != m_pNavMeshAgentCom && m_pNavMeshAgentCom->Is_Active())
	{
		if(!m_pNavMeshAgentCom->Is_AirState())
			m_WorldMatrix.m[STATE_POSITION][1] = m_pNavMeshAgentCom->Get_Height(Get_Position().xyz());
	}
}

void CTransform::Set_Look(const Vec4& vLookDir)
{
	Vec4		vDir = vLookDir;

	Vec3		vScale = Get_Scale();

	Vec4		vLook = vDir.Normalized() * vScale.z;
	Vec4		vRight = XMVector3Normalize(XMVector3Cross(Vec4::UnitY, vLook)) * vScale.x;
	Vec4		vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * vScale.y;

	Set_State(CTransform::STATE_RIGHT, vRight);
	Set_State(CTransform::STATE_UP, vUp);
	Set_State(CTransform::STATE_LOOK, vLook);
}

void CTransform::Set_State(STATE eState, Vec4 vState)
{
	_matrix		StateMatrix;

	StateMatrix = XMLoadFloat4x4(&m_WorldMatrix);

	StateMatrix.r[eState] = vState;

	XMStoreFloat4x4(&m_WorldMatrix, StateMatrix);
}

void CTransform::Set_RootPos(const Vec4& vPos, _bool bNotAgent)
{
	Vec4 vDir = Get_State(STATE_LOOK);
	Vec4 vRootPos = vPos;

	/* Check NavMeshAgent */
	if (nullptr != m_pNavMeshAgentCom && m_pNavMeshAgentCom->Is_Active())
	{
		Vec3 vTemp = (vDir.Normalized() * vRootPos.ZeroW().Length()) + Vec4(m_WorldMatrix.m[3]).xyz();
		if (!m_pNavMeshAgentCom->Can_Move(vTemp))
			return;
	}

	/* Apply */
	m_vRootPos = vDir.Normalized() * vRootPos.ZeroW().Length();
}

void CTransform::Set_Scale(const Vec3& vScale)
{
	Set_State(CTransform::STATE_RIGHT,
		XMVector3Normalize(Get_State(CTransform::STATE_RIGHT)) * XMVectorGetX(vScale));
	Set_State(CTransform::STATE_UP,
		XMVector3Normalize(Get_State(CTransform::STATE_UP)) * XMVectorGetY(vScale));
	Set_State(CTransform::STATE_LOOK,
		XMVector3Normalize(Get_State(CTransform::STATE_LOOK)) * XMVectorGetZ(vScale));

	//for (_int i = 0; i < 3; ++i)
	//{
	//	Vec3 v(m_WorldMatrix.m[i][0], m_WorldMatrix.m[i][1], m_WorldMatrix.m[i][2]);
	//	v.Normalize();

	//	for (_int j = 0; j < 3; ++j)
	//		m_WorldMatrix.m[i][j] = *(((_float*)&v) + j) * *(((_float*)&vScale) + j);
	//}
}

void CTransform::Set_Rotation(const Vec3& vEulers, const _bool& bWorld)
{
	// 각 회전 축에 대한 쿼터니언을 초기화합니다.
	Quaternion quatX = Quaternion::Identity;
	Quaternion quatY = Quaternion::Identity;
	Quaternion quatZ = Quaternion::Identity;

	// 오일러 각도를 90도 범위 내로 조정합니다.
	Vec3 clampedEulers = vEulers;
	clampedEulers.x = fmodf(clampedEulers.x, 360.0f);
	clampedEulers.y = fmodf(clampedEulers.y, 360.0f);
	clampedEulers.z = fmodf(clampedEulers.z, 360.0f);

	// 쿼터니언을 순서대로 결합합니다.
	Quaternion finalQuat = Quaternion::Identity;

	// X 축 회전을 추가합니다.
	if (clampedEulers.x != 0.0f)
	{
		Vec3 xAxis = bWorld ? Vec3::Right : static_cast<Vec3>(m_WorldMatrix.m[0]);
		quatX = Quaternion::CreateFromAxisAngle(xAxis, XMConvertToRadians(clampedEulers.x));
		finalQuat *= quatX;
	}

	// Y 축 회전을 추가합니다.
	if (clampedEulers.y != 0.0f)
	{
		Vec3 yAxis = bWorld ? Vec3::Up : static_cast<Vec3>(m_WorldMatrix.m[1]);
		quatY = Quaternion::CreateFromAxisAngle(yAxis, XMConvertToRadians(clampedEulers.y));
		finalQuat *= quatY;
	}

	// Z 축 회전을 추가합니다.
	if (clampedEulers.z != 0.0f)
	{
		Vec3 zAxis = bWorld ? Vec3::Forward : static_cast<Vec3>(m_WorldMatrix.m[2]);
		quatZ = Quaternion::CreateFromAxisAngle(zAxis, XMConvertToRadians(clampedEulers.z));
		finalQuat *= quatZ;
	}

	// 현재의 스케일과 위치 정보를 추출합니다.
	Vec3 scale = Get_Scale();
	Vec3 position = Get_Position().xyz();

	// 쿼터니언을 회전 행렬로 변환합니다.
	Matrix matRotation = Matrix::CreateFromQuaternion(finalQuat);
	m_RotMatrix = matRotation;

	// 스케일 및 위치 정보를 적용한 최종 변환 행렬을 생성합니다.
	matRotation.m[3][0] = position.x;
	matRotation.m[3][1] = position.y;
	matRotation.m[3][2] = position.z;
	matRotation.m[0][0] *= scale.x;
	matRotation.m[1][1] *= scale.y;
	matRotation.m[2][2] *= scale.z;

	// 월드 행렬을 최종 변환 행렬로 설정합니다.
	m_WorldMatrix = matRotation;
}

void CTransform::Set_Rotation(const Vec3& vAxis, const _float fRad)
{
	_float3		vScaled = Get_Scale();

	/* 항등상태 기준으로 정해준 각도만큼 회전시켜놓는다. */
	/* Right, Up, Look를 회전시킨다. */
	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScaled.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScaled.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScaled.z;

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, XMConvertToRadians(fRad));
	m_RotMatrix = RotationMatrix;

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
		quat = Quaternion::CreateFromAxisAngle(vAxis, XMConvertToRadians(vEulers.y)); /* 매개변수 : 회전 중심으로 사용할 벡터, 회전 각도(라디안) */
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
	m_RotMatrix = matRotation;

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
	/* 현재 상태기준 정해준 각도만큼 회전시켜놓는다. */
	_vector		vRight = Get_State(STATE_RIGHT);
	_vector		vUp = Get_State(STATE_UP);
	_vector		vLook = Get_State(STATE_LOOK);

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRad);
	m_RotMatrix = RotationMatrix;

	vRight = XMVector4Transform(vRight, RotationMatrix);
	vUp = XMVector4Transform(vUp, RotationMatrix);
	vLook = XMVector4Transform(vLook, RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Translate(const Vec3& vTranslation, _bool bNotAgent)
{
	/* Check NavMeshAgent */
	if (nullptr != m_pNavMeshAgentCom && m_pNavMeshAgentCom->Is_Active())
	{
		Vec3 vTemp = vTranslation + Vec4(m_WorldMatrix.m[3]).xyz();
		if (!m_pNavMeshAgentCom->Can_Move(vTemp))
			return;
	}

	/* Apply */
	for (_uint i = 0; i < 3; ++i)
		*((_float*)(&m_WorldMatrix.m[3]) + i) += *((_float*)&vTranslation + i);

	/* Agent Height */
	if (nullptr != m_pNavMeshAgentCom && m_pNavMeshAgentCom->Is_Active())
	{
		if (!m_pNavMeshAgentCom->Is_AirState())
			m_WorldMatrix.m[STATE_POSITION][1] = m_pNavMeshAgentCom->Get_Height(Get_Position().xyz());

	}
}

HRESULT CTransform::Bind_ShaderResources(CShader* pShader, const char* pConstantName)
{
	Matrix matFinal = Get_FinalMat();

	return pShader->Bind_Matrix(pConstantName, &matFinal);
}

const Vec3 CTransform::ToEulerAngles(Quaternion quat)
{
	Vec3 angles;

	// roll (x-axis rotation)
	_float sinr_cosp = 2.f * (quat.w * quat.x + quat.y * quat.z);
	_float cosr_cosp = 1.f - 2.f * (quat.x * quat.x + quat.y * quat.y);
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	_float sinp = std::sqrt(1.f + 2.f * (quat.w * quat.y - quat.x * quat.z));
	_float cosp = std::sqrt(1.f - 2.f * (quat.w * quat.y - quat.x * quat.z));
	angles.y = 2.f * std::atan2(sinp, cosp) - 3.14159265f / 2.f;

	// yaw (z-axis rotation)
	_float siny_cosp = 2.f * (quat.w * quat.z + quat.x * quat.y);
	_float cosy_cosp = 1.f - 2.f * (quat.y * quat.y + quat.z * quat.z);
	angles.z = std::atan2(siny_cosp, cosy_cosp);

	//return Vec3(RAD2DEG(angles.x), RAD2DEG(angles.y), RAD2DEG(angles.z));
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

const Matrix CTransform::Get_WorldMat()
{
	if (m_pOwner->Is_Parent())
		return m_pOwner->Get_Parent()->Get_Transform()->Get_WorldMat() * m_WorldMatrix;
	
	return m_WorldMatrix;
}

const Matrix CTransform::Get_FinalMat()
{
	Vec3	vfinalPos = m_vRootPos.xyz() + Vec4(m_WorldMatrix.m[3]).xyz();

	Matrix	matFinal = m_WorldMatrix;

	memcpy(&matFinal.m[3], &vfinalPos, sizeof(Vec3));

	if (m_pOwner->Is_Parent())
		return m_pOwner->Get_Parent()->Get_Transform()->Get_FinalMat() * matFinal;

	return matFinal;
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

	Safe_Release(m_pNavMeshAgentCom);
}
