#pragma once

#include "Client_Defines.h"
#include "Level.h"

/* 1. CLoader�� ���ؼ� Ư�� ����(Logo, GamePlay)�� �������� �ѹ� ���ļ� ���� ����. */
/* ��? Ư�� �������� ����ϱ����� �ڿ����� �̸� �������ִ� �۾��� �Ҳ���. */
/* Ư�� ����(Logo, GamePlay)�߿� ����� �ڿ��� �غ��ؾ��ϴ°�?!(eNextLevel)�޾ƿ´�. */

/* 2. �ε��� �ϴ� ���ȿ� ȭ�鿡 �ε� ���� �����ش�. */

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
	/* �ε� ��������. */
	LEVEL_ID			m_eNextLevel = { LV_END };
	class CLoader*	m_pLoader = { nullptr };

public:
	static class CLevel_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL_ID eNextLevel);
	virtual void Free() override;
};

END