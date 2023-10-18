#include "..\Default\stdafx.h"
#include "State_Chai_Attack.h"

CState_Chai_Attack::CState_Chai_Attack()
{
}

CState_Chai_Attack::CState_Chai_Attack(const CState_Chai_Attack& rhs)
{
}

HRESULT CState_Chai_Attack::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Chai_Attack::Enter()
{
	m_pChai->Get_Model()->Set_Animation(ANIM_CH::ATK_LIGHT_00, FALSE);

	return S_OK;
}

const wstring& CState_Chai_Attack::Tick(const _float& fTimeDelta)
{
	CModel* pModel = m_pChai->Get_Model();
	
	if (GAME_INSTNACE->Key_Down(VK_LBUTTON) && pModel->Is_OneThirds_Animation())
	{
		cout << "IN\n";

		++m_pChai->m_tFightDesc.iStep;

		switch (m_pChai->m_tFightDesc.iStep)
		{
		case 0:
			m_pChai->Get_Model()->Set_Animation(ANIM_CH::ATK_LIGHT_01, FALSE);
			break;
		case 1:
			m_pChai->Get_Model()->Set_Animation(ANIM_CH::ATK_LIGHT_02, FALSE);
			break;
		case 2:
		{
			m_pChai->Get_Model()->Set_Animation(ANIM_CH::ATK_LIGHT_03, FALSE);
			m_pChai->m_tFightDesc.iStep = -1;
		}
		break;
		default:
			break;
		}
	}

	return m_strName;
}

const wstring& CState_Chai_Attack::LateTick()
{

	return Check_Transition();
}

void CState_Chai_Attack::Exit()
{
	m_pChai->m_tFightDesc.iStep = -1;
}

const wstring& CState_Chai_Attack::Check_Transition()
{
	if (m_pChai->Get_Model()->Is_Tween())
		return m_strName;

	if(m_pChai->Get_Model()->Is_TwoThirds_Animation())
	{
		if (Input::Move())
		{
			return StateNames[STATE_RUN];
		}
		else
		{
			return StateNames[STATE_IDLE];
		}
	}

	return m_strName;
}

CState_Chai_Attack* CState_Chai_Attack::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Chai_Attack* pInstance = new CState_Chai_Attack();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Chai_Attack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Chai_Attack::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_Attack::Free()
{
}
