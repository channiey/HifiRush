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
	virtual HRESULT Initialize_Prototype(const string& strPath, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg);

private:
    HRESULT Read_BoneData(const string& strPath);
    HRESULT Read_MeshData(const string& strPath, Matrix PivotMatrix);
    HRESULT Read_MaterialData(const string& strPath);
    HRESULT Read_AnimaionData(const string& strPath);

public:
	/* ���� �ִϸ��̼��� �����ؾ��� ���� ���¸� �����Ѵ�. */
	HRESULT Update_Anim(_float fTimeDelta);

public:
	class CBone* Get_Bone(const char* pNodeName); 
	class CBone* Get_Bone(const _int& iIndex);
	_uint Get_NumMeshes() const { return m_iNumMeshes; }
	_uint Get_MaterialIndex(_uint iMeshIndex);
	_matrix Get_PivotMatrix() { return XMLoadFloat4x4(&m_PivotMatrix); }
	const _uint Get_AnimationCount() const { return m_iNumAnimations; }
	const _uint Get_CurAnimationIndex() const { return m_iCurrentAnimIndex; }

public:
	/* ���� ����� �ִϸ��̼� �ε����� �����Ѵ�.*/
	void Set_Animation(_uint iAnimIndex) { m_iCurrentAnimIndex = iAnimIndex; }

public: /* Render */
	/* ���̴��� ���� ���� ���׸��� ������ ���ε� �Ѵ�. */
	HRESULT SetUp_OnShader(class CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* pConstantName);
	/* ��Ű�� ��, ���̴��� ������ �����Ѵ�. */
	HRESULT Render(class CShader* pShader, _uint iMeshIndex, _uint iPassIndex = 0);

private: 
	_float4x4								m_PivotMatrix{};
	TYPE									m_eModelType = TYPE_END;

	/* aiNode */
	vector<class CBone*>					m_Bones;

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

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const string& strPath, _fmatrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END