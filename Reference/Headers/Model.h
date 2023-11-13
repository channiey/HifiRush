#pragma once
#include "Component.h"

BEGIN(Engine)

#define MAX_MODEL_TRANSFORMS	600 
#define MAX_MODEL_KEYFRAMES		400 

#define DF_PL_TIME				1.f
#define DF_TW_TIME				0.1f

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum			TYPE			{ TYPE_NONANIM, TYPE_ANIM, TYPE_END };
	enum			BONE_TYPE		{ BONE_ROOT, BONE_SOCKET_LEFT, BONE_SOCKET_RIGHT, BONE_END };
	enum            ANIM_PROGRESS	{ QUATER, ONE_THIRDS, HALF, TWO_THIRDS, FINISH, PROGRESS_END };

	typedef struct SoundEventDesc
	{
		_int	iFrame = -1;
		_int	eSoundID = -1;
		_int	eChannelID = -1;
		_float	fVolume = 0;

		void Clear()
		{
			iFrame = -1;
			eSoundID = -1;
			eChannelID = -1;
			fVolume = 0;
		}

	}SOUND_EVENT_DESC;

	typedef struct	KeyframeDesc
	{
		_int	iAnimIndex	= -1;	
		_uint	iCurFrame	= 0;
		_uint	iNextFrame	= 1;
		_float	fRatio		= 0.f;

		SOUND_EVENT_DESC tSoundEventDesc = {};

		void ClearAnim()
		{
			iCurFrame	= 0;
			iNextFrame	= 1;
			fRatio		= 0.f;

			tSoundEventDesc.Clear();
		}

	}KEYFRAME_DESC;

	typedef struct	TweenDesc
	{
		KEYFRAME_DESC cur	= {};
		KEYFRAME_DESC next	= {};

		_float fTweenDuration = DF_TW_TIME;
		_float fTweenRatio	 = 0.f;
		_float fTweenAcc		 = 0.f;
		_float fPadding		 = 0.f;

		TweenDesc()
		{
			cur.iAnimIndex	= -1;
			next.iAnimIndex = -1;
		}

		void ClearNextAnim()
		{
			next.iAnimIndex = -1;
			next.iCurFrame	= 0;
			next.iNextFrame = 1;
			next.tSoundEventDesc.Clear();

			fTweenAcc		= 0.f;
			fTweenRatio		= 0.f;
			fTweenDuration = DF_TW_TIME;
		}

	}TWEEN_DESC;

private:
	typedef struct	AnimTransformCache
	{
		using TransformArrayType = array<Matrix, MAX_MODEL_TRANSFORMS>;
		array<TransformArrayType, MAX_MODEL_KEYFRAMES> transforms;

	}ANIM_TRANSFORM_CACHE;
	typedef struct	AnimTransform
	{
		using TransformArrayType = array<Matrix, BONE_END>;
		array<TransformArrayType, MAX_MODEL_KEYFRAMES> transforms;

	}ANIM_TRANSFORM;

private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	virtual HRESULT			Initialize_Prototype(const string& strPath, _fmatrix PivotMatrix);
	virtual HRESULT			Initialize(void* pArg);
	HRESULT					Update(_double fTimeDelta);
	HRESULT					Render(class CShader* pShader, _uint iMeshIndex, _uint iPassIndex = 0);
	
public:
	HRESULT					Bind_Material(class CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* pConstantName); 

public:
	class CBone*			Get_Bone(const char* pNodeName); 
	class CBone*			Get_Bone(const _int& iIndex);
	const Matrix			Get_AnimBoneMat(const BONE_TYPE& eType);
	const Matrix			Get_AnimBoneMatNoneLerp(const BONE_TYPE& eType);
	const Matrix			Get_SocketBoneMat(const BONE_TYPE& eType);
	const Vec4				Get_AnimBonePos(const BONE_TYPE& eType);
	const Vec4				Get_AnimBoneRootNoneLerp();


	vector<class CMesh*>*	Get_Meshes() { return &m_Meshes; }
	_uint					Get_MeshCount() const { return (_uint)m_Meshes.size(); }

	_uint					Get_MaterialIndex(_uint iMeshIndex);

	class CAnimation*		Get_Animation(const _uint& iIndex);
	class CAnimation*		Get_Animation(const string& strName);
	const vector<class CAnimation*>& Get_Animations() const { return m_Animations; }
	const _uint				Get_AnimationCount() const { return (_uint)m_Animations.size(); }
	const KEYFRAME_DESC		Get_CurAnimation() const { return m_TweenDesc.cur; }
	class CAnimation*		Get_CurAnimation() { return m_Animations[m_TweenDesc.cur.iAnimIndex]; }
	const _uint				Get_CurAnimationIndex() const { return m_TweenDesc.cur.iAnimIndex; }
	const TWEEN_DESC&		Get_TweenDesc() const { return m_TweenDesc; }

	_matrix					Get_PivotMatrix() { return XMLoadFloat4x4(&m_PivotMatrix); }
	const TYPE&				Get_Type() const { return m_eModelType; }

public:
	void					Set_Animation(const _uint& iAnimIndex, const _double& dSpeed = DF_PL_TIME, const _float& fTweenDuration = DF_TW_TIME);
	void					Set_Animation(class CAnimation* pAnim, const _double& dSpeed = DF_PL_TIME, const _float& fTweenDuration = DF_TW_TIME);
	void					Set_SoundEvent(_uint iFrame, _uint eSoundID, _uint eChannelID, float fVolume);
	void					Set_SoundEvent(SOUND_EVENT_DESC desc);

	void					Set_AnimationSpeed(const _double& dSpeed);

	void					Set_BoneIndex(const BONE_TYPE& eType, const _int iIndex);
	void					Set_RootAnimation(const _bool bRootAnim) { m_bPrevRootAnimation = m_bRootAnimation; m_bRootAnimation = bRootAnim; }

public:
	const _bool&			Is_RootMotion() const { return m_bRootAnimation; }
	const _bool&			Is_Finish_Tween() const {return m_bFinishTween; }

	const _bool				Is_Quater_Animation();
	const _bool				Is_OneThirds_Animation();
	const _bool				Is_Half_Animation();
	const _bool				Is_TwoThirds_Animation();
	const _bool				Is_ThreeFourths_Animation();
	const _bool&			Is_Finish_Animation() { return m_bFinishAnimation; }
	const _bool				Is_Tween();
	const _bool				Is_Contain_InTween(const _uint& iAnimIndex);
	const _bool				Is_Playing_Animation(const _uint& iAnimIndex);

	const _bool&			Is_StopAnimation() const { return m_bStopAnim; }
	void					Stop_Animation(const _bool& bStop) { m_bStopAnim = bStop; }

private: 
    HRESULT					Read_BoneData(const string& strPath);
    HRESULT					Read_MeshData(const string& strPath, Matrix PivotMatrix);
    HRESULT					Read_MaterialData(const string& strPath);
    HRESULT					Read_AnimaionData(const string& strPath);

	HRESULT					Create_Texture();
	void					Create_AnimationTransform(uint32 iAnimIndex, vector<ANIM_TRANSFORM>& pAnimTransform);
	void					Create_AnimationTransformCache(uint32 iAnimIndex, vector<ANIM_TRANSFORM_CACHE>& pAnimTransformCache);

	HRESULT					Update_Anim(_double fTimeDelta);

	const Matrix			Get_AnimBoneLocal(const _uint& iAnimIndex, const _uint& iFrameIndex, const BONE_TYPE& eBoneType);
	const Matrix			Get_CurAnimBonefinal(const BONE_TYPE& eBoneType);

	HRESULT					Clear_Cache();

	void					Set_RootPosition_Tween(); /* 트윈 중 */
	void					Set_RootPosition(); /* 트윈 없는 싱글 애니메이션 */

	void					Check_SoundEvent();

	class CAnimation* Find_Animation(const string& strName);

private: 
	TYPE						m_eModelType = TYPE_END;
	Matrix						m_PivotMatrix = {};

	vector<class CBone*>		m_Bones;
	vector<class CMesh*>		m_Meshes;
	vector<MATERIALDESC>		m_Materials;
	vector<class CAnimation*>	m_Animations;

	ID3D11ShaderResourceView*	m_pSrv = { nullptr };
	vector<ANIM_TRANSFORM>		m_AnimTransforms;		/* 루트랑 소켓 매트릭스만 저장 (루트는 루트 적용됨, 소켓은 루트 적용안됨) */
	TWEEN_DESC					m_TweenDesc = {};

	Vec4						m_vPrevAnimRoot = {};
	_bool						m_bRootAnimation = TRUE;
	_bool						m_bPrevRootAnimation = TRUE;
	_bool						m_bFinishAnimation = FALSE;
	_bool						m_bFinishTween = FALSE;
	_int						m_AnimBoneIndecies[BONE_END];

	_bool						m_bStopAnim = FALSE;

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const string& strPath, _fmatrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END