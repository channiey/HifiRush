#include "..\Public\Mesh.h"
#include "Model.h"
#include "HierarchyNode.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CMesh::CMesh(const CMesh& rhs)
	: CVIBuffer(rhs)
	, m_iMaterialIndex(rhs.m_iMaterialIndex)
	, m_iNumBones(rhs.m_iNumBones)
	, m_Bones(rhs.m_Bones)
	, m_BoneIndex(rhs.m_BoneIndex)
{
	strcpy_s(m_szName, rhs.m_szName);
}

HRESULT CMesh::Initialize_Prototype(string& strName, vector<VTXMODEL>& Vertices, vector<_int>& Indices, 
	_uint iMatIndex, vector<_int>& Bones, Matrix& PivotMatrix, CModel* pModel)
{
	/* �� �޽ÿ� �̸��� ���� ���� �����Ѵٸ�. �� ���� ����� �޽ø� �����ϴ� ������ ������ �� �ֵ��� �����Ѵ�. */
	strcpy_s(m_szName, strName.c_str());

	m_iMaterialIndex = iMatIndex;
	
	if (FAILED(Ready_Vertices(Vertices, PivotMatrix)))
		return E_FAIL;

	if (FAILED(Ready_Indices(Indices)))
		return E_FAIL;

	m_iNumBones = (_uint)Bones.size();
	m_BoneIndex.reserve(m_iNumBones);
	for (size_t i = 0; i < m_iNumBones; i++)
		m_BoneIndex.push_back(Bones[i]);

	/*m_iNumBones = (_uint)Bones.size();
	for (size_t i = 0; i < Bones.size(); i++)
		m_Bones.push_back(Bones[i]);

	if (0 == m_iNumBones)
	{
		CHierarchyNode* pNode = pModel->Get_HierarchyNode(m_szName);

		if (nullptr == pNode)
			return S_OK;

		m_iNumBones = 1;

		m_Bones.push_back(pNode);
		Safe_AddRef(pNode);
	}*/

	return S_OK;
}

HRESULT CMesh::Initialize_Prototype(string& strName, vector<VTXANIMMODEL>& Vertices, vector<_int>& Indices, 
	_uint iMatIndex, vector<_int>& Bones, CModel* pModel)
{
	strcpy_s(m_szName, strName.c_str());

	m_iMaterialIndex = iMatIndex;

	if (FAILED(Ready_AnimVertices(Vertices)))
		return E_FAIL;

	if (FAILED(Ready_Indices(Indices)))
		return E_FAIL;

	m_iNumBones = (_uint)Bones.size();
	m_BoneIndex.reserve(m_iNumBones);
	for (size_t i = 0; i < m_iNumBones; i++)
		m_BoneIndex.push_back(Bones[i]);
	

	/*m_iNumBones = (_uint)Bones.size();
	for (size_t i = 0; i < Bones.size(); i++)
		m_Bones.push_back(Bones[i]);

	if (0 == m_iNumBones)
	{
		CHierarchyNode* pNode = pModel->Get_HierarchyNode(m_szName);

		if (nullptr == pNode)
			return S_OK;

		m_iNumBones = 1;

		m_Bones.push_back(pNode);
		Safe_AddRef(pNode);
	}*/

	return S_OK;
}

//HRESULT CMesh::Initialize_Prototype(CModel::TYPE eModelType, const aiMesh* pAIMesh, CModel* pModel, _fmatrix PivotMatrix)
//{
//	/* �� �޽ÿ� �̸��� ���� ���밡 �����Ѵٸ�.
//	�� ������ ����� �޽ø� �����ϴ� ������ ������ �� �ֵ��� �����ϳ�. */
//	strcpy_s(m_szName, pAIMesh->mName.data);
//
//	/* �޽ø��� ����ϴ� ���׸���(�ؽ��������� ǥ��)�� �ٸ���. */
//	/* �޽ø� �׸��� ���� � ���׸����� �ѵ�÷� �������� ������ �������ֱ����ؼ�. */
//	m_iMaterialIndex = pAIMesh->mMaterialIndex;
//
//#pragma region VERTEXBUFFER
//
//	HRESULT		hr = 0;
//
//	if (CModel::TYPE_NONANIM == eModelType)
//		hr = Ready_Vertices(pAIMesh, PivotMatrix);
//	else
//		hr = Ready_AnimVertices(pAIMesh, pModel);
//
//	if (FAILED(hr))
//		return E_FAIL;
//
//#pragma endregion
//
//#pragma region INDEXBUFFER
//	m_iNumPrimitives = pAIMesh->mNumFaces;
//	m_iIndexSizeofPrimitive = sizeof(FACEINDICES32);
//	m_iNumIndicesofPrimitive = 3;
//
//	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
//	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
//
//	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
//	m_BufferDesc.ByteWidth = m_iNumPrimitives * m_iIndexSizeofPrimitive;
//	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	m_BufferDesc.CPUAccessFlags = 0;
//	m_BufferDesc.MiscFlags = 0;
//	m_BufferDesc.StructureByteStride = 0;
//
//
//	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitives];
//	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitives);
//
//	for (_uint i = 0; i < m_iNumPrimitives; ++i)
//	{
//		pIndices[i]._0 = pAIMesh->mFaces[i].mIndices[0];
//		pIndices[i]._1 = pAIMesh->mFaces[i].mIndices[1];
//		pIndices[i]._2 = pAIMesh->mFaces[i].mIndices[2];
//	}
//
//
//	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
//	m_SubResourceData.pSysMem = pIndices;
//
//	if (FAILED(__super::Create_IndexBuffer()))
//		return E_FAIL;
//
//	Safe_Delete_Array(pIndices);
//
//#pragma endregion
//
//	return S_OK;
//}

HRESULT CMesh::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		CModel* pModel = static_cast<CModel*>(pArg);
		
		for (size_t i = 0; i < m_BoneIndex.size(); i++)
		{
			CHierarchyNode* pNode = pModel->Get_HierarchyNode(m_BoneIndex[i]);
			if (nullptr != pNode)
			{
				m_Bones.push_back(pNode);
				Safe_AddRef(pNode);
			}
		}
	}

	m_BoneIndex.clear();
	m_BoneIndex.shrink_to_fit();

	return S_OK;
}

//HRESULT CMesh::SetUp_HierarchyNodes(CModel* pModel, aiMesh* pAIMesh)
//{
//	m_iNumBones = pAIMesh->mNumBones;
//
//	/* ���� �޽ÿ� ������ �ִ� ������ ��ȸ�ϸ� ��������� �����ϰ�, ������ �����̳ʿ� ��Ƶд�. */
//
//	for (_uint i = 0; i < m_iNumBones; ++i)
//	{
//		aiBone* pAIBone = pAIMesh->mBones[i];
//
//		CHierarchyNode* pHierarchyNode = pModel->Get_HierarchyNode(pAIBone->mName.data);
//
//		_float4x4			OffsetMatrix;
//
//		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));
//
//		pHierarchyNode->Set_OffsetMatrix(XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));
//
//		m_Bones.push_back(pHierarchyNode);
//		Safe_AddRef(pHierarchyNode);
//	}
//
//	if (0 == m_iNumBones)
//	{
//		CHierarchyNode* pNode = pModel->Get_HierarchyNode(m_szName);
//
//		if (nullptr == pNode)
//			return S_OK;
//
//		m_iNumBones = 1;
//
//		m_Bones.push_back(pNode);
//		Safe_AddRef(pNode);
//	}
//
//	return S_OK;
//}

void CMesh::SetUp_BoneMatrices(_float4x4* pBoneMatrices, _fmatrix PivotMatrix)
{
	if (0 == m_iNumBones)
	{
		XMStoreFloat4x4(&pBoneMatrices[0], XMMatrixIdentity());
		return;
	}

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		/* ���̴��� ��� ���� ���� ��ġ �� */
		/* ���� Ʈ������ ���*/

		/* ���� Ʈ������ �߸��ư� , �޹���, ������ ��Ʈ���� ���� �׵� ������ */
		XMStoreFloat4x4(&pBoneMatrices[i], XMMatrixTranspose(m_Bones[i]->Get_OffSetMatrix() * m_Bones[i]->Get_CombinedTransformation() * PivotMatrix));
	}
}

HRESULT CMesh::Ready_Vertices(vector<VTXMODEL>& Vertices, _fmatrix PivotMatrix)
{
	m_iNumVertexBuffers = 1;
	m_iNumVertices = (_int)Vertices.size();
	m_iStride = sizeof(VTXMODEL);

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXMODEL* pVertices = new VTXMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &Vertices[i].vPosition, sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));

		memcpy(&pVertices[i].vNormal, &Vertices[i].vNormal, sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PivotMatrix));

		memcpy(&pVertices[i].vTexture, &Vertices[i].vTexture, sizeof(_float2));

		memcpy(&pVertices[i].vTangent, &Vertices[i].vTangent, sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vTangent, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vTangent), PivotMatrix));
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_AnimVertices(vector<VTXANIMMODEL>& Vertices)
{
	/* �⺻ ���� ���� ���� */
	m_iNumVertexBuffers = 1;
	m_iNumVertices = (_int)Vertices.size();
	m_iStride = sizeof(VTXANIMMODEL);

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXANIMMODEL* pVertices = new VTXANIMMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		/* NoneAnim �𵨰� �޸� ������ȯ�� �������� �ʴ´�. */
		memcpy(&pVertices[i].vPosition,	&Vertices[i].vPosition, sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &Vertices[i].vNormal, sizeof(_float3));
		memcpy(&pVertices[i].vTexture, &Vertices[i].vTexture, sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &Vertices[i].vTangent, sizeof(_float3));
		memcpy(&pVertices[i].vBlendIndex, &Vertices[i].vBlendIndex, sizeof(XMUINT4));
		memcpy(&pVertices[i].vBlendWeight, &Vertices[i].vBlendWeight, sizeof(Vec4));
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_Indices(vector<_int>& Indices)
{
	m_iNumPrimitives = ((_int)Indices.size()) / 3;
	m_iIndexSizeofPrimitive = sizeof(FACEINDICES32);
	m_iNumIndicesofPrimitive = 3;

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumPrimitives * m_iIndexSizeofPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitives];
	ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitives);

	for (_uint i = 0, j = 0; i < m_iNumPrimitives; ++i, ++j)
	{
		pIndices[i]._0 = Indices[j];
		pIndices[i]._1 = Indices[++j];
		pIndices[i]._2 = Indices[++j];
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

	return S_OK;
}

//HRESULT CMesh::Ready_Vertices(const aiMesh* pAIMesh, _fmatrix PivotMatrix)
//{
//	m_iNumVertexBuffers = 1;
//	m_iNumVertices = pAIMesh->mNumVertices;
//	m_iStride = sizeof(VTXMODEL);
//
//	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
//	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
//	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	m_BufferDesc.CPUAccessFlags = 0;
//	m_BufferDesc.MiscFlags = 0;
//	m_BufferDesc.StructureByteStride = m_iStride;
//
//	VTXMODEL* pVertices = new VTXMODEL[m_iNumVertices];
//	ZeroMemory(pVertices, sizeof(VTXMODEL) * m_iNumVertices);
//
//	for (_uint i = 0; i < m_iNumVertices; ++i)
//	{
//		/* ������ ��ġ�� ���� ���ϴ� ���ʱ���·� ��ȭ������./ */
//		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
//		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));
//
//		/* ������ ��ġ�� �ٲ���⶧������ ��ֵ� �ٲ��. */
//		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
//		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PivotMatrix));
//
//		memcpy(&pVertices[i].vTexture, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
//
//		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
//		XMStoreFloat3(&pVertices[i].vTangent, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vTangent), PivotMatrix));
//	}
//
//	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
//	m_SubResourceData.pSysMem = pVertices;
//
//	if (FAILED(__super::Create_VertexBuffer()))
//		return E_FAIL;
//
//	Safe_Delete_Array(pVertices);
//
//	return S_OK;
//}

//HRESULT CMesh::Ready_AnimVertices(const aiMesh* pAIMesh, CModel* pModel)
//{
//	/* �⺻ ���� ���� ���� */
//	m_iNumVertexBuffers = 1;
//	m_iNumVertices = pAIMesh->mNumVertices;
//	m_iStride = sizeof(VTXANIMMODEL);
//
//	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
//	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
//	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	m_BufferDesc.CPUAccessFlags = 0;
//	m_BufferDesc.MiscFlags = 0;
//	m_BufferDesc.StructureByteStride = m_iStride;
//
//	VTXANIMMODEL* pVertices = new VTXANIMMODEL[m_iNumVertices];
//	ZeroMemory(pVertices, sizeof(VTXANIMMODEL) * m_iNumVertices);
//
//	/* ���� ����ü ä��� 1 */
//	for (_uint i = 0; i < m_iNumVertices; ++i)
//	{
//		/* NoneAnim �𵨰� �޸� ������ȯ�� �������� �ʴ´�. */
//		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
//		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
//		memcpy(&pVertices[i].vTexture, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
//		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
//	}
//
//	/* ���� ����ü ä��� 2 (�� �޽ÿ� ������ �ִ� ���� �ε����� ����ġ) */
//	for (_uint i = 0; i < pAIMesh->mNumBones; ++i)
//	{
//		aiBone* pAIBone = pAIMesh->mBones[i];
//
//		for (_uint j = 0; j < pAIBone->mNumWeights; ++j)
//		{
//			_uint		iVertexIndex = pAIBone->mWeights[j].mVertexId;
//
//			if (0.0f == pVertices[iVertexIndex].vBlendWeight.x)
//			{
//				pVertices[iVertexIndex].vBlendIndex.x = i;
//				pVertices[iVertexIndex].vBlendWeight.x = pAIBone->mWeights[j].mWeight;
//			}
//
//			else if (0.0f == pVertices[iVertexIndex].vBlendWeight.y)
//			{
//				pVertices[iVertexIndex].vBlendIndex.y = i;
//				pVertices[iVertexIndex].vBlendWeight.y = pAIBone->mWeights[j].mWeight;
//			}
//
//			else if (0.0f == pVertices[iVertexIndex].vBlendWeight.z)
//			{
//				pVertices[iVertexIndex].vBlendIndex.z = i;
//				pVertices[iVertexIndex].vBlendWeight.z = pAIBone->mWeights[j].mWeight;
//			}
//
//			else if (0.0f == pVertices[iVertexIndex].vBlendWeight.w)
//			{
//				pVertices[iVertexIndex].vBlendIndex.w = i;
//				pVertices[iVertexIndex].vBlendWeight.w = pAIBone->mWeights[j].mWeight;
//			}
//		}
//	}
//
//	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
//	m_SubResourceData.pSysMem = pVertices;
//
//	if (FAILED(__super::Create_VertexBuffer()))
//		return E_FAIL;
//
//	Safe_Delete_Array(pVertices);
//
//	return S_OK;
//}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, string& strName,
	vector<VTXMODEL>& Vertices, vector<_int>& Indices, _uint iMatIndex, vector<_int>& Bones, Matrix& PivotMatrix, CModel* pModel)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(strName, Vertices, Indices, iMatIndex, Bones, PivotMatrix, pModel)))
	{
		MSG_BOX("Failed To Created : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, string& strName,
	vector<VTXANIMMODEL>& Vertices, vector<_int>& Indices, _uint iMatIndex, vector<_int>& Bones, CModel* pModel)
{
	
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(strName, Vertices, Indices, iMatIndex, Bones, pModel)))
	{
		MSG_BOX("Failed To Created : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

//CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, const aiMesh* pAIMesh, CModel* pModel, _fmatrix PivotMatrix)
//{
//	CMesh* pInstance = new CMesh(pDevice, pContext);
//
//	if (FAILED(pInstance->Initialize_Prototype(eModelType, pAIMesh, pModel, PivotMatrix)))
//	{
//		MSG_BOX("Failed To Created : CMesh");
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}

CComponent* CMesh::Clone(void* pArg)
{
	CMesh* pInstance = new CMesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMesh::Free()
{
	__super::Free();

	for (auto& pHierarchyNode : m_Bones)
		Safe_Release(pHierarchyNode);

	m_Bones.clear();
}
