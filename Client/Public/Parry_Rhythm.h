#pragma once

#include "Ui.h"

BEGIN(Client)

class CParry_Rhythm final : public CUi
{
	enum TEX_TYPE { WORD_READY, DIALOGUE, BEAT, TYPEEND };

private:
	CParry_Rhythm(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParry_Rhythm(const CGameObject& rhs);
	virtual ~CParry_Rhythm() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_double fTimeDelta);
	virtual void	LateTick(_double fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void	Set_State(const OBJ_STATE& eState) override;
	void			Set_Beat(const _uint& iBeatCount);
	void			On_Beat();

private:
	HRESULT			Ready_Components();
	HRESULT			Bind_ShaderResources();

private:
	void			Calculate_BeatsPos();

private:
	vector<pair<_bool, Vec2>> m_Beats;

public:
	static CParry_Rhythm* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CParry_Rhythm* Clone(void* pArg) override;
	virtual void	Free() override;
};

END