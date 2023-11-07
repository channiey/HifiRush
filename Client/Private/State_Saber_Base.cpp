#include "..\Default\stdafx.h"
#include "State_Saber_Base.h"

CState_Saber_Base::CState_Saber_Base()
{
}

CState_Saber_Base::CState_Saber_Base(const CState_Saber_Base& rhs)
{
}

HRESULT CState_Saber_Base::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName)))
		return E_FAIL;

	m_pSaber = dynamic_cast<CSaber*>(pOwner);

	if (nullptr == m_pSaber) 
		return E_FAIL;

	m_pModel = m_pSaber->Get_Model();

	if (nullptr == m_pModel)
		return E_FAIL;

	return S_OK;
}

const wstring CState_Saber_Base::Choice_NextState()
{
	vector<_float> Probabilities = { 0.6f, 0.3f, 0.1f };

	random_device			rd;
	mt19937					gen(rd());
	discrete_distribution<> distribution(Probabilities.begin(), Probabilities.end());

	const _int iChoiceIndex = distribution(gen);

	STATE_SA eNextState = STATE_IDLE_SA;

	switch (iChoiceIndex)
	{
	case 0:
	{
		eNextState = STATE_MOVE_SA;
	}
		break;
	case 1:
	{
		eNextState = STATE_IDLE_SA;
	}
		break;
	case 2:
	{
		if(StateNames_SA[STATE_ATTACK_SA] != m_pStateMachine->Get_CurState()->Get_Name())
			eNextState = STATE_ATTACK_SA;
	}
		break;
	default:
		eNextState = STATE_IDLE_SA;
		break;
	}

	return StateNames_SA[eNextState];
}

CState* CState_Saber_Base::Clone(void* pArg)
{
	return nullptr;
}

void CState_Saber_Base::Free()
{
	__super::Free();
}
