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

private:
    HRESULT Read_BoneData(const string& strPath);
    HRESULT Read_MeshData(const string& strPath, Matrix PivotMatrix);
    HRESULT Read_MaterialData(const string& strPath);
    HRESULT Read_AnimaionData(const string& strPath);

public:
	/* 현재 애니메이션이 제어해야할 뼈의 상태를 갱신한다. */
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
	/* 현재 재생할 애니메이션 인덱스를 설정한다.*/
	void Set_Animation(_uint iAnimIndex) { m_iCurrentAnimIndex = iAnimIndex; }

public: /* Render */
	/* 셰이더에 현재 모델의 매테리얼 정보를 바인딩 한다. */
	HRESULT SetUp_OnShader(class CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* pConstantName);
	/* 스키닝 후, 셰이더에 정점을 전달한다. */
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