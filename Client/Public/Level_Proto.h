#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Proto final : public CLevel
{
private:
	CLevel_Proto(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Proto() = default;

public:
	virtual HRESULT Initialize() override;
	virtual HRESULT Tick(_float fTimeDelta) override;
	virtual HRESULT LateTick(_float fTimeDelta) override;

private:
	void	Picking_Terrain();

public:
	static class CLevel_Proto* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END