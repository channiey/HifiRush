#pragma once

#include "Dynamic.h"

BEGIN(Client)

class CDynamic_Macaron_Wall final : public CDynamic
{
	enum MODEL_TYPE
	{
		BROKEN_WALL,
		UNBROKEN_WALL,
		DEBRIS,
		MODEL_TYPEEND
	};

	enum ANIM_MA_WALL
	{
		IDLE_MA_WALL,
		ACTIVE_MA_WALL,
		TYPEEND_MA_WALL
	};

	const string m_AnimNames[ANIM_MA_WALL::TYPEEND_MA_WALL]
	{
		"IDLE",
		"ACTIVE"
	};

protected:
	CDynamic_Macaron_Wall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDynamic_Macaron_Wall(const CDynamic_Macaron_Wall& rhs);
	virtual ~CDynamic_Macaron_Wall() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_double fTimeDelta);
	virtual void			LateTick(_double fTimeDelta);
	virtual HRESULT			Render();

protected:
	virtual HRESULT			Set_On() override;
	virtual HRESULT			Set_Off() override;

protected:
	HRESULT					Ready_Components();
	virtual HRESULT			Ready_Chilren() { return S_OK; }

protected:
	HRESULT					Bind_ShaderResources();

public:
	static CDynamic_Macaron_Wall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CDynamic_Macaron_Wall* Clone(void* pArg) override;
	virtual void			Free() override;
};

END