#pragma once

#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CModel;
class CShader;
class CCollider;
class CRenderer;
class CTransform;
class CNavigation;
END

BEGIN(Client)

class CCharacter abstract  : public CGameObject
{
public:

	typedef struct tagFightDesc
	{
		//_bool   bCanAttack = TRUE;
		_bool   bInAttack = FALSE;
		_bool   bInCombo = FALSE;
		_bool	bDamaged = FALSE;

		_bool   bInvulneralbe = FALSE;

	}FIGHT_DESC;

	typedef struct tagMovementDesc
	{
		_float  fMaxForwardSpeed = 10.f;
		_float  fMinForwardSpeed = 10.f;

		_float  fMinTurnSpeed = 10.f;
		_float  fMaxTurnSpeed = 10.f;

		_float  fGravity = 0.f;

		_bool   bGround = TRUE;
		_bool   bJump = FALSE;
		_bool   bDoubleJump = FALSE;
		_bool   bFall = FALSE;

		_float  fGroundedRayDistance = 5.f;

		_float  fJumpSpeed = 10.f;

	}MOVEMENT_DESC;


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
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_float fTimeDelta);
	virtual void		LateTick(_float fTimeDelta);
	virtual HRESULT		Render();

public:
	const STAT_DESC&		Get_StatDesc() const { return m_tStatDesc; }
	const FIGHT_DESC&		Get_FightDesc() const { return m_tFightDesc; }
	const MOVEMENT_DESC&	Get_MovementDesc() const { return m_tMoveDesc; }

protected:
	HRESULT				Ready_Components();
	HRESULT				Ready_ChildObjects();

protected:
	HRESULT				Bind_ShaderResources();

private:
	HRESULT				Update_RootMotion();

protected:
	CShader*			m_pShaderCom		= { nullptr };
	CRenderer*			m_pRendererCom		= { nullptr };
	CTransform*			m_pTransformCom		= { nullptr };
	CModel*				m_pModelCom			= { nullptr };
	CNavigation*		m_pNavigationCom	= { nullptr };
	vector<CCollider*>  m_pColliderComs;

protected:
	STAT_DESC			m_tStatDesc;
	FIGHT_DESC			m_tFightDesc;
	MOVEMENT_DESC		m_tMoveDesc;

public:
	virtual CCharacter* Clone(void* pArg)= 0;
	virtual void Free() override;
};

END