#include "..\Default\stdafx.h"
#include "State_Chai_Run.h"

CState_Chai_Run::CState_Chai_Run()
{
}

CState_Chai_Run::CState_Chai_Run(const CState_Chai_Run& rhs)
{
}

HRESULT CState_Chai_Run::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Chai_Run::Enter()
{
	m_pChai->Get_Model()->Set_Animation(4, TRUE, 0.1f); /* TODO:: 애니메이션 다 적용하고 CChai에 있는 enum으로 사용 */

	return S_OK;
}

const wstring& CState_Chai_Run::Tick(const _float& fTimeDelta)
{
	/* 회전 보간 작업 필요 */
	CTransform* pTranform = m_pChai->Get_Transform();

	if (Input::Up())
	{
		Vec4 vMoveDir = GAME_INSTNACE->Get_CurCamera_State(CTransform::STATE_LOOK);
		vMoveDir.y = 0.f;

		pTranform->Translate(vMoveDir.Normalized() * m_pChai->m_tMoveDesc.fMaxForwardSpeed * fTimeDelta);

		_float fYaw = acosf(vMoveDir.Normalized().Dot(pTranform->Get_Forward()));
		
		pTranform->Rotate(pTranform->Get_Up(), fYaw);
		//pTranform->Set_State(CTransform::STATE_LOOK, vRotDir);
	}
	else if (Input::Down())
	{
		Vec4 vMoveDir = GAME_INSTNACE->Get_CurCamera_State(CTransform::STATE_LOOK) * -1.f;
		vMoveDir.y = 0.f;

		pTranform->Translate(vMoveDir.Normalized() * m_pChai->m_tMoveDesc.fMaxForwardSpeed * fTimeDelta);
	}
	else if (Input::Left())
	{
		Vec4 vMoveDir = GAME_INSTNACE->Get_CurCamera_State(CTransform::STATE_RIGHT) * -1.f;;
		vMoveDir.y = 0.f;

		pTranform->Translate(vMoveDir.Normalized() * m_pChai->m_tMoveDesc.fMaxForwardSpeed * fTimeDelta);

	}
	else if (Input::Right())
	{
		Vec4 vMoveDir = GAME_INSTNACE->Get_CurCamera_State(CTransform::STATE_RIGHT);
		vMoveDir.y = 0.f;
		
		pTranform->Translate(vMoveDir.Normalized() * m_pChai->m_tMoveDesc.fMaxForwardSpeed * fTimeDelta);
	}

	return Check_Transition();
}

const wstring& CState_Chai_Run::LateTick()
{
	// ... 

	return Check_Transition();
}

void CState_Chai_Run::Exit()
{
}

const wstring& CState_Chai_Run::Check_Transition()
{
	if(Input::Move())
		return StateNames_CH[STATE_CH::RUN];

	///* First Condition */
	//if (m_pChai->m_tFightDesc.bDamaged)
	//{
	//	m_pChai->m_tFightDesc.bDamaged = FALSE;

	//	return StateNames_CH[STATE_CH::DAMAGED];
	//}

	///* Second Conditon */
	//if (Input::Move())
	//{
	//	return StateNames_CH[STATE_CH::RUN]; 
	//}
	//else if (Input::Attack())
	//{
	//	m_pChai->m_tFightDesc.bInAttack = TRUE;

	//	return StateNames_CH[STATE_CH::ATTACK_1];
	//}
	//else if (Input::Shift())
	//{
	//	return StateNames_CH[STATE_CH::DASH];
	//}
	//else if (Input::Space() && m_pChai->m_tMoveDesc.bGround)
	//{
	//	m_pChai->m_tMoveDesc.bGround = FALSE;
	//	m_pChai->m_tMoveDesc.bJump = TRUE;
	//}

	return StateNames_CH[STATE_CH::IDLE];
}

CState_Chai_Run* CState_Chai_Run::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Chai_Run* pInstance = new CState_Chai_Run();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Chai_Run");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Chai_Run::Clone(void* pArg)
{
	return nullptr;
}

void CState_Chai_Run::Free()
{
}
