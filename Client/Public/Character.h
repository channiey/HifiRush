#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CCollider;
class CRenderer;
class CTransform;
class CNavMeshAgent;
END

BEGIN(Client)

class CCharacter abstract  : public CGameObject
{
public:

	typedef struct tagFightDesc
	{
		_bool   bAttack = FALSE;
		_bool   bCombo = FALSE;
		_bool	bDamaged = FALSE;

		_bool   bInvulneralbe = FALSE;

		_bool	bParry = FALSE;
		_int	iCombo = -1;
		_int	iStep = -1;

		CCharacter* pTarget = nullptr;
		CCharacter* pAttacker = nullptr;

	}FIGHT_DESC;

	typedef struct tagPhysicsDesc
	{
		_float  fMaxForwardSpeed = 10.f;
		_float  fMinForwardSpeed = 10.f;

		_float  fMinTurnSpeed = 10.f;
		_float  fMaxTurnSpeed = 20.f;

		_float  fJumpPower = 550.f;

		_float	fJumpGravity = -15.f;
		_float  fFallGravity = -160.f;

		_bool   bGround = TRUE;
		_bool   bJump = FALSE;
		_bool   bDoubleJump = FALSE;
		_bool   bFall = FALSE;
		_bool	bLanding = FALSE;

		_bool   bDash = FALSE;
		_float	fDashPower = 300.f;

		_float  fGroundedRayDistance = 5.f;


		_float	fKnockBackPower = 35.f;


	}PHYSICS_DESC;

	typedef struct tagStatDesc
	{
		_float fMaxHp = 100.f;
		_float fCurHp = 100.f;

		_float fAd = 10.f;

		_int iGold = 100;
		
		_bool bDead = FALSE;

	}STAT_DESC;

protected:
	CCharacter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCharacter(const CCharacter& rhs);
	virtual ~CCharacter() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_double fTimeDelta);
	virtual void			LateTick(_double fTimeDelta);
	virtual HRESULT			Render();

public:
	const STAT_DESC&		Get_StatDesc() const { return m_tStatDesc; }
	const FIGHT_DESC&		Get_FightDesc() const { return m_tFightDesc; }
	const PHYSICS_DESC&		Get_PhysicsDesc() const { return m_tPhysicsDesc; }
	

protected:
	HRESULT					Ready_Components();
	virtual HRESULT			Ready_Chilren() PURE;
	virtual HRESULT			Ready_StateMachine() PURE;

protected:
	HRESULT					Bind_ShaderResources();

protected:
	HRESULT					Update_RootMotion();

public:
	void					Attack(CCharacter* pCharacter);
	void					Damaged(CCharacter* pCharacter);
	void					Die();

	void					KnockBack(CCharacter* pAttacker);
	void					Reset_Desc();

protected:
	CShader*				m_pShaderCom		= { nullptr };
	CRenderer*				m_pRendererCom		= { nullptr };
	CTransform*				m_pTransformCom		= { nullptr };
	CModel*					m_pModelCom			= { nullptr };
	CNavMeshAgent*			m_pNavMeshAgentCom	= { nullptr };
	CStateMachine*			m_pStateMachineCom	= { nullptr };
	CRigidbody*				m_pRigidbodyCom		= { nullptr };

	vector<CCollider*>		m_pColliderComs;

protected:
	STAT_DESC				m_tStatDesc;
	FIGHT_DESC				m_tFightDesc;
	PHYSICS_DESC			m_tPhysicsDesc;

public:
	virtual CCharacter*		Clone(void* pArg)= 0;
	virtual void			Free() override;
};

END