#pragma once
#include "Component.h"

BEGIN(Engine)

#define MAX_BONES 600 /* ���̴� ���Ͽ����� �Ȱ��� ���� ������Ѵ�. */
// Bone
#define MAX_MODEL_TRANSFORMS 700 /* �� �ִϸ��̼��� ���� �� �ִ� �ִ� �� ���� */
#define MAX_MODEL_KEYFRAMES 300 /* �� �ִϸ��̼��� ���� �� �ִ� �ִ� ������ �� */

struct AnimTransform /* �ϳ��� �ִϸ��̼��� ��� Bone Transform ������ 2���� ���·� ���� */
{
	// [ ][ ][ ][ ][ ][ ][ ] ... w
	using TransformArrayType = array<Matrix, MAX_MODEL_TRANSFORMS>;

	// [ ][ ][ ][ ][ ][ ][ ] ... 
	array<TransformArrayType, MAX_MODEL_KEYFRAMES> transforms;

	/* Ű������ ����ŭ transforms�� ���� �ְ�, */
	/* transforms �ȿ��� �� ���� ��ŭ Matrix�� �ִ�. */

	/* 2�� �迭 ���·� ����Ѵ�. */
};

/* ���� ������ �ִ� ��� �ִϸ��̼� �� ���� ����ǰ� �ִ� �ִϸ��̼� ���¸� ��Ÿ�� */
struct KeyframeDesc
{
	/* ���� , ���� , ���̵� , ���� , �� , ��ũ */
	_int	animIndex = 2;	/* ��� �ִϸ��̼� ��, ���� �ִϸ��̼� �ε���*/
	_uint	currFrame = 0;
	_uint	nextFrame = 0;
	_float	ratio = 0.f;
	_float	sumTime = 0.f;	/* ������ ������ ���� acc */
	_float	speed = 1.f;	/* �ִϸ��̼� ��� ���ǵ� */
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
	HRESULT			Update_Anim(_float fTimeDelta);	/* ���� �ִϸ��̼��� �����ؾ��� ���� ���¸� �����Ѵ�. */
	HRESULT			Update_VTFAnim(_float fTimeDelta);
	HRESULT			Render(class CShader* pShader, _uint iMeshIndex, _uint iPassIndex = 0); /* ��Ű�� ��, ���̴��� ������ �����Ѵ�. */
	
	HRESULT			SetUp_OnShader(class CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* pConstantName); /* ���̴��� ���� ���� ���׸��� ������ ���ε��Ѵ�. */

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
	/* �� ���� ����ϴ�, ��� �ִϸ��̼���, Bone Transform ���� ���� -> 3�� �迭 ����*/
	/* �� �ִϸ��̼��� Bone Transform�� AnimTransform�̴�. �̰� �ִϸ��̼� ������ŭ ���ͷ� ������ �ִ�.*/
	ID3D11ShaderResourceView*	m_pSrv = { nullptr };
	KeyframeDesc				m_keyframeDesc = {};

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const string& strPath, _fmatrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END