#include "Profiler_Manager.h"

IMPLEMENT_SINGLETON(CProfiler_Manager)

CProfiler_Manager::CProfiler_Manager()
{
}

HRESULT CProfiler_Manager::FinishTick()
{
	ZeroMemory(&m_tStatus, sizeof(PROFILER_DESC));

	return S_OK;
}


void CProfiler_Manager::Free()
{
	__super::Free();
}
