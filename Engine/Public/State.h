#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CState abstract : public CBase
{
protected:
	CState();
	CState(const CState& rhs);
	virtual ~CState() = default;

public:
	virtual HRESULT			Enter() PURE;
	virtual const wstring&	Tick(const _float& fimeDelta) PURE;
	virtual const wstring&	LateTick() PURE;
	virtual void			Exit() PURE;

public:
	const wstring&			Get_Name() const { return m_pName; }
	void					Set_Name(const wstring& strName) { m_pName = strName; }

private:
	virtual const wstring&	Check_Transition() PURE; 

private:
	class CStateMachine*	m_pStateMachine = { nullptr };
	wstring					m_pName = {};

private:
	virtual void Free();
};

END		