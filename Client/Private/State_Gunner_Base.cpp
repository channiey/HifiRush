#include "..\Default\stdafx.h"
#include "State_Gunner_Base.h"

CState_Gunner_Base::CState_Gunner_Base()
{
}

CState_Gunner_Base::CState_Gunner_Base(const CState_Gunner_Base& rhs)
{
}

HRESULT CState_Gunner_Base::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName)))
		return E_FAIL;

	m_pGunner = dynamic_cast<CGunner*>(pOwner);

	if (nullptr == m_pGunner)
		return E_FAIL;

	m_pModel = m_pGunner->Get_Model();

	if (nullptr == m_pModel)
		return E_FAIL;

	return S_OK;
}

const wstring CState_Gunner_Base::Choice_NextState()
{
	vector<_float> Probabilities = { 0.7f, 0.3f }; // move idle attack

	random_device			rd;
	mt19937					gen(rd());
	discrete_distribution<> distribution(Probabilities.begin(), Probabilities.end());

	const _int iChoiceIndex = distribution(gen);

	STATE_GU eNextState = STATE_GU::STATE_IDLE_GU;

	switch (iChoiceIndex)
	{
	case 0:
	{
		eNextState = STATE_MOVE_GU;
	}
	break;
	case 1:
	{
		if (StateNames_GU[STATE_ATTACK_GU] != m_pStateMachine->Get_CurState()->Get_Name())
			eNextState = STATE_ATTACK_GU;
		else
			eNextState = STATE_MOVE_GU;
	}
	break;
	default:
		eNextState = STATE_MOVE_GU;
		break;
	}

	return StateNames_GU[eNextState];
}

const _float CState_Gunner_Base::Get_Distance()
{
	return Vec4::Distance(m_pGunner->Get_Transform()->Get_FinalPosition(),
		m_pGunner->m_tFightDesc.pTarget->Get_Transform()->Get_FinalPosition());
}

void CState_Gunner_Base::Look_Target()
{
	if (!m_pModel->Is_Tween())
	{
		const _float fRotConstNormal = 20.f * 0.016f;

		Vec4 vLook, vDir, vLerpDir;

		vLook = m_pGunner->Get_Transform()->Get_State(CTransform::STATE_LOOK);

		vDir = m_pGunner->m_tFightDesc.pTarget->Get_Transform()->Get_FinalPosition()
				- m_pGunner->Get_Transform()->Get_FinalPosition();

		vLook.y = vDir.y = 0.f;

		vLook.Normalize();
		vDir.Normalize();

		vLerpDir = Vec4::Lerp(vLook, vDir, fRotConstNormal);

		m_pGunner->Get_Transform()->Set_Look(vLerpDir);
	}
}

CState* CState_Gunner_Base::Clone(void* pArg)
{
	return nullptr;
}

void CState_Gunner_Base::Free()
{
	__super::Free();
}
