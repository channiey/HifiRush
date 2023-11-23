#pragma once

#include "Dynamic.h"

BEGIN(Client)

class CDynamic_Macaron_Wall final : public CDynamic
{
	enum MODEL_TYPE
	{
		BROKEN,
		UNBROKEN,
		DEBRIS,
		MODEL_TYPEEND
	};

	enum class STATE_TYPE
	{
		UNBROKEN,
		BROKEN,
		TYPEEND
	};


	enum ANIM_MA_WALL
	{
		BROKE_WALL,
		TYPEEND_MA_WALL
	};

	const string m_AnimNames[ANIM_MA_WALL::TYPEEND_MA_WALL]
	{
		"bg1210_event_020",
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
	virtual void			Check_Progress(_double fTimeDelta) override;

protected:
	HRESULT					Ready_Components();
	virtual HRESULT			Ready_Chilren() { return S_OK; }

protected:
	HRESULT					Bind_ShaderResources();

private:
	STATE_TYPE				m_eStateType				= CDynamic_Macaron_Wall::STATE_TYPE::UNBROKEN;
	CShader*				m_pShaderCom_Dynamic		= { nullptr };
	CShader*				m_pShaderCom_Static 		= { nullptr };
	CModel*					m_pModelComs[CDynamic_Macaron_Wall::MODEL_TYPE::MODEL_TYPEEND];

public:
	static CDynamic_Macaron_Wall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CDynamic_Macaron_Wall* Clone(void* pArg) override;
	virtual void			Free() override;
};

END