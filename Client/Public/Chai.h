#pragma once

#include "Character.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CInput;

class CChai final : public CCharacter
{
	enum ChildType { CH_WEAPON_RIGHT, CH_END };

	enum CHAI_STATE {IDLE, WALK, RUN, DASH, JUMP, DOUBLEJUMP, FALL, LAND, ATTACK_1, ATTACK_2, ATTACK_3, DAMAGED, DEAD, NONE, STATE_END };
	
	wstring m_StateNames[CHAI_STATE::STATE_END]{L"IDLE", L"WALK", L"RUN", L"DASH", L"JUMP", L"DOUBLEJUMP", L"FALL", L"LAND", L"ATTACK_1", L"ATTACK_2", L"ATTACK_3", L"DAMAGED", L"DEAD", L"NONE"};

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
	HRESULT				Ready_ChildObjects();

private:
	HRESULT				Bind_ShaderResources();

private: 
	CStateMachine*		m_pStateMachineCom = { nullptr };
	CInput*				m_pInput = { nullptr };

public:
	static CChai* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CCharacter* Clone(void* pArg) override; 
	virtual void Free() override;

#pragma region friend class 

private:
	friend class CState_Chai_Base;
	friend class CState_Chai_Idle;

#pragma endregion

};

END