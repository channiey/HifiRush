#pragma once
#include "State_Gunner_Base.h"

BEGIN(Client)
class CState_Gunner_Attack final : public CState_Gunner_Base
{
	enum class ATTACK_TYPE 
	{
		GROUND, 
		AIR, 
		TYPEEND 
	};

private:
	CState_Gunner_Attack();
	CState_Gunner_Attack(const CState_Gunner_Attack& rhs);
	virtual ~CState_Gunner_Attack() = default;

public:
	virtual HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);

public:
	HRESULT			Enter() override;
	const wstring	Tick(const _double& fTimeDelta) override;
	const wstring	LateTick() override;
	void			Exit() override;

public:
	const wstring	Check_Transition() override;

public:
	const Ray		Get_Ray() const { return m_Ray; }
	const _bool		Is_Set_Ray() const { return m_bSetRay; }
	void			Render_Ray();

private:
	void			Detect_AttackCollision();
	void			Set_Ray();


private:

	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	ATTACK_TYPE		m_eAttackType = ATTACK_TYPE::TYPEEND;

	Ray				m_Ray;
	_bool			m_bSetRay;
	_bool			m_bPlaySound = FALSE;


	PrimitiveBatch<VertexPositionColor>*	m_pBatch = nullptr;
	BasicEffect*							m_pEffect = nullptr;
	ID3D11InputLayout*						m_pInputLayout = nullptr;
	_float4									m_vColor = _float4(1.f, 0.f, 0.f, 1.f);

public:
	static CState_Gunner_Attack* Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner);
	virtual CState* Clone(void* pArg) override;
	virtual void Free() override;
};
END
