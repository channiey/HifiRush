#pragma once
#include "Component.h"

BEGIN(Engine)

#define MAX_BONES 600 /* 셰이더 파일에서도 똑같이 정의 해줘야한다. */
// Bone
#define MAX_MODEL_TRANSFORMS 700 /* 한 애니메이션이 가질 수 있는 최대 뼈 갯수 */
#define MAX_MODEL_KEYFRAMES 300 /* 한 애니메이션이 가질 수 있는 최대 프레임 수 */

struct AnimTransform /* 하나의 애니메이션의 모든 Bone Transform 정보를 2차원 형태로 보관 */
{
	// [ ][ ][ ][ ][ ][ ][ ] ... w
	using TransformArrayType = array<Matrix, MAX_MODEL_TRANSFORMS>;

	// [ ][ ][ ][ ][ ][ ][ ] ... 
	array<TransformArrayType, MAX_MODEL_KEYFRAMES> transforms;

	/* 키프레임 수만큼 transforms를 갖고 있고, */
	/* transforms 안에는 뼈 갯수 만큼 Matrix가 있다. */

	/* 2차 배열 형태로 사용한다. */
};

/* 모델이 가지고 있는 모든 애니메이션 중 현재 실행되고 있는 애니메이션 상태를 나타냄 */
struct KeyframeDesc
{
	/* 공격 , 공격 , 아이들 , 공중 , 런 , 워크 */
	_int	animIndex = 2;	/* 모든 애니메이션 중, 현재 애니메이션 인덱스*/
	_uint	currFrame = 0;
	_uint	nextFrame = 0;
	_float	ratio = 0.f;
	_float	sumTime = 0.f;	/* 프레임 갱신을 위한 acc */
	_float	speed = 1.f;	/* 애니메이션 재생 스피드 */
	Vec2	padding;
};

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	virtual HRESULT Initialize_Prototype(const string& strPath, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg);
	HRESULT			Update_Anim(_float fTimeDelta);	/* 현재 애니메이션이 제어해야할 뼈의 상태를 갱신한다. */
	HRESULT			Update_VTFAnim(_float fTimeDelta);
	HRESULT			Render(class CShader* pShader, _uint iMeshIndex, _uint iPassIndex = 0); /* 스키닝 후, 셰이더에 정점을 전달한다. */
	
	HRESULT			SetUp_OnShader(class CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* pConstantName); /* 셰이더에 현재 모델의 매테리얼 정보를 바인딩한다. */

public:
	class CBone*			Get_Bone(const char* pNodeName); 
	class CBone*			Get_Bone(const _int& iIndex);
	vector<class CMesh*>*	Get_Meshes() { return &m_Meshes; }
	_uint					Get_NumMeshes() const { return (_uint)m_Meshes.size(); }
	_uint					Get_MaterialIndex(_uint iMeshIndex);
	const _uint				Get_AnimationCount() const { return (_uint)m_Animations.size(); }
	class CAnimation*		Get_AnimationByIndex(const _uint& iIndex);
	const _uint				Get_CurAnimationIndex() const { return m_iCurrentAnimIndex; }
	_matrix					Get_PivotMatrix() { return XMLoadFloat4x4(&m_PivotMatrix); }
	const TYPE&				Get_Type() const { return m_eModelType; }

public:
	void					Set_Animation(_uint iAnimIndex) { m_iCurrentAnimIndex = iAnimIndex; }

private:
    HRESULT					Read_BoneData(const string& strPath);
    HRESULT					Read_MeshData(const string& strPath, Matrix PivotMatrix);
    HRESULT					Read_MaterialData(const string& strPath);
    HRESULT					Read_AnimaionData(const string& strPath);

private: /* VTF */
	HRESULT					Create_Texture();
	HRESULT					Create_Texture_Ver2();
	void					Create_AnimationTransform(uint32 iAnimIndex, vector<AnimTransform>& pAnimTransform);

private: 
	TYPE						m_eModelType = TYPE_END;
	_float4x4					m_PivotMatrix = {};

	vector<class CBone*>		m_Bones;
	vector<class CMesh*>		m_Meshes;
	vector<MATERIALDESC>		m_Materials;
	vector<class CAnimation*>	m_Animations;

	/* Cache */
	_uint						m_iCurrentAnimIndex = 0;
	_float4x4					m_BoneMatrices[MAX_BONES] = {};

	/* VTF */
	/* 한 모델이 사용하는, 모든 애니메이션의, Bone Transform 정보 보관 -> 3차 배열 형태*/
	/* 한 애니메이션의 Bone Transform이 AnimTransform이다. 이걸 애니메이션 갯수만큼 벡터로 가지고 있다.*/
	ID3D11ShaderResourceView*	m_pSrv = { nullptr };
	KeyframeDesc				m_keyframeDesc = {};

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const string& strPath, _fmatrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END