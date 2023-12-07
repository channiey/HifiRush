#pragma once

#include "Ui.h"

BEGIN(Client)

class CCharacter;

class CUi_Dialouge final : public CUi
{
	enum TEX_TYPE 
	{ 
		FACE_CHAI, FACE_PEPPER, FACE_MACARON, FACE_KORSICA, 
		FACE_HINTBOT, 
		FACE_SABER,
	
		DLG_WIN_00, DLG_WIN_01, DLG_WIN_02, DLG_WIN_03, 
		
		DLG_NAME, 


		// << : New 
		FACE_ROBOT_SECURITY,
		FACE_ROBOT_CLEAN,
		FACE_ROBOT_JOY,
		FACE_ROBOT_BRIDGE,

		FACE_GUNNER,
		FACE_BLADER,
		
		TYPEEND 
	};

	enum PROGRESS_ID
	{
		INTRO, FIX, OUTTRO, PROGRESS_END
	};

	typedef struct tagDialougeDesc
	{
		wstring strText;

		TEX_TYPE eCharacterType;

		_bool	bSound = FALSE;

		_uint	iSoundID;
		_uint	iChannelID;
		_float	fVolume;

	}DIALOUGE_DESC;

private:
	CUi_Dialouge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUi_Dialouge(const CGameObject& rhs);
	virtual ~CUi_Dialouge() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_double fTimeDelta);
	virtual void	LateTick(_double fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT			On_Dialouge(_uint iCharacterType, const wstring& strText, _bool bSound = FALSE, _uint iSoundID = 0, _uint iChannelID = 0, _float fVolume = 0.f);
	void			Off_Dialouge();
	void			Clear_Dialouge();

private:
	HRESULT			Ready_Components();
	HRESULT			Bind_ShaderResources();

private:
	void			Render_Text();
	void			Set_CharacterName();

	const _bool		Check_Dialouge();

private:

	/* Progress */
	_float 			m_fAcc				= 0.f;
	const _float	m_fLimit			= 2.8f;

	LERP_FLOAT_DESC m_tLerpDesc_Alpha	= {};
	const _float	m_fInOutTroTime		= 0.1f;
	PROGRESS_ID		m_eProgress			= PROGRESS_ID::PROGRESS_END;
	

	/* Input */
	TEX_TYPE		m_eCurCharacter		= TEX_TYPE::TYPEEND;

	_tchar			m_szCharacterName[MAX_PATH]		= TEXT("");
	_tchar			m_szConversationText[MAX_PATH]	= TEXT("");

	/* Transform */
	const Vec2		m_vNamePos				= { 410.f, 565.f };
	const Vec2		m_vDlgWindowPos			= { 400.f, 621.f };
	const _float	m_fNameTextSize			= 0.4f;
	const _float	m_fDlgWindowTextSize	= 0.5f;


	queue<DIALOUGE_DESC> m_Dialouges;

public:
	static CUi_Dialouge*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CUi_Dialouge*	Clone(void* pArg) override;
	virtual void			Free() override;
};

END