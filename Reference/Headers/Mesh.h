#pragma once

/* 모델을 구성하는 하나의 메시의 정보를 나타낸다. */
/* 이 메시를 구성하는 정점, 인덱스 버퍼를 보관한다. */
/* 이 메시에 영향을 주는 뼈의 정보와, 메시를 그리기 위한 매태리얼 정보를 보관한다. */

#include "VIBuffer.h"
#include "Model.h"

BEGIN(Engine)

class CMesh final : public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& rhs);
	virtual ~CMesh() = default;

public:
	virtual HRESULT Initialize_Prototype(CModel::TYPE eModelType, const aiMesh* pAIMesh, class CModel* pModel, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg);	

public:
	HRESULT SetUp_HierarchyNodes(class CModel* pModel, aiMesh* pAIMesh); 	/* 현재 메시에 영향을 주는 뼈들을 순회하며 행렬정보를 저장하고, 뼈들을 컨테이너에 모아둔다. */
	void SetUp_BoneMatrices(_float4x4* pBoneMatrices, _fmatrix PivotMatrix); /* 메시의 정점을 그리기위해 셰이더에 넘기기위한 뼈행렬의 배열을 구성한다. */

public:
	_uint Get_MaterialIndex() const { return m_iMaterialIndex; }

private:
	char				m_szName[MAX_PATH] = "";
	_uint				m_iMaterialIndex = 0; /* 메시를 그릴 때 사용하는 매태리얼 인덱스 */

private:
	_uint							m_iNumBones = 0; /* 이 메시에 영향을 주는 뼈의 갯수*/
	vector<class CHierarchyNode*>	m_Bones; /* 이 메시에 영향을 주는 뼈 집합 (메시별로 렌더링시, 해당 메시에 영향을 주는 뼈 행렬을 모아서 토스 */

private:
	HRESULT Ready_Vertices(const aiMesh* pAIMesh, _fmatrix PivotMatrix);
	HRESULT Ready_AnimVertices(const aiMesh* pAIMesh, CModel* pModel);

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, const aiMesh* pAIMesh, class CModel* pModel, _fmatrix PivotMatrix);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END