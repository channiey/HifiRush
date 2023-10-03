#pragma once

#include "Client_Defines.h"
#include "Level.h"

/* 1. CLoader를 통해서 특정 레벨(Logo, GamePlay)에 들어가기전에 한번 거쳐서 들어가는 레벨. */
/* 왜? 특정 레벨에서 사용하기위한 자원들을 미리 생성해주는 작업을 할꺼야. */
/* 특정 레벨(Logo, GamePlay)중에 어떤놈의 자원을 준비해야하는가?!(eNextLevel)받아온다. */

/* 2. 로딩을 하는 동안에 화면에 로딩 씬을 보여준다. */

BEGIN(Client)

class CLevel_Loading final : public CLevel
{
private:
	CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Loading() = default;

public:
	virtual HRESULT Initialize(LEVEL_ID eNextLevel);
	virtual HRESULT Tick(_float fTimeDelta) override;
	virtual HRESULT LateTick(_float fTimeDelta) override;

public:
	static HRESULT Parse_LevelData(const _uint& iLevelID);

private:
	/* 로딩 다음레벨. */
	LEVEL_ID			m_eNextLevel = { LV_END };
	class CLoader*	m_pLoader = { nullptr };

public:
	static class CLevel_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL_ID eNextLevel);
	virtual void Free() override;
};

END