#pragma once

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
	virtual HRESULT Initialize(void* pArg);	

public:
	_uint					Get_MaterialIndex() const { return m_iMaterialIndex; }
	FACEINDICES32*			Get_Indices() const { return m_pIndices; }
	vector<class CBone*>&	Get_Bones() { return m_Bones; }
public:
	void					SetUp_BoneMatrices(_float4x4* pBoneMatrices, _fmatrix PivotMatrix); /* �޽��� ������ �׸������� ���̴��� �ѱ������ ������� �迭�� �����Ѵ�. */

private:
	char					m_szName[MAX_PATH] = "";
	_uint					m_iMaterialIndex = 0;	
	FACEINDICES32*			m_pIndices = nullptr;

private:
	vector<class CBone*>	m_Bones;			/* �� �޽ÿ� ������ �ִ� �� ���� (�޽ú��� ��������, �ش� �޽ÿ� ������ �ִ� �� ����� ��Ƽ� �佺 */

	class CModel*			m_pModel = { nullptr };

	/* Cache */
	vector<_int>			m_BoneIndices;		/* Cache (Initialize���� ���� ã�ƿ��� ���� �ε�����) */

private:
	HRESULT Ready_StaticVertices(vector<VTXMODEL>& Vertices, _fmatrix PivotMatrix);
	HRESULT Ready_AnimVertices(vector<VTXANIMMODEL>& Vertices);
	HRESULT Ready_Indices(vector<_int>& Indices);

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, string& strName, vector<VTXMODEL>& Vertices, vector<_int>& Indices, _uint iMatIndex, vector<_int>& Bones, Matrix& PivotMatrix, CModel* pModel);
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, string& strName, vector<VTXANIMMODEL>& Vertices, vector<_int>& Indices, _uint iMatIndex, vector<_int>& Bones, CModel* pModel);
	
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END