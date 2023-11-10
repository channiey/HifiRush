#include "..\Default\stdafx.h"
#include "State_Saber_Attack.h"

CState_Saber_Attack::CState_Saber_Attack()
{
}

CState_Saber_Attack::CState_Saber_Attack(const CState_Saber_Attack& rhs)
{
}

HRESULT CState_Saber_Attack::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Saber_Attack::Enter()
{
	ANIM_SA			eAnimID = ANIM_SA_END;
	CAnimation*		pAnimation = nullptr;
	_double			fTimePerFrame = 0.f;

	//if (0 == rand() % 2)
	
		eAnimID = ANIM_SA::ATK_FLYINNG_SA;
		pAnimation = m_pModel->Get_Animation(eAnimID);
		fTimePerFrame = CBeatManager::GetInstance()->Get_SPB(4) / (_double)55.f;


		CModel::SoundEventDesc tSoundEventDesc;
		{
			tSoundEventDesc.eChannelID = ENEMY_SABER;
			tSoundEventDesc.eSoundID = EFC_SABER_SWING;
			tSoundEventDesc.fVolume = 0.6f;
			tSoundEventDesc.iFrame = 37;
		}
	
		m_AttackDesc.iAnimIndex = ANIM_SA::ATK_FLYINNG_SA;
		m_AttackDesc.iFrame = 40;
		m_AttackDesc.bSet = TRUE;
	//else
	/*{
		eAnimID = ANIM_SA::ATK_NONEFLYING_SA;
		pAnimation = m_pModel->Get_Animation(eAnimID);
		fTimePerFrame = CBeatManager::GetInstance()->Get_SPB(6) / (_double)85.f;

		CModel::SoundEventDesc tSoundEventDesc;
		{
			tSoundEventDesc.eChannelID = ENEMY_SABER;
			tSoundEventDesc.eSoundID = EFC_SABER_SWING;
			tSoundEventDesc.fVolume = 0.8f;
			tSoundEventDesc.iFrame = 60;
		}
		m_pModel->Set_SoundEvent(tSoundEventDesc);

	}*/
	
	m_pModel->Set_Animation(eAnimID, fTimePerFrame, DF_TW_TIME);
	m_pModel->Set_SoundEvent(tSoundEventDesc);

	m_pSaber->Get_Child(CSaber::CHILD_TYPE::SA_WEAPON_RIGHT)->Get_Collider_Sphere()->Set_Active(TRUE);

	ENGINE_INSTANCE->Play_Sound(EFC_SABER_CHARGE, ENEMY_SABER, 0.6f);

	return S_OK;
}

const wstring CState_Saber_Attack::Tick(const _double& fTimeDelta)
{
	Detect_AttackCollision();

	return m_strName;
}

const wstring CState_Saber_Attack::LateTick()
{
	return Check_Transition();
}

void CState_Saber_Attack::Exit()
{
	m_pSaber->Get_Child(CSaber::CHILD_TYPE::SA_WEAPON_RIGHT)->Get_Collider_Sphere()->Set_Active(FALSE);
}

const wstring CState_Saber_Attack::Check_Transition()
{
	if (m_pSaber->m_tFightDesc.bDamaged)
		return StateNames_SA[STATE_DAMAGED_SA];

	if (m_pModel->Is_Tween())
		return m_strName;

	if (m_pModel->Is_ThreeFourths_Animation())
		return StateNames_SA[STATE_IDLE_SA];

	return m_strName;
}

void CState_Saber_Attack::Detect_AttackCollision()
{
	CModel::KeyframeDesc KeyFrame = m_pModel->Get_TweenDesc().cur;

	if (!m_AttackDesc.bSet || KeyFrame.iAnimIndex != m_AttackDesc.iAnimIndex ||
		KeyFrame.iCurFrame != m_AttackDesc.iFrame)
		return;

	const _float fMaxAngle = 90.f;
	const _float fMaxDist = 5.f;

	Vec3 vEnemyDir = m_pSaber->Get_Transform()->Get_Forward().xyz();

	Vec3 vDirToPlayer = m_pSaber->m_tFightDesc.pTarget->Get_Transform()->Get_FinalPosition().xyz() -
		m_pSaber->Get_Transform()->Get_FinalPosition().xyz();

	//  정규화된 벡터 사이의 각도를 계산, 반환된 라디안 값을 도 단위로 변환(180.0f / XM_PI)
	_float fAngle = XMVectorGetX(XMVector3AngleBetweenNormals(vEnemyDir, XMVector3Normalize(vDirToPlayer))) * (180.0f / XM_PI);

	_float fDist = XMVectorGetX(XMVector3Length(vDirToPlayer));

	//  플레이어의 바라보는 방향으로부터 좌우로 떨어진 각도를 계산
	if (fAngle <= fMaxAngle / 2 && fDist <= fMaxDist)
	{
		m_pSaber->m_tFightDesc.pTarget->Damaged(dynamic_cast<CCharacter*>(m_pSaber));

		m_AttackDesc.bSet = FALSE;
	}

}

CState_Saber_Attack* CState_Saber_Attack::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Saber_Attack* pInstance = new CState_Saber_Attack();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Saber_Attack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Saber_Attack::Clone(void* pArg)
{
	return nullptr;
}

void CState_Saber_Attack::Free()
{
}
