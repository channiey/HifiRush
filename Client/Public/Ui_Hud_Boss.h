#pragma once

#include "Ui.h"

BEGIN(Client)
class CCharacter;

class CUi_Hud_Boss final : public CUi
{
	enum TEX_TYPE
	{
		DROP_HEALTH,

		SIDE_ICON_LEFT,
		SIDE_ICON_RIGHT,

		HEALTH_EMPTY_0,
		HEALTH_EMPTY_1,
		HEALTH_EMPTY_2,
		HEALTH_EMPTY_3,

		HEALTH_CRACKED_0, // 미사용
		HEALTH_CRACKED_1, // 미사용
		HEALTH_CRACKED_2, // 미사용
		HEALTH_CRACKED_3, // 미사용

		HEALTH_FULL_0,
		HEALTH_FULL_1,
		HEALTH_FULL_2,
		HEALTH_FULL_3,

		HEALTH_DAMAGED_0, // 데미지 블러 이미지
		HEALTH_DAMAGED_1, // 데미지 블러 이미지
		HEALTH_DAMAGED_2, // 데미지 블러 이미지
		HEALTH_DAMAGED_3, // 데미지 블러 이미지

		LOGO,

		TYPEEND
	};

private:
	CUi_Hud_Boss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUi_Hud_Boss(const CGameObject& rhs);
	virtual ~CUi_Hud_Boss() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_double fTimeDelta);
	virtual void	LateTick(_double fTimeDelta);
	virtual HRESULT Render();

public:
	virtual HRESULT	Set_Target(CGameObject* pTarget) override;

private:
	HRESULT			Ready_Components();
	HRESULT			Bind_ShaderResources();

private:
	void			Drop(_double fTimeDelta);
	

private:
	LERP_FLOAT_DESC	m_tLerpDesc;
	_bool			m_bMax = FALSE;

	CCharacter*		m_pBoss = nullptr;

public:
	static CUi_Hud_Boss* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CUi_Hud_Boss* Clone(void* pArg) override;
	virtual void	Free() override;
};

END