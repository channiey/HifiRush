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
	m_tAttackDesc.Reset();

	CModel* pModel = m_pChai->Get_Model();
	CModel::TweenDesc desc = pModel->Get_TweenDesc();
	
	if (Input::LBtn())
	{
		m_tAttackDesc.iAnimIndex		= ANIM_CH::ATK_LIGHT_00;
		m_tAttackDesc.fTweenTime		= DF_TW_TIME * 0.5f;
		m_tAttackDesc.dPrevAnimCheckFrame = (_double)15.f;
		m_tAttackDesc.dTimePerFrame		= CBeatManager::GetInstance()->Get_SPB(1) / (_double)15.f;

		m_tAttackDesc.eAttackType		= ATTACK_TYPE::LIGHT;

		m_tAttackDesc.tSoundEventDesc.iFrame = 10;
	}
	else if (Input::RBtn())
	{
		m_tAttackDesc.iAnimIndex = ANIM_CH::ATK_HEAVY_00;
		m_tAttackDesc.fTweenTime = DF_TW_TIME * 0.5f;
		m_tAttackDesc.dPrevAnimCheckFrame = (_double)27.f;
		m_tAttackDesc.dTimePerFrame = CBeatManager::GetInstance()->Get_SPB(2) / (_double)30.f;

		m_tAttackDesc.eAttackType = ATTACK_TYPE::HEAVY;

		m_tAttackDesc.tSoundEventDesc.iFrame = 15;
	}
	else if (Input::MBtn())
	{
		
	}

	m_tAttackDesc.tSoundEventDesc.eSoundID = EFC_CHAI_ATTACK_COMBO_00;
	m_tAttackDesc.tSoundEventDesc.eChannelID = PLAYER_CHAI;
	m_tAttackDesc.tSoundEventDesc.fVolume = EfcVolumeChai;

	m_pChai->m_tFightDesc.iStep++;
	m_tAttackDesc.bSet = TRUE;

	//m_pChai->Get_Child(CChai::CHILD_TYPE::CH_WEAPON_RIGHT)->Get_Collider_Sphere()->Set_Active(TRUE);

	m_tAttackDescForCol = m_tAttackDesc;

	return S_OK;
}

const wstring CState_Chai_Attack::Tick(const _double& fTimeDelta)
{
	if (!CPlayerController::GetInstance()->Is_Controll(PLAYER_TYPE::CHAI))
		return StateNames_CH[STATE_IDLE_CH];

	Set_AttackDesc();
	Detect_AttackCollision();

	CModel* pModel = m_pChai->Get_Model();
	CModel::TweenDesc desc = pModel->Get_TweenDesc();
	//cout << desc.cur.iAnimIndex << "\t" << desc.next.iAnimIndex << "\t" << desc.cur.iCurFrame << "\t" << desc.next.iCurFrame << "\t" << desc.cur.tSoundEventDesc.eSoundID << endl;

	if (m_tAttackDesc.bSet)
	{
		/* 첫 공격 */
		if (0 == m_pChai->m_tFightDesc.iStep && CBeatManager::GetInstance()->Is_InverseBeat())
		{
			pModel->Set_Animation(m_tAttackDesc.iAnimIndex, m_tAttackDesc.dTimePerFrame, m_tAttackDesc.fTweenTime);
			pModel->Set_SoundEvent(m_tAttackDesc.tSoundEventDesc);

			m_tAttackDesc.bSet = FALSE;

			m_tAttackDesc.bFirstAttack = TRUE;

			ENGINE_INSTANCE->Play_Sound(EFC_CHAI_BESTTIMMING, ETC_NONE_00, 0.8f);

		}/* 콤보 공격 */
		else if(1 <= m_pChai->m_tFightDesc.iStep && !pModel->Is_Tween())
		{
			if (m_tAttackDesc.dPrevAnimCheckFrame <= desc.cur.iCurFrame)
			{
				pModel->Set_Animation(m_tAttackDesc.iAnimIndex, m_tAttackDesc.dTimePerFrame, m_tAttackDesc.fTweenTime);
				pModel->Set_SoundEvent(m_tAttackDesc.tSoundEventDesc);
				
				m_tAttackDescForCol = m_tAttackDesc;

				ENGINE_INSTANCE->Play_Sound(EFC_CHAI_BESTTIMMING, ETC_NONE_00, 0.8f);

				m_tAttackDesc.bSet = FALSE;
			}
		}
	}
	else
	{

	}
	return m_strName;
}

const wstring CState_Chai_Attack::LateTick()
{
	if (!CPlayerController::GetInstance()->Is_Controll(PLAYER_TYPE::CHAI))
		return StateNames_CH[STATE_IDLE_CH];

	return Check_Transition();
}

void CState_Chai_Attack::Exit()
{
	m_tAttackDesc.Reset();

	//m_pChai->Get_Child(CChai::CHILD_TYPE::CH_WEAPON_RIGHT)->Get_Collider_Sphere()->Set_Active(FALSE);

	m_pChai->m_tFightDesc.iStep = -1;
}

const wstring CState_Chai_Attack::Check_Transition()
{
	if (!m_tAttackDesc.bFirstAttack)
		return m_strName;

	if (!m_tAttackDesc.bSet)
	{
		CModel*	pModel = m_pChai->Get_Model();
		CModel::TweenDesc desc = pModel->Get_TweenDesc();

		if (ANIM_CH::ATK_LIGHT_00 == m_tAttackDesc.iAnimIndex)
		{
			if (!pModel->Is_Tween() && m_pModel->Is_Half_Animation())
				return StateNames_CH[STATE_IDLE_CH];
		}
		else if (ANIM_CH::ATK_LIGHT_02 == m_tAttackDesc.iAnimIndex)
		{
			if (!pModel->Is_Tween() && 40 <= desc.cur.iCurFrame)
				return StateNames_CH[STATE_IDLE_CH];
		}
		else if (ANIM_CH::ATK_LIGHT_03 == m_tAttackDesc.iAnimIndex)
		{
			if(!pModel->Is_Tween() && 70 <= desc.cur.iCurFrame )
				return StateNames_CH[STATE_IDLE_CH];
		}
		else if (ANIM_CH::ATK_HEAVY_00 == m_tAttackDesc.iAnimIndex)
		{
			if (!pModel->Is_Tween() && 70 <= desc.cur.iCurFrame)
				return StateNames_CH[STATE_IDLE_CH];
		}
		else if (ANIM_CH::ATK_HEAVY_01 == m_tAttackDesc.iAnimIndex)
		{
			if (!pModel->Is_Tween() && 65 <= desc.cur.iCurFrame)
				return StateNames_CH[STATE_IDLE_CH];
		}
		else if (ANIM_CH::ATK_HEAVY_02 == m_tAttackDesc.iAnimIndex)
		{
			if (!pModel->Is_Tween() && 70 <= desc.cur.iCurFrame)
				return StateNames_CH[STATE_IDLE_CH];
		}
		else
		{
			if(!pModel->Is_Tween() && pModel->Is_TwoThirds_Animation())
				return StateNames_CH[STATE_IDLE_CH];
		}
	}

	return m_strName;
}

void CState_Chai_Attack::Set_AttackDesc()
{
	/* 이미 세팅되어 있는 경우 리턴 */
	if (m_tAttackDesc.bSet)
		return;

	/* 현재 애니메이션에 해당하는 타이밍을 지났다면 리턴 */
	CModel* pModel = m_pChai->Get_Model();
	CModel::TweenDesc desc = pModel->Get_TweenDesc();

	if (m_tAttackDesc.dPrevAnimCheckFrame <= desc.cur.iCurFrame)
		return;
	
	/* 다음 공격 세팅 */
	Input::Attack();
	if (Input::LBtn())
	{
		if (0 == m_pChai->m_tFightDesc.iStep)
		{
			m_tAttackDesc.iAnimIndex = ANIM_CH::ATK_LIGHT_01;
			m_tAttackDesc.dPrevAnimCheckFrame = (_double)15.f;
			m_tAttackDesc.dTimePerFrame = CBeatManager::GetInstance()->Get_SPB(1) / (_double)15.f;
			
			m_tAttackDesc.tSoundEventDesc.iFrame = 10;
			m_tAttackDesc.tSoundEventDesc.eSoundID = EFC_CHAI_ATTACK_COMBO_01;

		}
		else if (1 == m_pChai->m_tFightDesc.iStep)
		{
			m_tAttackDesc.iAnimIndex = ANIM_CH::ATK_LIGHT_02;
			m_tAttackDesc.dPrevAnimCheckFrame = (_double)15.f;
			m_tAttackDesc.dTimePerFrame = CBeatManager::GetInstance()->Get_SPB(1) / (_double)15.f;

			m_tAttackDesc.tSoundEventDesc.iFrame = 10;
			m_tAttackDesc.tSoundEventDesc.eSoundID = EFC_CHAI_ATTACK_COMBO_02;

		}
		else if (2 == m_pChai->m_tFightDesc.iStep)
		{
			m_tAttackDesc.iAnimIndex = ANIM_CH::ATK_LIGHT_03;
			m_tAttackDesc.dPrevAnimCheckFrame = (_double)15.f;
			m_tAttackDesc.dTimePerFrame = CBeatManager::GetInstance()->Get_SPB(2) / (_double)30.f;

			m_tAttackDesc.tSoundEventDesc.iFrame = 25;
			m_tAttackDesc.tSoundEventDesc.eSoundID = EFC_CHAI_ATTACK_COMBO_03;

		}
		else if (3 <= m_pChai->m_tFightDesc.iStep)
		{
			return;
		}

		m_tAttackDesc.tSoundEventDesc.eChannelID = PLAYER_CHAI;
		m_tAttackDesc.tSoundEventDesc.fVolume = EfcVolumeChai;

		m_tAttackDesc.eAttackType		= ATTACK_TYPE::LIGHT;
		m_tAttackDesc.fTweenTime		= DF_TW_TIME * 0.5f;
		m_tAttackDesc.bSet = TRUE;

		m_pChai->m_tFightDesc.iStep++;
	}
	else if (Input::RBtn())
	{
		if (0 == m_pChai->m_tFightDesc.iStep)
		{
			m_tAttackDesc.iAnimIndex = ANIM_CH::ATK_HEAVY_01;
			m_tAttackDesc.fTweenTime		= DF_TW_TIME * 0.5f;
			m_tAttackDesc.dPrevAnimCheckFrame = (_double)30.f;
			m_tAttackDesc.dTimePerFrame = CBeatManager::GetInstance()->Get_SPB(2) / (_double)35.f;

			m_tAttackDesc.tSoundEventDesc.iFrame = 25;
			m_tAttackDesc.tSoundEventDesc.eSoundID = EFC_CHAI_ATTACK_COMBO_01;

		}
		else if (1 == m_pChai->m_tFightDesc.iStep)
		{
			m_tAttackDesc.iAnimIndex = ANIM_CH::ATK_HEAVY_02;
			m_tAttackDesc.fTweenTime = DF_TW_TIME * 0.5;
			m_tAttackDesc.dPrevAnimCheckFrame = (_double)35.f;
			m_tAttackDesc.dTimePerFrame = CBeatManager::GetInstance()->Get_SPB(2) / (_double)38.f;

			m_tAttackDesc.tSoundEventDesc.iFrame = 33;
			m_tAttackDesc.tSoundEventDesc.eSoundID = EFC_CHAI_ATTACK_COMBO_02;

		}
		else if (2 <= m_pChai->m_tFightDesc.iStep)
		{
			return;
		}
		
		m_tAttackDesc.tSoundEventDesc.eChannelID = PLAYER_CHAI;
		m_tAttackDesc.tSoundEventDesc.fVolume = EfcVolumeChai;

		m_tAttackDesc.eAttackType = ATTACK_TYPE::HEAVY;
		m_tAttackDesc.bSet = TRUE;

		m_pChai->m_tFightDesc.iStep++;
	}
}

void CState_Chai_Attack::Detect_AttackCollision()
{
	CModel::KeyframeDesc KeyFrame = m_pModel->Get_TweenDesc().cur;

	if (!m_tAttackDescForCol.bSet || KeyFrame.iAnimIndex != m_tAttackDescForCol.iAnimIndex ||
		KeyFrame.iCurFrame != m_tAttackDescForCol.tSoundEventDesc.iFrame)
		return;

	m_tAttackDescForCol.bSet = FALSE;

	Vec4 vPlayerPos = m_pChai->Get_Transform()->Get_FinalPosition();
	Vec3 vPlayerLook = m_pChai->Get_Transform()->Get_Forward().xyz().Normalized();
	const _float fMaxAngle	= 90.f;
	const _float fMaxDist	= 5.f;

	list<CGameObject*>* pEnemyList = ENGINE_INSTANCE->Get_Layer(ENGINE_INSTANCE->Get_CurLevelIndex(), LayerNames[LAYER_ENEMY]);

	if (nullptr == pEnemyList) return;

	for (auto pEnemy : *pEnemyList)
	{
		if (pEnemy != nullptr && pEnemy->Is_Active())
		{
			Vec3 vDirToEnemy = pEnemy->Get_Transform()->Get_FinalPosition().xyz() - vPlayerPos.xyz();

			const _float fAngle = XMVectorGetX(XMVector3AngleBetweenNormals(vPlayerLook, XMVector3Normalize(vDirToEnemy))) * (180.0f / XM_PI);
			const _float fDist = XMVectorGetX(XMVector3Length(vDirToEnemy));

			if (fAngle <= fMaxAngle / 2 && fDist <= fMaxDist)
			{
				dynamic_cast<CCharacter*>(pEnemy)->Damaged(dynamic_cast<CCharacter*>(m_pChai));

				/* 넉백 */
				KnockBack(dynamic_cast<CCharacter*>(pEnemy));
			}
		}
	}
}

void CState_Chai_Attack::KnockBack(CCharacter* pTarget)
{
	/* 플레이어 바라보게 */
	Vec4 vLook = m_pChai->Get_Transform()->Get_FinalPosition() - pTarget->Get_Transform()->Get_FinalPosition();
	pTarget->Get_Transform()->Set_Look(vLook.ZeroY().Normalized());

	/* 넉백 힘 적용 */
	Vec3 vDir = pTarget->Get_Transform()->Get_FinalPosition().xyz() - m_pChai->Get_Transform()->Get_FinalPosition().xyz();

	Vec3 vForce = vDir.ZeroY().Normalized() * m_pChai->m_tPhysicsDesc.fKnockBackPower;
	if (2 == m_pChai->m_tFightDesc.iStep)
		vForce *= 10.f;

	pTarget->Get_Rigidbody()->Add_Force(vForce, CRigidbody::FORCE_MODE::IMPULSE);
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
