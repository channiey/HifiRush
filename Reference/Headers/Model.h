#pragma once
#include "Component.h"

BEGIN(Engine)

#define MAX_MODEL_TRANSFORMS	600 
#define MAX_MODEL_KEYFRAMES		400 

#define DF_PL_TIME		1.4f
#define DF_TW_TIME		0.2f

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum			TYPE		{ TYPE_NONANIM, TYPE_ANIM, TYPE_END };
	enum			BONE_TYPE	{ BONE_ROOT, BONE_SOCKET_LEFT, BONE_SOCKET_RIGHT, BONE_END };

	typedef struct	KeyframeDesc
{
	_int	iAnimIndex	= 0;	
	_uint	iCurFrame	= 0;
	_uint	iNextFrame	= 1;
	_float	fRatio		= 0.f;
	_float	fFrameAcc	= 0.f;	
	_float	fAnimAcc	= 0.f;
	_float	fSpeed		= DF_PL_TIME;

	_int	bLoop		= TRUE;

	void ClearAnim()
	{
		iCurFrame	= 0;
		iNextFrame	= 1;
		fRatio		= 0.f;
		fFrameAcc	= 0.f;
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
		cur.iAnimIndex	= 0;
		next.iAnimIndex = -1;
	}

	void ClearNextAnim()
	{
		next.iAnimIndex = -1;
		next.iCurFrame	= 0;
		next.iNextFrame = 0;
		next.fFrameAcc = 0.f;
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
	HRESULT					Update(_float fTimeDelta);
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
	const vector<class CAnimation*>& Get_Animations() const { return m_Animations; }
	const _uint				Get_AnimationCount() const { return (_uint)m_Animations.size(); }
	const KEYFRAME_DESC		Get_CurAnimation() const { return m_TweenDesc.cur; }
	const _uint				Get_CurAnimationIndex() const { return m_TweenDesc.cur.iAnimIndex; }
	const TWEEN_DESC		Get_TweenDesc() const { return m_TweenDesc; }

	_matrix					Get_PivotMatrix() { return XMLoadFloat4x4(&m_PivotMatrix); }
	const TYPE&				Get_Type() const { return m_eModelType; }

public:
	void					Set_Animation(const _uint& iAnimIndex, const _bool& bLoop, const _float& fSpeed, const _float& fTweenDuration);
	void					Set_BoneIndex(const BONE_TYPE& eType, const _int iIndex);

public:
	const _bool&			Is_RootMotion() const { return m_bRootAnimation; }
	const _bool&			Is_Finish_Tween() const {return m_bFinishTween; }

	const _bool				Is_Half_Animation();
	const _bool				Is_OneThirds_Animation();
	const _bool				Is_TwoThirds_Animation();
	const _bool&			Is_Finish_Animation() { return m_bFinishAnimation; }
	const _bool				Is_Tween();

private: 
    HRESULT					Read_BoneData(const string& strPath);
    HRESULT					Read_MeshData(const string& strPath, Matrix PivotMatrix);
    HRESULT					Read_MaterialData(const string& strPath);
    HRESULT					Read_AnimaionData(const string& strPath);

	HRESULT					Create_Texture();
	void					Create_AnimationTransform(uint32 iAnimIndex, vector<ANIM_TRANSFORM>& pAnimTransform);
	void					Create_AnimationTransformCache(uint32 iAnimIndex, vector<ANIM_TRANSFORM_CACHE>& pAnimTransformCache);

	HRESULT					Update_Anim(_float fTimeDelta);

	const Matrix			Get_AnimBoneLocal(const _uint& iAnimIndex, const _uint& iFrameIndex, const BONE_TYPE& eBoneType);
	const Matrix			Get_CurAnimBonefinal(const BONE_TYPE& eBoneType);

	HRESULT					Clear_Cache();

	void					Set_RootPosition_Tween(); /* 트윈 중 */
	void					Set_RootPosition(); /* 트윈 없는 싱글 애니메이션 */

private: 
	TYPE						m_eModelType = TYPE_END;
	Matrix						m_PivotMatrix = {};

	vector<class CBone*>		m_Bones;
	vector<class CMesh*>		m_Meshes;
	vector<MATERIALDESC>		m_Materials;
	vector<class CAnimation*>	m_Animations;

	ID3D11ShaderResourceView*	m_pSrv = { nullptr };
	vector<ANIM_TRANSFORM>		m_AnimTransforms;		/* 루트랑 소켓 매트릭스만 저장 */
	TWEEN_DESC					m_TweenDesc = {};

	_int						m_iPrevAnimIndex = -1;
	Vec4						m_vPrevAnimRoot = {};
	_bool						m_bRootAnimation = TRUE;
	_bool						m_bFinishAnimation = FALSE;
	_bool						m_bFinishTween = FALSE;
	_int						m_AnimBoneIndecies[BONE_END];

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const string& strPath, _fmatrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END