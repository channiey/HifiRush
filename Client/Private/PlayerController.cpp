#include "../Default/stdafx.h"

#include "PlayerController.h"
#include "EngineInstance.h"

#include "Chai.h"
#include "Peppermint.h"
#include "Macaron.h"
#include "Korsica.h"

IMPLEMENT_SINGLETON(CPlayerController)

CPlayerController::CPlayerController()
{
}

HRESULT CPlayerController::Initialize()
{
	m_eCurControlPlayerType = PLAYER_TYPE::CHAI;

	return S_OK;
}

void CPlayerController::Tick(_double fTimeDelta)
{
	m_eCurControlPlayerType;

	Update_Player(fTimeDelta);
}

void CPlayerController::LateTick(_double fTimeDelta)
{
}

HRESULT CPlayerController::SetOn_Player(const PLAYER_TYPE& eType)
{
	if (PLAYER_TYPE::TYPEEND <= eType || nullptr == m_tPlayerDesc[eType].pPlayer ||
		PLAYER_STATE::WAIT != m_tPlayerDesc[eType].eState)
		return E_FAIL;

	m_tPlayerDesc[eType].pPlayer->Set_State(CGameObject::STATE_ACTIVE);

	m_tPlayerDesc[eType].eState = PLAYER_STATE::APPEAR;

	/* 사운드 재생 */
	Play_Sound(eType);

	return S_OK;
}

HRESULT CPlayerController::SetOff_Player(const PLAYER_TYPE& eType)
{
	if (PLAYER_TYPE::TYPEEND <= eType || nullptr == m_tPlayerDesc[eType].pPlayer ||
		PLAYER_STATE::APPEAR != m_tPlayerDesc[eType].eState)
		return E_FAIL;

	m_tPlayerDesc[eType].pPlayer->Set_State(CGameObject::STATE_UNACTIVE);

	m_tPlayerDesc[eType].eState = PLAYER_STATE::COOLTIME;

	/* 현재 컨트롤러 플레이어 대상 설정 */
	if (PLAYER_TYPE::CHAI != m_eCurControlPlayerType)
		m_eCurControlPlayerType = PLAYER_TYPE::CHAI;

	return S_OK;
}

HRESULT CPlayerController::Change_ControlPlayer(const PLAYER_TYPE& eType)
{
	if (PLAYER_TYPE::TYPEEND <= eType || nullptr == m_tPlayerDesc[eType].pPlayer)
		return E_FAIL;

	m_eCurControlPlayerType = eType;

	return S_OK;
}

const PLAYER_STATE CPlayerController::Get_PlayerState(const PLAYER_TYPE& eType)
{
	if (PLAYER_TYPE::TYPEEND <= eType || nullptr == m_tPlayerDesc[eType].pPlayer)
		return PLAYER_STATE::STATE_END;

	return m_tPlayerDesc[eType].eState;
}

const PLAYER_DESC CPlayerController::Get_PlayerDesc(const PLAYER_TYPE& eType)
{
	if (PLAYER_TYPE::TYPEEND <= eType || nullptr == m_tPlayerDesc[eType].pPlayer)
		return PLAYER_DESC{};

	return m_tPlayerDesc[eType];
}

const _bool CPlayerController::Is_Controll(const PLAYER_TYPE eType)
{
	if (eType == m_eCurControlPlayerType)
		return TRUE;

	return FALSE;
}

CCharacter* CPlayerController::Get_Player(const PLAYER_TYPE& eType)
{
	if (PLAYER_TYPE::TYPEEND <= eType)
		return nullptr;

	return m_tPlayerDesc[eType].pPlayer;
}

HRESULT CPlayerController::Add_Player(CCharacter* pCharacter, const PLAYER_TYPE& eType)
{
	if (PLAYER_TYPE::TYPEEND <= eType || nullptr != m_tPlayerDesc[eType].pPlayer)
		return E_FAIL;

	m_tPlayerDesc[eType].pPlayer = pCharacter;

	if (PLAYER_TYPE::CHAI == eType)
		m_tPlayerDesc[eType].eState = PLAYER_STATE::APPEAR;
	else
		m_tPlayerDesc[eType].eState = PLAYER_STATE::WAIT;

	return S_OK;
}

void CPlayerController::Update_Player(_double fTimeDelta)
{
	for (size_t i = 0; i < PLAYER_TYPE::TYPEEND; i++)
	{
		if (PLAYER_TYPE::CHAI == i) continue;

		if (PLAYER_STATE::COOLTIME == m_tPlayerDesc[i].eState)
		{
			m_tPlayerDesc[i].fCoolTimeAcc += fTimeDelta;

			if (m_tPlayerDesc[i].fCoolTimeLimit <= m_tPlayerDesc[i].fCoolTimeAcc)
			{
				m_tPlayerDesc[i].eState = PLAYER_STATE::WAIT;
				m_tPlayerDesc[i].fCoolTimeAcc = 0.f;
			}
		}
	}
}

void CPlayerController::Play_Sound(const PLAYER_TYPE& eType)
{
	const _int iRand = rand() % 4;
	SOUND_FILE_ID eSoundID = SOUND_FILE_END;

	switch (eType)
	{
	case PEPPERMINT :
		switch (iRand)
		{
		case 0 :
			eSoundID = SOUND_FILE_ID::EFC_CHAI_CALL_PEPPER_00;
			break;
		case 1:
			eSoundID = SOUND_FILE_ID::EFC_CHAI_CALL_PEPPER_01;
			break;
		case 2:
			eSoundID = SOUND_FILE_ID::EFC_CHAI_CALL_PEPPER_02;
			break;
		case 3:
			eSoundID = SOUND_FILE_ID::EFC_CHAI_CALL_PEPPER_03;
			break;

		default:
			break;
		}
		break;
	case MACARON:
		switch (iRand)
		{
		case 0:
			eSoundID = SOUND_FILE_ID::EFC_CHAI_CALL_MACARON_00;
			break;
		case 1:
			eSoundID = SOUND_FILE_ID::EFC_CHAI_CALL_MACARON_01;
			break;
		case 2:
			eSoundID = SOUND_FILE_ID::EFC_CHAI_CALL_MACARON_02;
			break;
		case 3:
			eSoundID = SOUND_FILE_ID::EFC_CHAI_CALL_MACARON_03;
			break;

		default:
			break;
		}
		break;
	case KORSICA:
		switch (iRand)
		{
		case 0:
			eSoundID = SOUND_FILE_ID::EFC_CHAI_CALL_KORSICA_00;
			break;
		case 1:
			eSoundID = SOUND_FILE_ID::EFC_CHAI_CALL_KORSICA_01;
			break;
		case 2:
			eSoundID = SOUND_FILE_ID::EFC_CHAI_CALL_KORSICA_02;
			break;
		case 3:
			eSoundID = SOUND_FILE_ID::EFC_CHAI_CALL_KORSICA_03;
			break;

		default:
			break;
		}
		break;
	default:
		break;
	}

	ENGINE_INSTANCE->Play_Sound(eSoundID, CHANNEL_ID::PLAYER_CHAI, 0.8f);
}


void CPlayerController::Free()
{
	__super::Free();
}

