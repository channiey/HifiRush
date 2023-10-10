#pragma once
#include "Component.h"

BEGIN(Engine)

#define MAX_BONES 600 /* 셰이더 파일에서도 똑같이 정의 해줘야한다. */

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
	/* 현재 애니메이션이 제어해야할 뼈의 상태를 갱신한다. */
	HRESULT Update_Anim(_float fTimeDelta);
	/* 셰이더에 현재 모델의 매테리얼 정보를 바인딩한다. */
	HRESULT SetUp_OnShader(class CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* pConstantName);
	/* 스키닝 후, 셰이더에 정점을 전달한다. */
	HRESULT Render(class CShader* pShader, _uint iMeshIndex, _uint iPassIndex = 0);

public:
	class CBone*			Get_Bone(const char* pNodeName); 
	class CBone*			Get_Bone(const _int& iIndex);

	_uint					Get_NumMeshes() const { return (_uint)m_Meshes.size(); }
	vector<class CMesh*>*	Get_Meshes() { return &m_Meshes; }

	_uint					Get_MaterialIndex(_uint iMeshIndex);

	const _uint				Get_AnimationCount() const { return (_uint)m_Animations.size(); }
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

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const string& strPath, _fmatrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END