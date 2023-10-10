#pragma once
#include "Component.h"

BEGIN(Engine)

#define MAX_BONES 600 /* ���̴� ���Ͽ����� �Ȱ��� ���� ������Ѵ�. */
// Bone
#define MAX_MODEL_TRANSFORMS 600 /* �� �ִϸ��̼��� ���� �� �ִ� �ִ� �� ���� (ǥ ��)*/
#define MAX_MODEL_KEYFRAMES 500 /* �� �ִϸ��̼��� ���� �� �ִ� �ִ� ������ �� (ǥ ��)*/

struct AnimTransform /* �ϳ��� �ִϸ��̼��� ��� Bone Transform ������ 2���� ���·� ���� */
{
	// [ ][ ][ ][ ][ ][ ][ ] ... 600�� 
	using TransformArrayType = array<Matrix, MAX_MODEL_TRANSFORMS>;

	// [ ][ ][ ][ ][ ][ ][ ] ... 500 ��
	array<TransformArrayType, MAX_MODEL_KEYFRAMES> transforms;

	/* 2�� �迭 ���·� ����Ѵ�. */
};

/* ���� ������ �ִ� ��� �ִϸ��̼� �� ���� ����ǰ� �ִ� �ִϸ��̼� ���¸� ��Ÿ�� */
struct KeyframeDesc
{
	int32	animIndex = 1;	/* ��� �ִϸ��̼� ��, ���� �ִϸ��̼� �ε���*/
	uint32	currFrame = 0;
	uint32	nextFrame = 0;
	float	ratio = 0.f;
	float	sumTime = 0.f;	/* ������ ������ ���� acc */
	float	speed = 1.f;	/* �ִϸ��̼� ��� ���ǵ� */
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

public:
	/* ���� �ִϸ��̼��� �����ؾ��� ���� ���¸� �����Ѵ�. */
	HRESULT Update_Anim(_float fTimeDelta);
	HRESULT Update_VTFAnim(_float fTimeDelta);
	/* ���̴��� ���� ���� ���׸��� ������ ���ε��Ѵ�. */
	HRESULT SetUp_OnShader(class CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* pConstantName);
	/* ��Ű�� ��, ���̴��� ������ �����Ѵ�. */
	HRESULT Render(class CShader* pShader, _uint iMeshIndex, _uint iPassIndex = 0);

public:
	class CBone*			Get_Bone(const char* pNodeName); 
	class CBone*			Get_Bone(const _int& iIndex);
	_uint					Get_NumMeshes() const { return (_uint)m_Meshes.size(); }
	vector<class CMesh*>*	Get_Meshes() { return &m_Meshes; }
	_uint					Get_MaterialIndex(_uint iMeshIndex);
	const _uint				Get_AnimationCount() const { return (_uint)m_Animations.size(); }
	const _uint				Get_CurAnimationIndex() const { return m_iCurrentAnimIndex; }
	class CAnimation*		Get_AnimationByIndex(const _uint& iIndex);
	_matrix					Get_PivotMatrix() { return XMLoadFloat4x4(&m_PivotMatrix); }
	const TYPE&				Get_Type() const { return m_eModelType; }

public:
	void					Set_Animation(_uint iAnimIndex) { m_iCurrentAnimIndex = iAnimIndex; }

private:
    HRESULT					Read_BoneData(const string& strPath);
    HRESULT					Read_MeshData(const string& strPath, Matrix PivotMatrix);
    HRESULT					Read_MaterialData(const string& strPath);
    HRESULT					Read_AnimaionData(const string& strPath);

	HRESULT					Create_Texture();
	void					Create_AnimationTransform(uint32 index);

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
	/* �� ���� ����ϴ�, ��� �ִϸ��̼���, Bone Transform ���� ���� -> 3�� �迭 ����*/
	/* �� �ִϸ��̼��� Bone Transform�� AnimTransform�̴�. �̰� �ִϸ��̼� ������ŭ ���ͷ� ������ �ִ�.*/
	vector<AnimTransform>		m_AnimTransforms;
	ID3D11Texture2D*			m_pTexture = { nullptr };
	ID3D11ShaderResourceView*	m_pSrv = { nullptr };
	KeyframeDesc				m_keyframeDesc;

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const string& strPath, _fmatrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END