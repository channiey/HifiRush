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
	vector<_float> Probabilities = { 0.6f, 0.3f, 0.1f }; // move idle attack

	if (Get_Distance() <= m_fTargetMinDist)
	{
		Probabilities[0] = 0.1f;
		Probabilities[1] = 0.3f;
		Probabilities[2] = 0.6f;
	}

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

const _float CState_Saber_Base::Get_Distance()
{
	return Vec4::Distance(m_pSaber->Get_Transform()->Get_FinalPosition(),
		m_pSaber->m_tFightDesc.pTarget->Get_Transform()->Get_FinalPosition());
}

void CState_Saber_Base::Look_Target()
{
	if (!m_pModel->Is_Tween())
	{
		const _float fRotConstNormal = 20.f * 0.016f;

		Vec4 vLook, vDir, vLerpDir;

		vLook = m_pSaber->Get_Transform()->Get_State(CTransform::STATE_LOOK);

		vDir = m_pSaber->m_tFightDesc.pTarget->Get_Transform()->Get_FinalPosition()
						- m_pSaber->Get_Transform()->Get_FinalPosition();

		vLook.y = vDir.y = 0.f;

		vLook.Normalize();
		vDir.Normalize();

		vLerpDir = Vec4::Lerp(vLook, vDir, fRotConstNormal);

		m_pSaber->Get_Transform()->Set_Look(vLerpDir);
	}
}

CState* CState_Saber_Base::Clone(void* pArg)
{
	return nullptr;
}

void CState_Saber_Base::Free()
{
	__super::Free();
}
