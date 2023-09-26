#pragma once

#include "Component.h"

BEGIN(Engine)

#define MAX_BONES 600 /* ���̴� ���Ͽ����� �Ȱ��� ���� ������Ѵ�. */

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
	class CHierarchyNode* Get_HierarchyNode(const char* pNodeName); /* �̸����� Ư�� ��带 �޾ƿ´�. */
	_uint Get_NumMeshes() const { return m_iNumMeshes; }
	_uint Get_MaterialIndex(_uint iMeshIndex);
	_matrix Get_PivotMatrix() { return XMLoadFloat4x4(&m_PivotMatrix); }
	const _uint Get_AnimationCount() const { return m_iNumAnimations; }
	const _uint Get_CurAnimationIndex() const { return m_iCurrentAnimIndex; }

public:
	/* ���� ����� �ִϸ��̼� �ε����� �����Ѵ�.*/
	void Set_AnimIndex(_uint iAnimIndex) { m_iCurrentAnimIndex = iAnimIndex; }

public:
	/* ���� �ִϸ��̼��� �����ؾ��� ���� ���¸� �����Ѵ�. */
	HRESULT Play_Animation(_float fTimeDelta);

public: /* Render */
	/* ���̴��� ���� ���� ���׸��� ������ ���ε� �Ѵ�. */
	HRESULT SetUp_OnShader(class CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* pConstantName);
	/* ��Ű�� ��, ���̴��� ������ �����Ѵ�. */
	HRESULT Render(class CShader* pShader, _uint iMeshIndex, _uint iPassIndex = 0);

private: /* ������ ���´�. */
	const aiScene*				m_pAIScene = nullptr;
	Assimp::Importer			m_Importer;
	_float4x4					m_PivotMatrix;
	TYPE						m_eModelType = TYPE_END;

private: /* Ŭ���� ���´�. */
	/* aiNode */
	vector<class CHierarchyNode*>			m_HierarchyNodes;

	/* Mesh*/
	_uint									m_iNumMeshes = 0;
	vector<class CMesh*>					m_Meshes;
	typedef vector<class CMesh*>			MESHES;

	/* Mat*/
	_uint									m_iNumMaterials = 0;
	vector<MATERIALDESC>					m_Materials;

	/* Anim */
	_uint									m_iCurrentAnimIndex = 0;
	_uint									m_iNumAnimations = 0;
	vector<class CAnimation*>				m_Animations;

private:
	/* Assimp�� ���� fbx�� �ε��Ͽ� ī�װ����� ���� �Ľ��Ѵ�.*/
	HRESULT Ready_Meshes(_fmatrix PivotMatrix);
	HRESULT Ready_Materials(const char* pModelFilePath);
	HRESULT Ready_Animations();
	HRESULT Ready_HierarchyNodes(aiNode* pNode, class CHierarchyNode* pParent, _uint iDepth);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const char* pModelFilePath, const char* pModelFileName, _fmatrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END