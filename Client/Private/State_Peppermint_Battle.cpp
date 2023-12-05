#include "..\Default\stdafx.h"
#include "State_Peppermint_Battle.h"

#include "Peppermint_Bullet.h"

CState_Peppermint_Battle::CState_Peppermint_Battle()
{
}

CState_Peppermint_Battle::CState_Peppermint_Battle(const CState_Peppermint_Battle& rhs)
{
}

HRESULT CState_Peppermint_Battle::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Peppermint_Battle::Enter()
{
	Set_Transform();

	CAnimation* pAnim = m_pModel->Get_Animation(AnimNames_PE[ANIM_PE::BATTLE_APPEAR_PE]);
	
	if (nullptr == pAnim) return E_FAIL;

	m_pModel->Clear_Animation();

	m_pModel->Set_Animation(pAnim, pAnim->Get_TickPerFrame() * 2.f, DF_TW_TIME, FALSE);

	m_eProgressID = PROGRESS_ID::APPEAR;

	return S_OK;
}

const wstring CState_Peppermint_Battle::Tick(const _double& fTimeDelta)
{
	Check_Progress(fTimeDelta);

	return m_strName;
}

const wstring CState_Peppermint_Battle::LateTick()
{
	return Check_Transition();
}

void CState_Peppermint_Battle::Exit()
{
	m_eProgressID = PROGRESS_END;
	m_bShoot = FALSE;
	CPlayerController::GetInstance()->SetOff_Player(PLAYER_TYPE::PEPPERMINT);
}

const wstring CState_Peppermint_Battle::Check_Transition()
{
	return m_strName;
}

void CState_Peppermint_Battle::Check_Progress(const _double& fTimeDelta)
{	
	switch (m_eProgressID)
	{
	case APPEAR:
	{
		if (!m_pModel->Is_Tween() && m_pModel->Is_Quater_Animation())
		{
			CAnimation* pAnim = m_pModel->Get_Animation(AnimNames_PE[ANIM_PE::BATTLE_SHOOT_PE]);
			if (nullptr != pAnim)
			{
				Play_Sound();

				m_pModel->Set_Animation(pAnim, pAnim->Get_TickPerFrame(), DF_TW_TIME, FALSE);
				m_eProgressID = SHOOT;
			}
		}
	}
		break;
	case SHOOT:
	{
		/* 총알 발사, 사운드 재생 */
		if (!m_pModel->Is_Tween())
		{
			CModel::TweenDesc tDesc = m_pModel->Get_TweenDesc();
			if (5 == tDesc.cur.iCurFrame || 10 == tDesc.cur.iCurFrame || 15 == tDesc.cur.iCurFrame || 20 == tDesc.cur.iCurFrame)
			{
				if (!m_bShoot)
				{
					Play_Sound();

					if(FAILED(Shoot(tDesc.cur.iCurFrame)))
						Exit();

					m_bShoot = TRUE;
				}
			}
			else
				m_bShoot = FALSE;
		}

		/* 다음 단계 */
		if (!m_pModel->Is_Tween() && m_pModel->Is_TwoThirds_Animation())
		{
			CAnimation* pAnim = m_pModel->Get_Animation(AnimNames_PE[ANIM_PE::BATTLE_DISAPPEAR_PE]);
			if (nullptr != pAnim)
			{
				m_pModel->Set_Animation(pAnim, pAnim->Get_TickPerFrame() * 1.5f, DF_TW_TIME);
				m_eProgressID = DISAPPEAR;

				m_pPeppermint->Get_Transform()->Rotate(Vec4::UnitY, DEG2RAD(180.f));
			}
		}
	}
		break;
	case DISAPPEAR:
	{
		if (!m_pModel->Is_Tween() && m_pModel->Is_TwoThirds_Animation())
		{
			Exit();
		}
	}
		break;
	default:
		Exit();
		break;
	}
}

void CState_Peppermint_Battle::Set_Transform()
{
	CTransform* pTransform_Peppermint	= m_pPeppermint->Get_Transform();
	CTransform* pTrnasform_Chai			= CPlayerController::GetInstance()->Get_Player(PLAYER_TYPE::CHAI)->Get_Transform();

	/* 네비 인덱스 설정 */
	//m_pPeppermint->Get_NavMeshAgent()->Set_CurIndex(CPlayerController::GetInstance()->Get_Player(PLAYER_TYPE::CHAI)->Get_NavMeshAgent()->Get_Index());
	
	/* 루트 포지션 초기화  */
	pTransform_Peppermint->Set_RootPos(Vec4::Zero, TRUE);

	/* 포지션 설정 */
	Vec4 vPlayerPos = pTrnasform_Chai->Get_FinalPosition();
	Vec4 vRelativePos = pTrnasform_Chai->Get_RelativePosition(Vec4{ -1.f, -0.25f, -1.f, 0.f });
	pTransform_Peppermint->Set_Position(vRelativePos + vPlayerPos, TRUE);

	/* 룩 설정 */
	list<CGameObject*>* pEnemies = ENGINE_INSTANCE->Get_Layer(ENGINE_INSTANCE->Get_CurLevelIndex(), LayerNames[LAYER_ENEMY]);

	if (nullptr != pEnemies && !pEnemies->empty())
	{
		Vec4 vLook = { 100.f, 100.f, 100.f, 0.f };
		for (auto& pEnemy : *pEnemies)
		{
			Vec4 vDir = pEnemy->Get_Transform()->Get_FinalPosition() -
				pTransform_Peppermint->Get_FinalPosition();

			if (vDir.Length() <= vLook.Length())
				vLook = vDir;
		}
		pTransform_Peppermint->Set_Look(vLook.ZeroY().Normalized());
	}
	else
	{
		pTransform_Peppermint->Set_Look(pTrnasform_Chai->Get_State(CTransform::STATE_LOOK));
	}
}

void CState_Peppermint_Battle::Play_Sound()
{
	_int iRand = 0;
	SOUND_FILE_ID eSoundID = SOUND_FILE_END;
	CHANNEL_ID	  eChannelID = CHANNEL_ID_END;

	switch (m_eProgressID)
	{
	case APPEAR:
	{
		eSoundID = SOUND_FILE_ID::EFC_PEPPERMINT_RESPAWN_00;
		/*iRand = rand() % 8;

		switch (iRand)
		{
		case 0:
			eSoundID = SOUND_FILE_ID::EFC_PEPPERMINT_RESPAWN_00;
			break;
		case 1:
			eSoundID = SOUND_FILE_ID::EFC_PEPPERMINT_RESPAWN_01;
			break;
		case 2:
			eSoundID = SOUND_FILE_ID::EFC_PEPPERMINT_RESPAWN_02;
			break;
		case 3:
			eSoundID = SOUND_FILE_ID::EFC_PEPPERMINT_RESPAWN_03;
			break;
		case 4:
			eSoundID = SOUND_FILE_ID::EFC_PEPPERMINT_RESPAWN_04;
			break;
		case 5:
			eSoundID = SOUND_FILE_ID::EFC_PEPPERMINT_RESPAWN_05;
			break;
		case 6:
			eSoundID = SOUND_FILE_ID::EFC_PEPPERMINT_RESPAWN_06;
			break;
		case 7:
			eSoundID = SOUND_FILE_ID::EFC_PEPPERMINT_RESPAWN_07;
			break;
		default:
			break;
		}*/

		eChannelID = CHANNEL_ID::ETC_PLAYER_CALL;
	}
		break;
	case SHOOT:
	{
		eSoundID = SOUND_FILE_ID::EFC_PEPPERMINT_SHOOT;
		eChannelID = CHANNEL_ID::PLAYER_PEPPERMINT;
	}
		break;
	case DISAPPEAR:
	{

	}
		break;
	default:
		return;
		break;
	}

	ENGINE_INSTANCE->Play_Sound(eSoundID, eChannelID, 0.8f);
}

HRESULT CState_Peppermint_Battle::Shoot(const _uint& iCurFrame)
{
	CGameObject* pClone = ENGINE_INSTANCE->Pop_Pool(ENGINE_INSTANCE->Get_CurLevelIndex(), L"Projectile_Peppermint_Bullet");

	if (nullptr == pClone)
		return E_FAIL;

	CProjectile* pProjectile = dynamic_cast<CProjectile*>(pClone);

	if (nullptr == pProjectile) return E_FAIL;

	CProjectile::PROJECTILE_DESC tDesc;
	{
		tDesc.pOwner = m_pPeppermint;
		
		tDesc.fSpeedPerSec = 30.f;
		tDesc.vDir = m_pPeppermint->Get_Transform()->Get_State(CTransform::STATE_LOOK);

		CModel::BONE_TYPE		eBontType	= CModel::BONE_END;
		CPeppermint::CHILD_TYPE eChildType	= CPeppermint::CHILD_TYPE::CH_END;
		if (5 == iCurFrame || 15 == iCurFrame)
		{
			eBontType = CModel::BONE_SOCKET_RIGHT;
			eChildType = CPeppermint::CHILD_TYPE::WP_RIGHT;
		}
		else if (10 == iCurFrame || 20 == iCurFrame)
		{
			eBontType = CModel::BONE_SOCKET_LEFT;
			eChildType = CPeppermint::CHILD_TYPE::WP_LEFT;
		}

		Matrix matWorld = m_pPeppermint->Get_Model()->Get_SocketBoneMat(eBontType)* m_pPeppermint->Get_Child((_uint)eChildType)->Get_Transform()->Get_FinalMat();
		
		memcpy(&tDesc.vOrigin, matWorld.m[3], sizeof(Vec4));
	}

	return pProjectile->Shoot(tDesc);
}

CState_Peppermint_Battle* CState_Peppermint_Battle::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Peppermint_Battle* pInstance = new CState_Peppermint_Battle();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Peppermint_Battle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Peppermint_Battle::Clone(void* pArg)
{
	return nullptr;
}

void CState_Peppermint_Battle::Free()
{
}
