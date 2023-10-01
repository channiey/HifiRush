#pragma once

/* ���� �����ϴ� �ϳ��� �޽��� ������ ��Ÿ����. */
/* �� �޽ø� �����ϴ� ����, �ε��� ���۸� �����Ѵ�. */
/* �� �޽ÿ� ������ �ִ� ���� ������, �޽ø� �׸��� ���� ���¸��� ������ �����Ѵ�. */

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
	HRESULT Initialize_Prototype(string& strName, vector<VTXMODEL>& Vertices, vector<_int>& Indices, _uint iMatIndex, vector<_int>& Bones, Matrix& PivotMatrix, CModel* pModel);
	HRESULT Initialize_Prototype(string& strName, vector<VTXANIMMODEL>& Vertices, vector<_int>& Indices, _uint iMatIndex, vector<_int>& Bones, CModel* pModel);

	//HRESULT Initialize_Prototype(CModel::TYPE eModelType, const aiMesh* pAIMesh, class CModel* pModel, _fmatrix PivotMatrix);
	virtual HRESULT Initialize(void* pArg);	

public:
	//HRESULT SetUp_HierarchyNodes(class CModel* pModel, aiMesh* pAIMesh); 	/* ���� �޽ÿ� ������ �ִ� ������ ��ȸ�ϸ� ��������� �����ϰ�, ������ �����̳ʿ� ��Ƶд�. */
	void SetUp_BoneMatrices(_float4x4* pBoneMatrices, _fmatrix PivotMatrix); /* �޽��� ������ �׸������� ���̴��� �ѱ������ ������� �迭�� �����Ѵ�. */

public:
	_uint Get_MaterialIndex() const { return m_iMaterialIndex; }

private:
	char				m_szName[MAX_PATH] = "";
	_uint				m_iMaterialIndex = 0; /* �޽ø� �׸� �� ����ϴ� ���¸��� �ε��� */

private:
	_uint							m_iNumBones = 0; /* �� �޽ÿ� ������ �ִ� ���� ����*/
	vector<class CHierarchyNode*>	m_Bones; /* �� �޽ÿ� ������ �ִ� �� ���� (�޽ú��� ��������, �ش� �޽ÿ� ������ �ִ� �� ����� ��Ƽ� �佺 */

	vector<_int> m_BoneIndex; /* Cache */

private:
	HRESULT Ready_Vertices(vector<VTXMODEL>& Vertices, _fmatrix PivotMatrix);
	HRESULT Ready_AnimVertices(vector<VTXANIMMODEL>& Vertices);
	HRESULT Ready_Indices(vector<_int>& Indices);

	//HRESULT Ready_Vertices(const aiMesh* pAIMesh, _fmatrix PivotMatrix);
	//HRESULT Ready_AnimVertices(const aiMesh* pAIMesh, CModel* pModel);

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, string& strName, vector<VTXMODEL>& Vertices, vector<_int>& Indices, _uint iMatIndex, vector<_int>& Bones, Matrix& PivotMatrix, CModel* pModel);
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, string& strName, vector<VTXANIMMODEL>& Vertices, vector<_int>& Indices, _uint iMatIndex, vector<_int>& Bones, CModel* pModel);
	
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END