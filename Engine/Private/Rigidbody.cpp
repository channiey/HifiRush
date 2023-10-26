#include "..\Public\Rigidbody.h"

#include "GameInstance.h"
#include "GameObject.h"

CRigidbody::CRigidbody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CRigidbody::CRigidbody(const CRigidbody& rhs)
	: CComponent(rhs)
	, m_eType(rhs.m_eType)
	, m_bSleep(rhs.m_bSleep)
	, m_bUseGravity(rhs.m_bUseGravity)
	, m_bKinematic(rhs.m_bKinematic)
	, m_fFriction(rhs.m_fFriction)
	, m_fMass (rhs.m_fMass)
	, m_fGravitionalConstant(rhs.m_fGravitionalConstant)
	, m_byConstraints(rhs.m_byConstraints)
	, m_vLinearAcceleration(rhs.m_vLinearAcceleration)
	, m_vLinearVelocity(rhs.m_vLinearVelocity)
	, m_fSleepThreshold(rhs.m_fSleepThreshold)
{

}

HRESULT CRigidbody::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRigidbody::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memmove(&m_eType, pArg, sizeof(RIGIDBODY_TYPE));

	return S_OK;
}

void CRigidbody::Tick(const _float& fTimeDelta)
{
	if (Check_Sleep() || m_eType == RIGIDBODY_TYPE::STATIC)
		return;

	m_bKinematic ? Update_Kinematic(fTimeDelta) : Update_Kinetic(fTimeDelta);
}

void CRigidbody::Add_Force(const Vec3& vForce, const FORCE_MODE& eMode)
{
	switch (eMode)
	{
	case FORCE_MODE::FORCE:
		m_vLinearAcceleration += vForce / m_fMass;
		break;
	case FORCE_MODE::IMPULSE:
		m_vLinearVelocity += vForce / m_fMass;
		break;
	case FORCE_MODE::ACCELERATION:
		m_vLinearAcceleration += vForce;
		break;
	case FORCE_MODE::VELOCITY_CHANGE:
		m_vLinearVelocity += vForce;
		break;
	default:
		break;
	}

	Wake();
}

void CRigidbody::Clear_Force(const FORCE_MODE& eMode)
{
	switch (eMode)
	{
	case FORCE_MODE::FORCE:
		::ZeroMemory(&m_vLinearAcceleration, sizeof(Vec3));
		break;
	case FORCE_MODE::IMPULSE:
		::ZeroMemory(&m_vLinearVelocity, sizeof(Vec3));
		break;
	case FORCE_MODE::ACCELERATION:
		::ZeroMemory(&m_vLinearAcceleration, sizeof(Vec3));
		break;
	case FORCE_MODE::VELOCITY_CHANGE:
		::ZeroMemory(&m_vLinearVelocity, sizeof(Vec3));
		break;
	default:
		break;
	}
}

void CRigidbody::Clear_NetPower()
{
	ZeroMemory(&m_vLinearAcceleration, sizeof(Vec3));
	ZeroMemory(&m_vLinearVelocity, sizeof(Vec3));
}

void CRigidbody::Update_Kinetic(const _float& fTimeDelta)
{
	if (m_bUseGravity)
		m_vLinearVelocity.y += m_fGravitionalConstant * fTimeDelta;

	m_vLinearVelocity += m_vLinearAcceleration * fTimeDelta;

	const _float fLinearResistance = m_fFriction; 

	m_vLinearVelocity =  (fLinearResistance < 1.f) ? (m_vLinearVelocity * (1.f - fLinearResistance)) : (m_vLinearVelocity = Vec3::Zero);

	if (m_byConstraints)
	{
		_int i = 0;
		while (i < 3)
			(m_byConstraints & 1 << i) ? (*(((_float*)&m_vLinearVelocity) + i++) = 0) : i++;
	}

	Update_Transform(fTimeDelta);
}

void CRigidbody::Update_Kinematic(const _float& fTimeDelta)
{
	Clear_Force(FORCE_MODE::VELOCITY_CHANGE);
}

void CRigidbody::Update_Transform(const _float& fTimeDelta)
{
	m_pOwner->Get_Transform()->Translate(m_vLinearVelocity * fTimeDelta);
}

const _bool CRigidbody::Check_Sleep()
{
	if (m_bSleep)
		return TRUE;

	if (!m_bUseGravity && m_fSleepThreshold > m_vLinearVelocity.Length())
	{
		Sleep();
		return TRUE;
	}

	return FALSE;
}

CRigidbody* CRigidbody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRigidbody* pInstance = new CRigidbody(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRigidbody");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CRigidbody::Clone(void* pArg)
{
	CRigidbody* pInstance = new CRigidbody(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CRigidbody");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRigidbody::Free()
{
	__super::Free();
}
