#include "..\Default\stdafx.h"
#include "State_Macaron_Battle.h"

CState_Macaron_Battle::CState_Macaron_Battle()
{
}

CState_Macaron_Battle::CState_Macaron_Battle(const CState_Macaron_Battle& rhs)
{
}

HRESULT CState_Macaron_Battle::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Macaron_Battle::Enter()
{
	Set_Transform();

	CAnimation* pAnim = m_pModel->Get_Animation(AnimNames_MA[ANIM_MA::BATTLE_APPEAR_MA]);

	if (nullptr == pAnim) return E_FAIL;

	m_pModel->Clear_Animation();

	m_pModel->Set_Animation(pAnim, pAnim->Get_TickPerFrame(), DF_TW_TIME, FALSE);

	m_eProgressID = PROGRESS_ID::APPEAR;

	return S_OK;
}

const wstring CState_Macaron_Battle::Tick(const _double& fTimeDelta)
{
	Check_Progress(fTimeDelta);

	return m_strName;
}

const wstring CState_Macaron_Battle::LateTick()
{
	return Check_Transition();
}

void CState_Macaron_Battle::Exit()
{
	m_eProgressID	= PROGRESS_END;
	m_bAttack		= FALSE;
	m_pEnemy		= nullptr;
	CPlayerController::GetInstance()->SetOff_Player(PLAYER_TYPE::MACARON);
}

const wstring CState_Macaron_Battle::Check_Transition()
{
	return m_strName;
}

void CState_Macaron_Battle::Check_Progress(const _double& fTimeDelta)
{
	switch (m_eProgressID)
	{
	case APPEAR: 
	{
		if (!m_pModel->Is_Tween() && m_pModel->Is_Quater_Animation())
		{
			CAnimation* pAnim = m_pModel->Get_Animation(AnimNames_MA[ANIM_MA::BATTLE_ATTACK_MA]);
			if (nullptr != pAnim)
			{
				Play_Sound();

				m_pModel->Set_Animation(pAnim, pAnim->Get_TickPerFrame() * 0.75f, DF_TW_TIME);
				m_eProgressID = ATTACK;
			}
		}
	}
		break;
	case ATTACK:
	{
		if (!m_pModel->Is_Tween())
		{
			CModel::TweenDesc tDesc = m_pModel->Get_TweenDesc();
			if (!m_bAttack)
			{
				if (0 == tDesc.cur.iCurFrame)
				{
					m_bAttack = TRUE;
					ENGINE_INSTANCE->Play_Sound(EFC_MACARON_JUMP, CHANNEL_ID::PLAYER_MACARON, 1.f);
				}
				else if (15 == tDesc.cur.iCurFrame)
				{
					m_bAttack = TRUE;
					ENGINE_INSTANCE->Play_Sound(EFC_MACARON_LAND, CHANNEL_ID::PLAYER_MACARON, 0.6f);

					if (nullptr != m_pEnemy && m_pEnemy->Is_Active())
						m_pEnemy->Damaged(m_pMacaron, CCharacter::ATK_TYPE::LIGHT);
				}
				else if (45 == tDesc.cur.iCurFrame)
				{
					m_bAttack = TRUE;
					ENGINE_INSTANCE->Play_Sound(EFC_MACARON_SHOT, CHANNEL_ID::PLAYER_MACARON, 0.25f);

					if (nullptr != m_pEnemy && m_pEnemy->Is_Active())
						m_pEnemy->Damaged(m_pMacaron, CCharacter::ATK_TYPE::LIGHT);
				}
			}
			else 
				m_bAttack = FALSE;
		}

		if (!m_pModel->Is_Tween() && m_pModel->Is_TwoThirds_Animation())
		{
			CAnimation* pAnim = m_pModel->Get_Animation(AnimNames_MA[ANIM_MA::BATTLE_DISAPPEAR_MA]);
			if (nullptr != pAnim)
			{
				m_pModel->Set_Animation(pAnim, pAnim->Get_TickPerFrame() * 0.75f, DF_TW_TIME);
				m_eProgressID = DISAPPEAR;
				m_pMacaron->Get_Transform()->Rotate(Vec4::UnitY, DEG2RAD(180.f));
			}
		}
	}
		break;
	case DISAPPEAR:
	{
		if (!m_pModel->Is_Tween() && m_pModel->Is_Quater_Animation())
		{
			Exit();
		}
	}
		break;
	default:
		break;
	}
}

void CState_Macaron_Battle::Set_Transform()
{
	CTransform* pTransform_Macaron	= m_pMacaron->Get_Transform();
	CTransform* pTrnasform_Chai			= CPlayerController::GetInstance()->Get_Player(PLAYER_TYPE::CHAI)->Get_Transform();

	/* 포지션, 룩 설정 */
	list<CGameObject*>* pEnemies = ENGINE_INSTANCE->Get_Layer(ENGINE_INSTANCE->Get_CurLevelIndex(), LayerNames[LAYER_ENEMY]);
	
	if(nullptr != pEnemies && !pEnemies->empty())
	{
		_float		fClosedDist = 1000.f;
		CTransform* pEnemyTransform = nullptr;
		_uint		iNavIndex = 0;
		for (auto& pEnemy : *pEnemies)
		{
			_float fDist = Vec4(pTrnasform_Chai->Get_FinalPosition() - pEnemy->Get_Transform()->Get_FinalPosition()).Length();
			
			if (fDist < fClosedDist)
			{
				fClosedDist = fDist;
				pEnemyTransform = pEnemy->Get_Transform();
				iNavIndex = pEnemy->Get_NavMeshAgent()->Get_Index();
				m_pEnemy = dynamic_cast<CCharacter*>(pEnemy);
			}
		}

		Vec4 vEnemyPos = pEnemyTransform->Get_FinalPosition();
		Vec4 vRelativePos = pEnemyTransform->Get_RelativePosition(Vec4{ 0.f, 0.f, 3.f, 0.f });
		pTransform_Macaron->Set_Position(vRelativePos + vEnemyPos, TRUE);
		pTransform_Macaron->Set_Look(pEnemyTransform->Get_State(CTransform::STATE_LOOK) * -1.f);

		/* 네비 인덱스 설정 */
		m_pMacaron->Get_NavMeshAgent()->Set_CurIndex(iNavIndex);

	}
	else
	{
		Vec4 vPlayerPos		= pTrnasform_Chai->Get_FinalPosition();
		Vec4 vRelativePos	= pTrnasform_Chai->Get_RelativePosition(Vec4{ 1.f, 0.f, 1.f, 0.f });
		pTransform_Macaron->Set_Position(vRelativePos + vPlayerPos, TRUE);
		pTransform_Macaron->Set_Look(pTrnasform_Chai->Get_State(CTransform::STATE_LOOK));

		/* 네비 인덱스 설정 */
		m_pMacaron->Get_NavMeshAgent()->Set_CurIndex(CPlayerController::GetInstance()->Get_Player(PLAYER_TYPE::CHAI)->Get_NavMeshAgent()->Get_Index());
	}

	/* 루트 포지션 초기화  */
	pTransform_Macaron->Set_RootPos(Vec4::Zero, TRUE);
}

void CState_Macaron_Battle::Play_Sound()
{
	_int iRand = 0;
	SOUND_FILE_ID eSoundID = SOUND_FILE_END;
	CHANNEL_ID	  eChannelID = CHANNEL_ID_END;

	switch (m_eProgressID)
	{
	case APPEAR:
	{
		iRand = rand() % 7;

		switch (iRand)
		{
		case 0:
			eSoundID = SOUND_FILE_ID::EFC_MACARON_RESPAWN_00;
			break;
		case 1:
			eSoundID = SOUND_FILE_ID::EFC_MACARON_RESPAWN_01;
			break;
		case 2:
			eSoundID = SOUND_FILE_ID::EFC_MACARON_RESPAWN_02;
			break;
		case 3:
			eSoundID = SOUND_FILE_ID::EFC_MACARON_RESPAWN_03;
			break;
		case 4:
			eSoundID = SOUND_FILE_ID::EFC_MACARON_RESPAWN_04;
			break;
		case 5:
			eSoundID = SOUND_FILE_ID::EFC_MACARON_RESPAWN_05;
			break;
		case 6:
			eSoundID = SOUND_FILE_ID::EFC_MACARON_RESPAWN_06;
			break;
		default:
			break;
		}

		eChannelID = CHANNEL_ID::ETC_PLAYER_CALL;
	}
	break;
	case ATTACK:
	{
	
	}
	break;
	case DISAPPEAR:
	{

	}
	break;
	default:
		break;
	}

	ENGINE_INSTANCE->Play_Sound(eSoundID, eChannelID, 0.6f);
}


CState_Macaron_Battle* CState_Macaron_Battle::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Macaron_Battle* pInstance = new CState_Macaron_Battle();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Macaron_Battle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Macaron_Battle::Clone(void* pArg)
{
	return nullptr;
}

void CState_Macaron_Battle::Free()
{
}
