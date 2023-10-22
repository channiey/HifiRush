#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRigidbody final : public CComponent
{
public:
	enum AXIS_TYPE				{ AXIS_X, AXIS_Y, AXIS_Z, AXIS_END };
	enum class RIGIDBODY_TYPE	{ STATIC, DYNAMIC, TYPEEND };
	enum class FORCE_MODE		{ FORCE, IMPULSE, ACCELERATION, VELOCITY_CHANGE, TYPEEND };
	/*
		FORCE : �ٶ��̳�, �ڱ�°� ���� ���������� �־����� ��

		IMPULSE : Ÿ���̳� ���߰� ���� �������� �� (��ݷ�)

		ACCELERATION : ���� ����, ���� ���ӷ� �ο� 
						�� �� ���� ������ ���� �������� �޶�������, �� ���� ������ ������� ������ ������ ����� ����.

		VELOCITY_CHANGE : ���� ���� ���� �ӵ� ��ȭ �ο�
							ACCELERATION�� �ð��� �귯���鼭 ��ȭ�� ����Ű�µ� ���� �� ���� ���������� ������ �ӵ��� ��ȭ�� ����Ų��. 

	*/

private:
	CRigidbody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRigidbody(const CRigidbody& rhs);
	virtual ~CRigidbody() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg) override;
	void					Tick(const _float& fTimeDelta);
	//void					LateTick(const _float& fTimeDelta);

public:
	const RIGIDBODY_TYPE&	Get_Type() const { return m_eType; }
	const _float&			Get_Mass() const { return m_fMass; }
	const _float&			Get_Friction() const { return m_fFriction; }

	Vec3					Get_LinearVelocity() const { return m_vLinearVelocity; }
	_float					Get_LinearAxisVelocity(AXIS_TYPE eAxis) const { return *((_float*)&m_vLinearVelocity + (_int)eAxis); }

public:
	void					Set_Kinematic(const _bool& bKinematic) { m_bKinematic = bKinematic; }
	void					Set_UseGravity(const _bool& bUse) { m_bUseGravity = bUse; }
	void					Set_Mass(const _float& fMass) { m_fMass = fMass;}
	void					Set_Friction(const _float& fFriction) { m_fFriction = fFriction; }

	void					Set_LinearVelocity(const Vec3& vLinearVelocity) { m_vLinearVelocity = vLinearVelocity; }
	void					Set_LinearAxisVelocity(AXIS_TYPE eAxis, _float fVelocity) { *((_float*)&m_vLinearVelocity + (_int)eAxis) = fVelocity; }
	void					Set_FreezePosition(AXIS_TYPE eAxis) { m_byConstraints ^= 1 << (_int)eAxis; }

public:
	const _bool&			Is_UseGravity() const { return m_bUseGravity; }
	const _bool&			Is_Kinematic() const { return m_bKinematic; }
	_bool					Is_FrozePosition(AXIS_TYPE eAxis) { return m_byConstraints & 1 << (_int)eAxis; }

public:
	void					Add_Force(const Vec3& vForce, const FORCE_MODE& eMode);
	void					Clear_Force(const FORCE_MODE& eMode);
	void					Clear_NetPower();

private:
	void					Update_Kinetic(const _float& fTimeDelta);
	void					Update_Kinematic(const _float& fTimeDelta); 
	void					Update_Transform(const _float& fTimeDelta);

private:
	const _bool				Check_Sleep();
	void					Sleep() { Clear_NetPower(); m_bSleep = TRUE; }
	void					Wake()	{ m_bSleep = FALSE; }

private:
	RIGIDBODY_TYPE			m_eType			= RIGIDBODY_TYPE::TYPEEND;
	_bool					m_bSleep		= FALSE;

	_bool					m_bUseGravity	= FALSE;
	_bool					m_bKinematic	= FALSE; 
	_float					m_fFriction		= 0.1f;
	_float					m_fMass			= 10.f;
	_float					m_fGravitionalConstant = -9.8f;

	_byte					m_byConstraints = 0;

	Vec3					m_vLinearAcceleration;
	Vec3					m_vLinearVelocity;

	_float					m_fSleepThreshold = 0.05f;

public:
	static CRigidbody* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};
END