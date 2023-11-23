#pragma once

#include "Dynamic.h"

BEGIN(Client)

class CDynamic_Crane final : public CDynamic
{
public:
	enum CHILD_TYPE 
	{ 
		CAM_TARGET,	
		CHILD_END 
	};

	enum PROGRESS_TYPE
	{
		IDLE_CRANE,
		WAIT_CRANE,
		ACTIVE_CRANE,
		ACTIVATED_IDLE_CRANE,
		TYPEEND
	};

	const string m_AnimNames[PROGRESS_TYPE::TYPEEND]
	{ 
		"bg0220_idle_000", 
		"NONE",
		"bg0220_action_000",
		"bg0220_idle_002"
	};

protected:
	CDynamic_Crane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDynamic_Crane(const CDynamic_Crane& rhs);
	virtual ~CDynamic_Crane() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_double fTimeDelta);
	virtual void			LateTick(_double fTimeDelta);
	virtual HRESULT			Render();

public:
	const PROGRESS_TYPE&	Get_Progress() const { return m_eProgress; }

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
	PROGRESS_TYPE			m_eProgress		= PROGRESS_TYPE::TYPEEND;

	const _float			m_fWaitTime		= 0.75f;
	_float					m_fWaitAcc		= 0.f;
	_bool					m_bWait			= FALSE;

public:
	static CDynamic_Crane*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CDynamic_Crane* Clone(void* pArg) override;
	virtual void			Free() override;
};

END