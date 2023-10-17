#pragma once

#include "Character.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)
enum STATE_CH 
{ 
	ANNOY_00,
	ANNOY_01,

	ATK_FINAL_B_00,
	ATK_FINAL_A_00,
	ATK_FINAL_A_01,

	ATK_LIGHT_00,
	ATK_LIGHT_01,
	ATK_LIGHT_02,
	ATK_LIGHT_03,

	ATK_RUN_A_00,
	ATK_RUN_A_01,

	ATK_SINGLE_00,
	ATK_SINGLE_01,
	ATK_SINGLE_02,
	ATK_SINGLE_03,
	ATK_SINGLE_04,

	ATK_SPC_0,
	ATK_SPC_1,
	ATK_SPC_2,

	CEREMONY_00,

	CLEAR_00,
	CLEAR_01,

	DMG_STRONG,
	DMG_LIGHT,
	DMG_ELEC_00,
	DMG_HEAP_00,
	DMG_HEAP_01,
	DMG_HEAP_02,

	DASH_BACK_00,
	DASH_FRONT_00,
	DASH_LEFT_00,
	DASH_RIGHT_00,

	DOUBLE_JUMP_00,
	DOUBLE_JUMP_FALL_00,

	FALL_00,

	IDLE_00,

	JUMP_00,
	JUMP_DASH_00,
	JUMP_DASH_01,

	LAND_00,
	
	PARRY_00,
	PARRY_01,
	PARRY_02,

	RUN_00,
	TALK_00,

	STATE_END 
};

class CChai final : public CCharacter
{
	enum CHILD_TYPE	{ CH_WEAPON_RIGHT, CH_END };

protected:
	CChai(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CChai(const CCharacter& rhs);
	virtual ~CChai() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_float fTimeDelta);
	virtual void		LateTick(_float fTimeDelta);
	virtual HRESULT		Render();

private:
	HRESULT				Ready_Components();
	HRESULT				Ready_Chilren();

private:
	HRESULT				Bind_ShaderResources();

private:
	virtual void		OnCollision_Enter(CGameObject* pGameObject) override;
	virtual void		OnCollision_Stay(CGameObject* pGameObject) override;
	virtual void		OnCollision_Exit(CGameObject* pGameObject) override;

private: 
	CStateMachine*			m_pStateMachineCom = { nullptr };
	vector<wstring>			m_StateNames;

public:
	static CChai* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CCharacter* Clone(void* pArg) override; 
	virtual void Free() override;

private:
	/* Base */
	friend class CState_Chai_Base;

	/* General */
	friend class CState_Chai_Idle;

	/* Movement */
	friend class CState_Chai_Run;
	friend class CState_Chai_Dash;
	friend class CState_Chai_Jump;
	friend class CState_Chai_DoubleJump;
	
	/* Action */
	friend class CState_Chai_Attack_1;
	friend class CState_Chai_Damaged;
	friend class CState_Chai_Parry;
};

END