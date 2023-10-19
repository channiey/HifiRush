#include "..\Default\stdafx.h"
#include "State_Chai_Base.h"

CState_Chai_Base::CState_Chai_Base()
{
}

CState_Chai_Base::CState_Chai_Base(const CState_Chai_Base& rhs)
{
}

HRESULT CState_Chai_Base::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName)))
		return E_FAIL;

	m_pChai = dynamic_cast<CChai*>(pOwner);

	if (nullptr == m_pChai)
		return E_FAIL;

	return S_OK;
}

void CState_Chai_Base::Set_LastFramePos()
{
	Vec4 vPos = m_pChai->Get_Transform()->Get_FinalPosition();
	m_pChai->Get_Transform()->Set_Position(vPos);
	m_pChai->Get_Transform()->Set_RootPos(Vec4::Zero);
}

CState* CState_Chai_Base::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_Base::Free()
{
	__super::Free();
}
