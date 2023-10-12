#pragma once
#include "Component.h"

BEGIN(Engine)

#define MAX_MODEL_TRANSFORMS	700 
#define MAX_MODEL_KEYFRAMES		150 

typedef struct AnimTransform 
{
	using TransformArrayType = array<Matrix, MAX_MODEL_TRANSFORMS>;
	array<TransformArrayType, MAX_MODEL_KEYFRAMES> transforms;

}ANIMTRANSFORM;

typedef struct KeyframeDesc
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

typedef struct TweenDesc
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

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	virtual HRESULT			Initialize_Prototype(const string& strPath, _fmatrix PivotMatrix);
	virtual HRESULT			Initialize(void* pArg);
	HRESULT					Update_Anim(_float fTimeDelta);
	HRESULT					Render(class CShader* pShader, _uint iMeshIndex, _uint iPassIndex = 0);
	
public:
	HRESULT					Bind_Material(class CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* pConstantName); 

public:
	class CBone*			Get_Bone(const char* pNodeName); 
	class CBone*			Get_Bone(const _int& iIndex);
	vector<class CMesh*>*	Get_Meshes() { return &m_Meshes; }
	_uint					Get_MeshCount() const { return (_uint)m_Meshes.size(); }
	_uint					Get_MaterialIndex(_uint iMeshIndex);
	class CAnimation*		Get_Animation(const _uint& iIndex);
	const _uint				Get_AnimationCount() const { return (_uint)m_Animations.size(); }
	const _uint				Get_CurAnimationIndex() const { return m_TweenDesc.cur.iAnimIndex; }
	_matrix					Get_PivotMatrix() { return XMLoadFloat4x4(&m_PivotMatrix); }
	const TYPE&				Get_Type() const { return m_eModelType; }

public:
	void					Set_Animation(const _uint& iAnimIndex, const _bool& bLoop = FALSE, const _float& fSpeed = 0.2f);

private:
    HRESULT					Read_BoneData(const string& strPath);
    HRESULT					Read_MeshData(const string& strPath, Matrix PivotMatrix);
    HRESULT					Read_MaterialData(const string& strPath);
    HRESULT					Read_AnimaionData(const string& strPath);

private:
	HRESULT					Create_Texture();
	void					Create_AnimationTransform(uint32 iAnimIndex, vector<AnimTransform>& pAnimTransform);

private: 
	TYPE						m_eModelType = TYPE_END;
	Matrix						m_PivotMatrix = {};

	vector<class CBone*>		m_Bones;
	vector<class CMesh*>		m_Meshes;
	vector<MATERIALDESC>		m_Materials;
	vector<class CAnimation*>	m_Animations;

	ID3D11ShaderResourceView*	m_pSrv = { nullptr };
	
	TWEEN_DESC					m_TweenDesc = {};
	_int						m_iPrevAnimIndex = -1;

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const string& strPath, _fmatrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END