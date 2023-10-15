#include "..\Default\stdafx.h"
#include "State_Chai_Base.h"

CState_Chai_Base::CState_Chai_Base()
{
}

CState_Chai_Base::CState_Chai_Base(const CState_Chai_Base& rhs)
{
}

CState* CState_Chai_Base::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_Base::Free()
{
	__super::Free();
}
