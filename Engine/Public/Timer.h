#pragma once
#include "Base.h"
#include "Engine_Defines.h"

BEGIN(Engine)

class CTimer :	public CBase
{
	
private:
	CTimer();
	virtual ~CTimer() = default;
	
public:
	HRESULT	Ready_Timer();
	_double	Compute_TimeDelta();
	
private:
	LARGE_INTEGER			m_FrameTime;
	LARGE_INTEGER			m_FixTime;
	LARGE_INTEGER			m_LastTime;
	LARGE_INTEGER			m_CpuTick;

	_double					m_dTimeDelta;

public:
	static CTimer*		Create(void);
private:
	virtual void Free();
};

END