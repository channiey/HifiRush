#pragma once

#include "Base.h"

/* Open_Level*/
/* ���� ���ӳ����� Ȱ���ϰ� �ִ� ������ü�� �ּҸ� �����Ѵ�. */
/* ���� ��ü�ÿ� ���� ������ �ı��Ѵ�. + ���� ���������� �������� �ڿ����� �����Ѵ�. */

/* �����ϰ� �ִ� ������ ƽ�� �ݺ������� ȣ�����ش�. */


BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	void Tick(_double fTimeDelta);
	void LateTick(_double fTimeDelta);

public:
	const _uint& Get_CurLevelIndex() const { return m_iCurrentLevelIndex; }

	const _uint& Get_CurLoadingLevel() const { return m_iCurrentLoadingLevelIndex; }
	void Set_CurLoadingLevel(const _uint iLevel) { m_iCurrentLoadingLevelIndex = iLevel; }
public:
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);

private:
	_uint						m_iCurrentLevelIndex = { 0 };
	class CLevel*				m_pCurrentLevel = { nullptr };
	_uint						m_iCurrentLoadingLevelIndex = { 0 };

public:
	virtual void Free() override;
};

END