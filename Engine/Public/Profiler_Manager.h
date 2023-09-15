#pragma once
#include "Base.h"

BEGIN(Engine)

class CProfiler_Manager final : public CBase
{
	/* ��� ���� ����׽� ���� ������ �������� �ȵȴ�. */

	DECLARE_SINGLETON(CProfiler_Manager)

	typedef struct tagProfilerDesc
	{
		_float fFPS			= 0.f;
		_float fTris		= 0.f; 
		_uint iDrawCall		= 0; 
		_uint iSetPassCall	= 0; 
		_uint iAnim			= 0; 

	}PROFILER_DESC;

private:
	CProfiler_Manager();
	virtual ~CProfiler_Manager() = default;

public:
	HRESULT FinishTick();

public:
	const _bool&			Is_Active() const				{ return m_bActive; }
	const PROFILER_DESC&	Get_Status() const				{ return m_tStatus; }

public:
	void					Set_Active(const _bool bActive) { m_bActive = bActive; }
	void					Set_FPS(const _float& fFPS)		{ if (!m_bActive) return; m_tStatus.fFPS = fFPS; }

public:
	void					Add_Tris(const _uint& iTris)	{ if (!m_bActive) return; m_tStatus.fTris += iTris; }
	void					Add_Anim()						{ if (!m_bActive) return; ++m_tStatus.iAnim; }
	void					Add_DrawCall()					{ if (!m_bActive) return; ++m_tStatus.iDrawCall; }
	void					Add_SetPassCall()				{ if (!m_bActive) return; ++m_tStatus.iSetPassCall; }

private:
	_bool					m_bActive	= { FALSE }; // �������ϸ� on off (����� + IMGUI On �϶��� ����)

	PROFILER_DESC			m_tStatus	= {};

public: 
	virtual void Free() override;
};

END

