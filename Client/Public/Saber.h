#pragma once
#include "Character.h"

BEGIN(Engine)
class CBehaviourTree;
END

BEGIN(Client)

enum ANIM_SA
{
	APPEAR_00,
	APPEAR_01,
	
	ATTACK_JUMP_CUT,
	ATTACK_WAIT_STING,

	CHANGE_IDLE_TO_ATTACK,
	CHANGE_RUN_TO_STOP,
	CHANGE_SIT_TO_STAND,
	CHANGE_STAND_TO_SIT,

	DIE_ELECTRIC,
	DIE_FLY_00,
	DIE_FLY_01,
	DIE_HOMERUN,

	DMG_00,
	DMG_01,
	DMG_02, // 5까지 있음

	DOUBLE_STEP_BACK_00,
	DOUBLE_STEP_FRONT_00,
	DOUBLE_STEP_LEFT_00,
	DOUBLE_STEP_LEFT_01,
	DOUBLE_STEP_LEFT_02,
	DOUBLE_STEP_LEFT_BACK_00,
	DOUBLE_STEP_LEFT_FRONE_00,
	DOUBLE_STRP_RIGHT_00,
	DOUBLE_STRP_RIGHT_01,
	DOUBLE_STRP_RIGHT_02, // 3까지 있음
	DOUBLE_STRP_RIGHT_BACK_00,
	DOUBLE_STRP_RIGHT_FRONT_00,
	DOUBLE_STRP_RIGHT_03,

	ESCAPE_BACK_00,
	ESCAPE_BACK_01,
	ESCAPE_FRONT_00,
	ESCAPE_FRONT_01,
	ESCAPE_FRONT_02,
	ESCAPE_LEFT_00,
	ESCAPE_LEFT_01,
	ESCAPE_LEFT_02,
	ESCAPE_RIGHT_00,
	ESCAPE_RIGHT_01,
	ESCAPE_RIGHT_02,

	DMG_03,
	DMG_04,
	DMG_05,

	IDLE_ATTACK,
	NONE_00,

	RUN_00,
	RUN_01,
	RUN_BACK_00,
	RUN_BACK_01,
	RUN_FRONT_00,
	RUN_LEFT_01,
	RUN_LEFT_02,
	RUN_LEFT_03,
	RUN_LEFT_BACK_00,
	RUN_LEFT_FRONT_00,
	RUN_LEFT_FRONT_01,
	RUN_RIGHT_00,
	RUN_RIGHT_01,
	RUN_RIGHT_02,
	RUN_RIGHT_FRONT_00,
	RUN_RIGHT_BACK_00,
	RUN_RIGHT_FRONT_01,

	SITTED,
	SLEEP,

	STEP_BACK_00,
	STEP_FRONT_00,
	STEP_LEFT_00,
	STEP_LEFT_BACK_00,
	STEP_LEFT_FRONT_00,
	STEP_RIGHT_00,
	STEP_RIGHT_BACK_00,
	STEP_RIGHT_FRONT_00,
	
	TALK_00,
	TALK_01,
	TALK_02,

	TURN_BACK_00,
	TURN_LEFT_00,
	TURN_LEFT_FRONT_00,
	TURN_RIGHT_00,
	TURN_RIGHT_FRONT_00,

	WALK_BACK_00,
	WALK_BACK_01,
	WALK_FRONT_00,
	WALK_LEFT_00,
	WALK_LEFT_01,
	WALK_LEFT_BACK_00,
	WALK_LEFT_FRONT_00,
	WALK_RIGHT_00,
	WALK_RIFHT_01,
	WALK_RIGHT_BACK_00,
	WALK_RIGHT_FRONT_00,
};

class CSaber final : public CCharacter
{
	enum CHILD_TYPE { SA_WEAPON_RIGHT, SA_TRIGGER_TRACKED, SA_END };

private:
	CSaber(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSaber(const CSaber& rhs);
	virtual ~CSaber() = default;

public:
	virtual HRESULT		Initialize_Prototype();
	virtual HRESULT		Initialize(void* pArg);
	virtual void		Tick(_float fTimeDelta);
	virtual void		LateTick(_float fTimeDelta);
	virtual HRESULT		Render();

private:
	HRESULT				Ready_Components();
	HRESULT				Ready_Chilren();
	HRESULT				Ready_BehavoiurTree();

private:
	HRESULT				Bind_ShaderResources();

private:
	virtual void		OnCollision_Enter(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Stay(CCollider* pCollider, const _int& iIndexAsChild = -1) override;
	virtual void		OnCollision_Exit(CCollider* pCollider, const _int& iIndexAsChild = -1) override;

private:
	CBehaviourTree*		m_pBehaviourTreeCom = { nullptr };

public:
	static CSaber* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CSaber* Clone(void* pArg) override;
	virtual void Free() override;

private:
	friend class CNode_Damaged_Saber;
	friend class CNode_Tracked_Saber;
};
END
