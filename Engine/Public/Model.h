#pragma once
#include "Component.h"

BEGIN(Engine)

#define MAX_MODEL_TRANSFORMS	600 
#define MAX_MODEL_KEYFRAMES		200 

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum			TYPE		{ TYPE_NONANIM, TYPE_ANIM, TYPE_END };
	enum			BONE_TYPE	{ BONE_ROOT, BONE_SOCKET_LEFT, BONE_SOCKET_RIGHT, BONE_END };

	typedef struct	KeyframeDesc
{
	_int	iAnimIndex	= 0;	
	_uint	iCurFrame	= 0;
	_uint	iNextFrame	= 0;
	_float	fRatio		= 0.f;
	_float	fFrameAcc	= 0.f;	
	_float	fAnimAcc	= 0.f;
	_float	fSpeed		= 1.f;	

	_int	bLoop		= TRUE;

	void ClearAnim()
	{
		iCurFrame	= 0;
		iNextFrame	= 0;
		fRatio		= 0.f;
		fFrameAcc	= 0.f;
	}

}KEYFRAME_DESC;
	typedef struct	TweenDesc
{
	KEYFRAME_DESC cur	= {};
	KEYFRAME_DESC next	= {};

	float fTweenDuration = 0.2f;
	float fTweenRatio	 = 0.f;
	float fTweenAcc		 = 0.f;
	float fPadding		 = 0.f;

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


	vector<class CMesh*>*	Get_Meshes() { return &m_Meshes; }
	_uint					Get_MeshCount() const { return (_uint)m_Meshes.size(); }

	_uint					Get_MaterialIndex(_uint iMeshIndex);

	class CAnimation*		Get_Animation(const _uint& iIndex);
	const vector<class CAnimation*>& Get_Animations() const { return m_Animations; }
	const _uint				Get_AnimationCount() const { return (_uint)m_Animations.size(); }
	const KEYFRAME_DESC		Get_CurAnimation() const { return m_TweenDesc.cur; }
	const _uint				Get_CurAnimationIndex() const { return m_TweenDesc.cur.iAnimIndex; }

	_matrix					Get_PivotMatrix() { return XMLoadFloat4x4(&m_PivotMatrix); }
	const TYPE&				Get_Type() const { return m_eModelType; }

public:
	void					Set_Animation(const _uint& iAnimIndex, const _bool& bLoop = FALSE, const _float& fSpeed = 0.2f);
	void					Set_BoneIndex(const BONE_TYPE& eType, const _int iIndex);

public:
	const _bool&			Is_RootMotion() const { return m_bRootAnimation; }
	const _bool&			Is_FinishAnimation() const { return m_bFinishAnim; }

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
	_bool						m_bFinishAnim = FALSE;
	_bool						m_bTweenFrame = TRUE; // 다음 애니메이션 트위닝시 다음 애님 프레임을 증가시킬지

	_bool						m_bRootAnimation = TRUE;
	_int						m_AnimBoneIndecies[BONE_END];

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const string& strPath, _fmatrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END