#pragma once

#include "Base.h"

BEGIN(Engine)

class CStateMachine;

class ENGINE_DLL CState abstract : public CBase
{
protected:
	CState();
	CState(const CState& rhs);
	virtual ~CState() = default;

public:
	HRESULT Initialize(CStateMachine* pStateMachine, const wstring& strStateName);

public:
	virtual HRESULT			Enter() PURE;
	virtual const wstring&	Tick(const _double& fTimeDelta) PURE;
	virtual const wstring&	LateTick() PURE;
	virtual void			Exit() PURE;

public:
	const wstring&			Get_Name() const { return m_strName; }
	void					Set_Name(const wstring& strName) { m_strName = strName; }

protected:
	virtual const wstring&	Check_Transition() PURE; 

protected:
	CStateMachine*			m_pStateMachine = { nullptr };
	wstring					m_strName = {};

public:
	virtual CState* Clone(void* pArg) = 0;
	virtual void Free();
};

END		