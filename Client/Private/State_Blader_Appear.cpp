#include "..\Default\stdafx.h"
#include "State_Blader_Appear.h"

#include "UiManager.h"
#include "UI.h"

#ifdef _DEBUG
#include "ImGui_Manager.h"
#endif // _DEBUG

CState_Blader_Appear::CState_Blader_Appear()
{
}

CState_Blader_Appear::CState_Blader_Appear(const CState_Blader_Appear& rhs)
{
}

HRESULT CState_Blader_Appear::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Blader_Appear::Enter()
{
	CUi* pUi = CUiManager::GetInstance()->Get_UI(UI_ID::UI_HUD_BOSS);
	if (nullptr == pUi)
		return E_FAIL;

	pUi->Set_Target(m_pBlader);
	pUi->Set_State(CGameObject::STATE_ACTIVE);

	CAnimation* pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::APPEAR_BL]);

	if (nullptr == pAnimation)
		return E_FAIL;

	m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame(), DF_TW_TIME);

	return S_OK;
}

const wstring CState_Blader_Appear::Tick(const _double& fTimeDelta)
{
	return m_strName;
}

const wstring CState_Blader_Appear::LateTick()
{
	/* 두번째 등장 애니메이션 재생 */
	Play_SecondAnimation();

	return Check_Transition();
}

void CState_Blader_Appear::Exit()
{

}

const wstring CState_Blader_Appear::Check_Transition()
{
	if (!m_pModel->Is_Tween() && AnimNames_BL[ANIM_BL::PARRY_EVENT_START_BL] == m_pModel->Get_CurAnimation()->Get_Name() && m_pModel->Is_ThreeFourths_Animation())
		return StateNames_BL[STATE_BL::STATE_IDLE_BL];

	return m_strName;
}

void CState_Blader_Appear::Play_SecondAnimation()
{
	CAnimation* pCurAnimation = m_pModel->Get_CurAnimation();

	if (!m_pModel->Is_Tween() && AnimNames_BL[ANIM_BL::APPEAR_BL] == pCurAnimation->Get_Name() && 36 <= m_pModel->Get_TweenDesc().cur.iCurFrame)
	{
		CAnimation* pAnimation = m_pModel->Get_Animation(AnimNames_BL[ANIM_BL::PARRY_EVENT_START_BL]);

		if (nullptr != pAnimation)
			m_pModel->Set_Animation(pAnimation, pAnimation->Get_TickPerFrame(), DF_TW_TIME);
	}
}

CState_Blader_Appear* CState_Blader_Appear::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Blader_Appear* pInstance = new CState_Blader_Appear();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Blader_Appear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Blader_Appear::Clone(void* pArg)
{
	return nullptr;
}

void CState_Blader_Appear::Free()
{
}
