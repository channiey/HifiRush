#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	virtual HRESULT Initialize_Prototype(TYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg);

public:
	class CHierarchyNode* Get_HierarchyNode(const char* pNodeName);
	_uint Get_NumMeshes() const { return m_iNumMeshes; }
	_uint Get_MaterialIndex(_uint iMeshIndex);
	_matrix Get_PivotMatrix() { return XMLoadFloat4x4(&m_PivotMatrix); }

public:
	/* 현재 재생할 애니메이션 인덱스를 설정한다.*/
	void Set_AnimIndex(_uint iAnimIndex) { m_iCurrentAnimIndex = iAnimIndex; }

	/* 현재 애니메이션이 제어해야할 뼈의 상태를 갱신한다. */
	HRESULT Play_Animation(_float fTimeDelta);

public: /* Render */
	/* 셰이더에 현재 모델의 매테리얼 정보를 바인딩 한다. */
	HRESULT SetUp_OnShader(class CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* pConstantName);
	/* 스키닝 후, 셰이더에 정점을 전달한다. */
	HRESULT Render(class CShader* pShader, _uint iMeshIndex, _uint iPassIndex = 0);

private: 
	const aiScene*				m_pAIScene = nullptr;
	Assimp::Importer			m_Importer;
	_float4x4					m_PivotMatrix;
	TYPE						m_eModelType = TYPE_END;

	/* Mesh*/
	_uint									m_iNumMeshes = 0;
	vector<class CMeshContainer*>			m_Meshes;
	typedef vector<class CMeshContainer*>	MESHES;

	/* Mat*/
	_uint									m_iNumMaterials = 0;
	vector<MATERIALDESC>					m_Materials;

	/* Anim */
	_uint									m_iCurrentAnimIndex = 0;
	_uint									m_iNumAnimations = 0;
	vector<class CAnimation*>				m_Animations;

	/* aiNode */
	vector<class CHierarchyNode*>			m_HierarchyNodes;

private:
	/* Assimp를 통해 fbx를 로드하여 카테고리별로 나눠 파싱한다.*/
	HRESULT Ready_MeshContainers(_fmatrix PivotMatrix);
	HRESULT Ready_Materials(const char* pModelFilePath);
	HRESULT Ready_Animations();
	HRESULT Ready_HierarchyNodes(aiNode* pNode, class CHierarchyNode* pParent, _uint iDepth);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END