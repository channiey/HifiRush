#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Stage_01 final : public CLevel
{
private:
	CLevel_Stage_01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Stage_01() = default;

public:
	virtual HRESULT Initialize() override;
	virtual HRESULT Tick(_float fTimeDelta) override;
	virtual HRESULT LateTick(_float fTimeDelta) override;

private:
	HRESULT Ready_Layer_BackGround(const wstring& strLayerTag);

public:
	static class CLevel_Stage_01* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END