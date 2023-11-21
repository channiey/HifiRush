#include "..\Default\stdafx.h"
#include "State_Korsica_Battle.h"

#include "Korsica_Wind.h"

CState_Korsica_Battle::CState_Korsica_Battle()
{
}

CState_Korsica_Battle::CState_Korsica_Battle(const CState_Korsica_Battle& rhs)
{
}

HRESULT CState_Korsica_Battle::Initialize(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	if (FAILED(__super::Initialize(pStateMachine, strStateName, pOwner)))
		return E_FAIL;

	return S_OK;
}

HRESULT CState_Korsica_Battle::Enter()
{
	Set_Transform();

	CAnimation* pAnim = m_pModel->Get_Animation(AnimNames_KO[ANIM_KO::BATTLE_APPEAR_KO]);

	if (nullptr == pAnim) return E_FAIL;

	m_pModel->Clear_Animation();

	m_pModel->Set_Animation(pAnim, pAnim->Get_TickPerFrame() * 2.f, DF_TW_TIME, FALSE);

	m_eProgressID = PROGRESS_ID::APPEAR;

	return S_OK;
}

const wstring CState_Korsica_Battle::Tick(const _double& fTimeDelta)
{
	Check_Progress(fTimeDelta);

	return m_strName;
}

const wstring CState_Korsica_Battle::LateTick()
{
	return Check_Transition();
}

void CState_Korsica_Battle::Exit()
{
	m_eProgressID = PROGRESS_ID::PROGRESS_END;
	m_bAtttack = FALSE;
	CPlayerController::GetInstance()->SetOff_Player(PLAYER_TYPE::KORSICA);
}

void CState_Korsica_Battle::Check_Progress(const _double& fTimeDelta)
{
	switch (m_eProgressID)
	{
	case APPEAR:
	{
		if (!m_pModel->Is_Tween() && m_pModel->Is_Quater_Animation())
		{
			CAnimation* pAnim = m_pModel->Get_Animation(AnimNames_KO[ANIM_KO::BATTLE_ATTACK_KO]);
			if (nullptr != pAnim)
			{
				Play_Sound();

				m_pModel->Set_Animation(pAnim, pAnim->Get_TickPerFrame() * 0.75f, DF_TW_TIME, FALSE);
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
			if (25 == tDesc.cur.iCurFrame || 35 == tDesc.cur.iCurFrame)
			{
				if (!m_bAtttack)
				{
					if (FAILED(Attack()))
						Exit();

					m_bAtttack = TRUE;
				}
			}
			else
				m_bAtttack = FALSE;
		}


		if (!m_pModel->Is_Tween() && m_pModel->Is_TwoThirds_Animation())
		{
			CAnimation* pAnim = m_pModel->Get_Animation(AnimNames_KO[ANIM_KO::BATTLE_DISAPPEAR_KO]);
			if (nullptr != pAnim)
			{
				m_pModel->Set_Animation(pAnim, pAnim->Get_TickPerFrame(), DF_TW_TIME);
				m_eProgressID = DISAPPEAR;

				m_pKorsica->Get_Transform()->Rotate(Vec4::UnitY, DEG2RAD(180.f));
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
		break;
	}
}

void CState_Korsica_Battle::Set_Transform()
{
	CTransform* pTransform_Korsica	= m_pKorsica->Get_Transform();
	CTransform* pTrnasform_Chai		= CPlayerController::GetInstance()->Get_Player(PLAYER_TYPE::CHAI)->Get_Transform();

	/* 루트 포지션 초기화  */
	pTransform_Korsica->Set_RootPos(Vec4::Zero, TRUE);

	/* 네비 인덱스 설정 */
	m_pKorsica->Get_NavMeshAgent()->Set_CurIndex(CPlayerController::GetInstance()->Get_Player(PLAYER_TYPE::CHAI)->Get_NavMeshAgent()->Get_Index());

	/* 포지션 설정 */
	Vec4 vPlayerPos = pTrnasform_Chai->Get_FinalPosition();
	Vec4 vRelativePos = pTrnasform_Chai->Get_RelativePosition(Vec4{ 1.f, 0.f, 0.f, 0.f });
	pTransform_Korsica->Set_Position(vRelativePos + vPlayerPos, TRUE);

	/* 룩 설정 */
	list<CGameObject*>* pEnemies = ENGINE_INSTANCE->Get_Layer(ENGINE_INSTANCE->Get_CurLevelIndex(), LayerNames[LAYER_ENEMY]);

	if (nullptr != pEnemies && !pEnemies->empty())
	{
		Vec4 vLook = { 100.f, 100.f, 100.f, 0.f };
		for (auto& pEnemy : *pEnemies)
		{
			Vec4 vDir = pEnemy->Get_Transform()->Get_FinalPosition() -
				pTransform_Korsica->Get_FinalPosition();

			if (vDir.Length() <= vLook.Length())
				vLook = vDir;
		}
		pTransform_Korsica->Set_Look(vLook.ZeroY().Normalized());
	}
	else
	{
		pTransform_Korsica->Set_Look(pTrnasform_Chai->Get_State(CTransform::STATE_LOOK));
	}
}

void CState_Korsica_Battle::Play_Sound()
{
	_int iRand = rand() % 8;
	SOUND_FILE_ID eSoundID = SOUND_FILE_END;

	switch (iRand)
	{
	case 0:
		eSoundID = SOUND_FILE_ID::EFC_KORSICA_RESPAWN_00;
		break;
	case 1:
		eSoundID = SOUND_FILE_ID::EFC_KORSICA_RESPAWN_01;
		break;
	case 2:
		eSoundID = SOUND_FILE_ID::EFC_KORSICA_RESPAWN_02;
		break;
	case 3:
		eSoundID = SOUND_FILE_ID::EFC_KORSICA_RESPAWN_03;
		break;
	case 4:
		eSoundID = SOUND_FILE_ID::EFC_KORSICA_RESPAWN_04;
		break;
	case 5:
		eSoundID = SOUND_FILE_ID::EFC_KORSICA_RESPAWN_05;
		break;
	case 6:
		eSoundID = SOUND_FILE_ID::EFC_KORSICA_RESPAWN_06;
		break;
	case 7:
		eSoundID = SOUND_FILE_ID::EFC_KORSICA_RESPAWN_07;
		break;
	default:
		break;
	}

	ENGINE_INSTANCE->Play_Sound(eSoundID, CHANNEL_ID::ETC_PLAYER_CALL, 0.8f);
}

HRESULT CState_Korsica_Battle::Attack()
{
	CGameObject* pClone = ENGINE_INSTANCE->Pop_Pool(ENGINE_INSTANCE->Get_CurLevelIndex(), L"Projectile_Korsica_Wind");

	if (nullptr == pClone)
		return E_FAIL;

	CProjectile* pProjectile = dynamic_cast<CProjectile*>(pClone);

	if (nullptr == pProjectile) return E_FAIL;

	CProjectile::PROJECTILE_DESC tDesc;
	{
		tDesc.pOwner = m_pKorsica;

		tDesc.fSpeedPerSec = 25.f;
		tDesc.vDir = m_pKorsica->Get_Transform()->Get_State(CTransform::STATE_LOOK);

		CModel::BONE_TYPE		eBontType = CModel::BONE_SOCKET_RIGHT;
		CKorsica::CHILD_TYPE	eChildType = CKorsica::CHILD_TYPE::WP_RIGHT;

		tDesc.vOrigin = m_pKorsica->Get_Child((_uint)eChildType)->Get_Transform()->Get_FinalPosition();
		tDesc.vOrigin.y += 2.f;
	}

	return pProjectile->Shoot(tDesc);
}

const wstring CState_Korsica_Battle::Check_Transition()
{
	return m_strName;
}


CState_Korsica_Battle* CState_Korsica_Battle::Create(CStateMachine* pStateMachine, const wstring& strStateName, CGameObject* pOwner)
{
	CState_Korsica_Battle* pInstance = new CState_Korsica_Battle();

	if (FAILED(pInstance->Initialize(pStateMachine, strStateName, pOwner)))
	{
		MSG_BOX("Failed to Created : CState_Korsica_Battle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CState* CState_Korsica_Battle::Clone(void* pArg)
{
	return nullptr;
}

void CState_Korsica_Battle::Free()
{
}
