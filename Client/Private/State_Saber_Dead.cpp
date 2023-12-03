#include "..\Default\stdafx.h"
#include "State_Saber_Dead.h"

#include "Effect.h"

CState_Saber_Dead::CState_Saber_Dead()
{
}

CState_Saber_Dead::CState_Saber_Dead(const CState_Saber_Dead& rhs)
{
}

HRESULT CState_Saber_Dead::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Saber_Dead::Enter()
{
	ANIM_SA		eAnimID = ANIM_SA::DIE_HEAVY_01_SA;
	CAnimation* pAnimation = m_pModel->Get_Animation(eAnimID);
	_double		fTimePerFrame = 1 / pAnimation->Get_TickPerSecond();

	m_pModel->Set_Animation(eAnimID, fTimePerFrame, DF_TW_TIME);

	return S_OK;
}

const wstring CState_Saber_Dead::Tick(const _double& fTimeDelta)
{
	return m_strName;
}

const wstring CState_Saber_Dead::LateTick()
{
	return Check_Transition();
}

void CState_Saber_Dead::Exit()
{
	Play_Effect();
}

const wstring CState_Saber_Dead::Check_Transition()
{
	if (!m_pModel->Is_Tween() && m_pModel->Is_ThreeFourths_Animation())
	{
		if (m_pSaber->Is_Active())
		{
			ENGINE_INSTANCE->Play_Sound(EFC_CHAI_KILL, ENEMY_SABER, 0.4f);
			m_pSaber->Return_To_Pool();
		}
	}
	return m_strName;
}

void CState_Saber_Dead::Play_Effect()
{
	CGameObject* pClone = ENGINE_INSTANCE->Pop_Pool(ENGINE_INSTANCE->Get_CurLevelIndex(), L"Effect_Damaged_Enemy");
	if (nullptr != pClone)
	{
		CEffect* pEffect = dynamic_cast<CEffect*>(pClone);
		if (nullptr != pEffect)
		{
			ENGINE_INSTANCE->Play_Sound(SOUND_FILE_ID::EFC_ENEMY_DEAD_EXPLOSION, CHANNEL_ID::ENEMY_EMPTY, 0.7f);

			Vec4 vPos = m_pSaber->Get_Transform()->Get_FinalPosition();
			vPos.y += 1.5f;
			pEffect->Get_Transform()->Set_Position(vPos);
			pEffect->Start_Effect();
		}
	}
}

CState_Saber_Dead* CState_Saber_Dead::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Saber_Dead* pInstance = new CState_Saber_Dead();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Saber_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Saber_Dead::Clone(void* pArg)
{
	return nullptr;
}

void CState_Saber_Dead::Free()
{
}
