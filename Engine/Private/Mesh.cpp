#include "..\Public\Mesh.h"
#include "Model.h"
#include "Bone.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CMesh::CMesh(const CMesh& rhs)
	: CVIBuffer(rhs)
	, m_iMaterialIndex(rhs.m_iMaterialIndex)
	, m_BoneIndices(rhs.m_BoneIndices)
{
	strcpy_s(m_szName, rhs.m_szName);
}

HRESULT CMesh::Initialize_Prototype(string& strName, vector<VTXMODEL>& Vertices, vector<_int>& Indices, 
	_uint iMatIndex, vector<_int>& Bones, Matrix& PivotMatrix, CModel* pModel)
{
	/* �� �޽ÿ� �̸��� ���� ���� �����Ѵٸ�. �� ���� ����� �޽ø� �����ϴ� ������ ������ �� �ֵ��� �����Ѵ�. */
	strcpy_s(m_szName, strName.c_str());

	m_iMaterialIndex = iMatIndex;
	
	if (FAILED(Ready_StaticVertices(Vertices, PivotMatrix)))
		return E_FAIL;

	if (FAILED(Ready_Indices(Indices)))
		return E_FAIL;

	/* �� �޽ÿ��� ����ϴ� ���� �ε����� �����Ѵ�. */
	m_BoneIndices.reserve((_uint)Bones.size());
	for (_int index : Bones)
		m_BoneIndices.push_back(index);

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

	/* �� �޽ÿ��� ����ϴ� ���� �ε����� �����Ѵ�. */
	m_BoneIndices.reserve((_uint)Bones.size());
	for (_int index : Bones)
		m_BoneIndices.push_back(index);

	m_pModel = pModel;
	Safe_AddRef(m_pModel);
	
	return S_OK;
}

HRESULT CMesh::Initialize(void* pArg)
{
	/* ������Ÿ�Կ��� ������ �� �ε����� �������� �𵨿��� ���� �����´�. */
	if (nullptr != pArg)
	{
		CModel* pModel = static_cast<CModel*>(pArg);
		
		for (size_t i = 0; i < m_BoneIndices.size(); i++)
		{
			CBone* pBone = pModel->Get_Bone(m_BoneIndices[i]);
			if (nullptr != pBone)
			{
				m_Bones.push_back(pBone);
				Safe_AddRef(pBone);
			}
		}

		m_BoneIndices.clear();
		m_BoneIndices.shrink_to_fit();
	}
	return S_OK;
}

void CMesh::SetUp_BoneMatrices(_float4x4* pBoneMatrices, _fmatrix PivotMatrix)
{
	if (0 == m_Bones.size())
	{
		XMStoreFloat4x4(&pBoneMatrices[0], XMMatrixIdentity());
		return;
	}

	for (_uint i = 0; i < m_Bones.size(); ++i)
	{
		/* ���̴��� ��� ���� ���� ��ġ �� */
		/* ���� Ʈ������ ���*/

		XMStoreFloat4x4(&pBoneMatrices[i], XMMatrixTranspose(m_Bones[i]->Get_OffSetMatrix() * m_Bones[i]->Get_CombinedTransformation() * PivotMatrix));
	}
}

HRESULT CMesh::Ready_StaticVertices(vector<VTXMODEL>& Vertices, _fmatrix PivotMatrix)
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

	m_pVerticeStatic = new VTXMODEL[m_iNumVertices];
	ZeroMemory(m_pVerticeStatic, sizeof(VTXMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&m_pVerticeStatic[i].vPosition, &Vertices[i].vPosition, sizeof(_float3));
		XMStoreFloat3(&m_pVerticeStatic[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&m_pVerticeStatic[i].vPosition), PivotMatrix));

		memcpy(&m_pVerticeStatic[i].vNormal, &Vertices[i].vNormal, sizeof(_float3));
		XMStoreFloat3(&m_pVerticeStatic[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&m_pVerticeStatic[i].vNormal), PivotMatrix));

		memcpy(&m_pVerticeStatic[i].vTexture, &Vertices[i].vTexture, sizeof(_float2));

		memcpy(&m_pVerticeStatic[i].vTangent, &Vertices[i].vTangent, sizeof(_float3));
		XMStoreFloat3(&m_pVerticeStatic[i].vTangent, XMVector3TransformNormal(XMLoadFloat3(&m_pVerticeStatic[i].vTangent), PivotMatrix));
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = m_pVerticeStatic;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

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

	m_pVerticesAnim = new VTXANIMMODEL[m_iNumVertices];
	ZeroMemory(m_pVerticesAnim, sizeof(VTXANIMMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		/* NoneAnim �𵨰� �޸� ������ȯ�� �������� �ʴ´�. */
		memcpy(&m_pVerticesAnim[i].vPosition,	&Vertices[i].vPosition, sizeof(_float3));
		memcpy(&m_pVerticesAnim[i].vNormal, &Vertices[i].vNormal, sizeof(_float3));
		memcpy(&m_pVerticesAnim[i].vTexture, &Vertices[i].vTexture, sizeof(_float2));
		memcpy(&m_pVerticesAnim[i].vTangent, &Vertices[i].vTangent, sizeof(_float3));
		memcpy(&m_pVerticesAnim[i].vBlendIndex, &Vertices[i].vBlendIndex, sizeof(XMUINT4));
		memcpy(&m_pVerticesAnim[i].vBlendWeight, &Vertices[i].vBlendWeight, sizeof(Vec4));
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = m_pVerticesAnim;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

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

	m_pIndices = new FACEINDICES32[m_iNumPrimitives];
	ZeroMemory(m_pIndices, sizeof(FACEINDICES32) * m_iNumPrimitives);

	for (_uint i = 0, j = 0; i < m_iNumPrimitives; ++i, ++j)
	{
		m_pIndices[i]._0 = Indices[j];
		m_pIndices[i]._1 = Indices[++j];
		m_pIndices[i]._2 = Indices[++j];
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = m_pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	return S_OK;
}

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

	/* Bones */
	for (auto& pBone : m_Bones)
		Safe_Release(pBone);

	m_Bones.clear();

	/* Vertices, Indices */
	if(nullptr != m_pVerticesAnim)
		Safe_Delete_Array(m_pVerticesAnim);

	if(nullptr != m_pVerticeStatic)
		Safe_Delete_Array(m_pVerticeStatic);

	Safe_Delete_Array(m_pIndices);

	/* Model */
	Safe_Release(m_pModel);
}
