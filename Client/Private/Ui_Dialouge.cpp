#include "..\Default\stdafx.h"
#include "..\Public\Ui_Dialouge.h"

#include "EngineInstance.h"
#include "ImGui_Manager.h"
#include "UiManager.h"

CUi_Dialouge::CUi_Dialouge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUi(pDevice, pContext)
{

}

CUi_Dialouge::CUi_Dialouge(const CGameObject& rhs)
	: CUi(rhs)
{

}

HRESULT CUi_Dialouge::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUi_Dialouge::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Load()))
		return E_FAIL;

	if (FAILED(CUiManager::GetInstance()->Add_UI(UI_ID::UI_DIALOUGE, this)))
		return E_FAIL;

	return S_OK;
}

void CUi_Dialouge::Tick(_double fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	switch (m_eProgress)
	{
	case CUi_Dialouge::PROGRESS_ID::INTRO: 
	{
		m_tLerpDesc_Alpha.Update(fTimeDelta);

		if (!m_tLerpDesc_Alpha.bActive)	
			m_eProgress = CUi_Dialouge::PROGRESS_ID::FIX;
	}
		break;
	case CUi_Dialouge::PROGRESS_ID::FIX:
	{
		m_fAcc += fTimeDelta;
		if (m_fLimit <= m_fAcc)
		{
			m_eProgress = CUi_Dialouge::PROGRESS_ID::OUTTRO;
			m_tLerpDesc_Alpha.Start(1.f, 0.f, m_fInOutTroTime, LERP_MODE::EASE_IN);
		}
	}
		break;
	case CUi_Dialouge::PROGRESS_ID::OUTTRO:
	{
		m_tLerpDesc_Alpha.Update(fTimeDelta);

		if (!m_tLerpDesc_Alpha.bActive)
			Off_Dialouge();
	}
		break;
	default:
		break;
	}
}

void CUi_Dialouge::LateTick(_double fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUi_Dialouge::Render()
{
	if (m_TextureLocalDesc.empty()) return E_FAIL;

	if (FAILED(__super::Render())) return E_FAIL;

	if (FAILED(Bind_ShaderResources())) return E_FAIL;

	/* Render Texture */
	Matrix matWorldOrigin = m_pTransformCom->Get_WorldMat();

	for (size_t i = 0; i < m_pTextureComs.size(); i++)
	{
		if (nullptr == m_pTextureComs[i]) continue;

		/* 다이얼 로그창과 현재 캐릭터 얼굴만 렌더링 한다. */
		if ((DLG_WIN_00 <= i && DLG_NAME >= i) || m_eCurCharacter == i)
		{
			const Matrix matNewWorld = Matrix::CreateScale(m_TextureLocalDesc[i].vSize.x, m_TextureLocalDesc[i].vSize.y, 1.f)
				* Matrix::CreateTranslation(m_TextureLocalDesc[i].vPos.x, m_TextureLocalDesc[i].vPos.y, 0.f)
				* matWorldOrigin;

			m_pTransformCom->Set_WorldMat(matNewWorld);

			if (FAILED(m_pTransformCom->Bind_ShaderResources(m_pShaderCom, "g_WorldMatrix")))
				return E_FAIL;

			if (FAILED(m_pTextureComs[i]->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
				return E_FAIL;

			m_pShaderCom->Begin(1);

			m_pVIBufferCom->Render();

		}
	}

	m_pTransformCom->Set_WorldMat(matWorldOrigin);

	/* Render Text */
	if (CUi_Dialouge::PROGRESS_ID::FIX == m_eProgress)
		Render_Text();

	return S_OK;
}

HRESULT CUi_Dialouge::On_Dialouge(_uint iCharacterType, const wstring& strText, _bool bSound, _uint iSoundID, _uint iChannelID, _float fVolume)
{
	/* 사전 필터링 */
	if(!m_Dialouges.empty())
	{
		DIALOUGE_DESC desc;
		desc = m_Dialouges.front();

		if (desc.eCharacterType == iCharacterType && desc.strText == strText)
			return S_OK;
	}
	
	wstring wstrText{ m_szConversationText };

	if (m_eCurCharacter == iCharacterType && wstrText == strText.c_str())
		return S_OK;


	DIALOUGE_DESC desc;
	{
		desc.strText = strText;
		desc.eCharacterType = (CUi_Dialouge::TEX_TYPE)iCharacterType;

		if (bSound)
		{
			desc.bSound = TRUE;
			desc.iSoundID = iSoundID;
			desc.iChannelID = iChannelID;
			desc.fVolume = fVolume;
		}
	}
	m_Dialouges.push(desc);

	Set_State(OBJ_STATE::STATE_ACTIVE);
	
	Check_Dialouge();

	return S_OK;
}

void CUi_Dialouge::Off_Dialouge()
{
	Clear_Dialouge();

	if (Check_Dialouge())
		return;

	Set_State(OBJ_STATE::STATE_UNACTIVE);
}

void CUi_Dialouge::Clear_Dialouge()
{
	m_eCurCharacter = CUi_Dialouge::TEX_TYPE::TYPEEND;

	m_eProgress		= CUi_Dialouge::PROGRESS_ID::PROGRESS_END;

	m_fAcc = 0.f;

	m_tLerpDesc_Alpha.Clear();


}

HRESULT CUi_Dialouge::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LV_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Vector Resize */
	m_pTextureComs.resize(TEX_TYPE::TYPEEND);
	m_TextureLocalDesc.resize(TEX_TYPE::TYPEEND);

	/* Load Texture */
	{
		/* Window */
		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_Talk_window_00"),
			TEXT("Com_Texture_T_Talk_window_00"), (CComponent**)&m_pTextureComs[TEX_TYPE::DLG_WIN_00])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_Talk_window_01"),
			TEXT("Com_Texture_T_Talk_window_01"), (CComponent**)&m_pTextureComs[TEX_TYPE::DLG_WIN_01])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_Talk_window_02"),
			TEXT("Com_Texture_T_Talk_window_02"), (CComponent**)&m_pTextureComs[TEX_TYPE::DLG_WIN_02])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_Talk_window_03"),
			TEXT("Com_Texture_T_Talk_window_03"), (CComponent**)&m_pTextureComs[TEX_TYPE::DLG_WIN_03])))
			return E_FAIL;

		/* Name */
		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_Talk_window_Name_00"),
			TEXT("Com_Texture_T_Talk_window_Name_00"), (CComponent**)&m_pTextureComs[TEX_TYPE::DLG_NAME])))
			return E_FAIL;

		/* Face */
		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_talk_chai"),
			TEXT("Com_Texture_T_talk_chai"), (CComponent**)&m_pTextureComs[TEX_TYPE::FACE_CHAI])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_talk_pepper"),
			TEXT("Com_Texture_T_talk_pepper"), (CComponent**)&m_pTextureComs[TEX_TYPE::FACE_PEPPER])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_talk_macaron"),
			TEXT("Com_Texture_T_talk_macaron"), (CComponent**)&m_pTextureComs[TEX_TYPE::FACE_MACARON])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_talk_korsica"),
			TEXT("Com_Texture_T_talk_korsica"), (CComponent**)&m_pTextureComs[TEX_TYPE::FACE_KORSICA])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_talk_hintbot"),
			TEXT("Com_Texture_T_talk_hintbot"), (CComponent**)&m_pTextureComs[TEX_TYPE::FACE_HINTBOT])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_talk_saber"),
			TEXT("Com_Texture_T_talk_saber"), (CComponent**)&m_pTextureComs[TEX_TYPE::FACE_SABER])))
			return E_FAIL;

		// << New 

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_talk_Worker3050_normal_00"),
			TEXT("Com_Texture_T_talk_Worker3050_normal_00"), (CComponent**)&m_pTextureComs[TEX_TYPE::FACE_ROBOT_SECURITY])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_talk_robotA"),
			TEXT("Com_Texture_T_talk_robotA"), (CComponent**)&m_pTextureComs[TEX_TYPE::FACE_ROBOT_CLEAN])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_talk_Worker3000_normal_00"),
			TEXT("Com_Texture_T_talk_Worker3000_normal_00"), (CComponent**)&m_pTextureComs[TEX_TYPE::FACE_ROBOT_JOY])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_Talk_NPC_SkillUpMachine"),
			TEXT("Com_Texture_T_Talk_NPC_SkillUpMachine"), (CComponent**)&m_pTextureComs[TEX_TYPE::FACE_ROBOT_BRIDGE])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_talk_Gunner"),
			TEXT("Com_Texture_T_talk_Gunner"), (CComponent**)&m_pTextureComs[TEX_TYPE::FACE_GUNNER])))
			return E_FAIL;

		if (FAILED(__super::Add_Component(LV_STATIC, TEXT("T_talk_T_ModelViewerIcon_Enemy_RedBrutal"),
			TEXT("Com_Texture_T_talk_T_ModelViewerIcon_Enemy_RedBrutal"), (CComponent**)&m_pTextureComs[TEX_TYPE::FACE_BLADER])))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CUi_Dialouge::Bind_ShaderResources()
{
	const _float fAlpha = (_float)m_tLerpDesc_Alpha.fCurValue;
	
	m_pShaderCom->Bind_RawValue("g_Alpha", &fAlpha, sizeof(_float));

	return S_OK;
}

void CUi_Dialouge::Render_Text()
{
	/* Render Name */
	ENGINE_INSTANCE->Render_Font(TEXT("Font_Default"), m_szCharacterName, m_vNamePos, DirectX::Colors::White, 0.f, Vec2::Zero, m_fNameTextSize);

	/* Render Conversaton */
	ENGINE_INSTANCE->Render_Font(TEXT("Font_Default"), m_szConversationText, m_vDlgWindowPos, DirectX::Colors::Black, 0.f, Vec2::Zero, m_fDlgWindowTextSize);
}

void CUi_Dialouge::Set_CharacterName()
{
	switch (m_eCurCharacter)
	{
	case Client::CUi_Dialouge::FACE_CHAI:
		wsprintf(m_szCharacterName, L"Chai");
		break;
	case Client::CUi_Dialouge::FACE_PEPPER:
		wsprintf(m_szCharacterName, L"Pepper");
		break;
	case Client::CUi_Dialouge::FACE_MACARON:
		wsprintf(m_szCharacterName, L"Macaron");
		break;
	case Client::CUi_Dialouge::FACE_KORSICA:
		wsprintf(m_szCharacterName, L"Korsica");
		break;
	case Client::CUi_Dialouge::FACE_HINTBOT:
		wsprintf(m_szCharacterName, L"Hintbot");
		break;
	case Client::CUi_Dialouge::FACE_SABER:
		wsprintf(m_szCharacterName, L"Saber");
		break;
	case Client::CUi_Dialouge::FACE_ROBOT_SECURITY:
		wsprintf(m_szCharacterName, L"NPK_6540");
		break;
	case Client::CUi_Dialouge::FACE_ROBOT_CLEAN:
		wsprintf(m_szCharacterName, L"NPK_1240");
		break;
	case Client::CUi_Dialouge::FACE_ROBOT_JOY:
		wsprintf(m_szCharacterName, L"NPK_1310");
		break;
	case Client::CUi_Dialouge::FACE_ROBOT_BRIDGE:
		wsprintf(m_szCharacterName, L"NPK_3440");
		break;
	case Client::CUi_Dialouge::FACE_GUNNER:
		wsprintf(m_szCharacterName, L"Gunner");
		break;
	case Client::CUi_Dialouge::FACE_BLADER:
		wsprintf(m_szCharacterName, L"Blader");
		break;
	default:
		wsprintf(m_szCharacterName, L"???");
		break;
	}
}

const _bool CUi_Dialouge::Check_Dialouge()
{
	if (CUi_Dialouge::PROGRESS_ID::PROGRESS_END == m_eProgress && !m_Dialouges.empty())
	{
		DIALOUGE_DESC desc = m_Dialouges.front();

		m_Dialouges.pop();

		wsprintf(m_szConversationText, desc.strText.c_str());

		m_eCurCharacter = desc.eCharacterType;

		m_eProgress = CUi_Dialouge::PROGRESS_ID::INTRO;

		m_tLerpDesc_Alpha.Start(0.f, 1.f, m_fInOutTroTime, LERP_MODE::EASE_IN);

		Set_CharacterName();

		if (desc.bSound)
		{
			ENGINE_INSTANCE->Play_Sound(desc.iSoundID, desc.iChannelID, desc.fVolume);
		}

		return TRUE;
	}
	else
		return FALSE;
}

CUi_Dialouge* CUi_Dialouge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUi_Dialouge* pInstance = new CUi_Dialouge(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUi_Dialouge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CUi_Dialouge* CUi_Dialouge::Clone(void* pArg)
{
	CUi_Dialouge* pInstance = new CUi_Dialouge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUi_Dialouge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUi_Dialouge::Free()
{
	__super::Free();
}
