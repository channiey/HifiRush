#include "..\Public\Model.h"
#include "Mesh.h"
#include "Texture.h"
#include "HierarchyNode.h"
#include "Animation.h"
#include "Channel.h"
#include "Shader.h"

#include "Util_File.h"
#include "Util_String.h"


CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, m_iNumMeshes(rhs.m_iNumMeshes)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_Meshes(rhs.m_Meshes)
	, m_Materials(rhs.m_Materials)
	, m_eModelType(rhs.m_eModelType)
	/*, m_HierarchyNodes(rhs.m_HierarchyNodes)*/
	, m_Animations(rhs.m_Animations)
	, m_iCurrentAnimIndex(rhs.m_iCurrentAnimIndex)
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_iNumAnimations(rhs.m_iNumAnimations)
	
{
	/* Meshes */
	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);

	/* Materials */
	for (auto& Material : m_Materials)
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_AddRef(Material.pTexture[i]);

	/* Animations */
	for (auto& pAnimation : m_Animations)
		Safe_AddRef(pAnimation);
}


HRESULT CModel::Initialize_Prototype(const string& strPath, _fmatrix PivotMatrix)
{
	// "../Bin/Resources/Models/Characters/Chai/"

	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

	if (FAILED(Read_BoneData(strPath)))
		return E_FAIL;

	if (FAILED(Read_MeshData(strPath, PivotMatrix)))
		return E_FAIL;

	if (FAILED(Read_MaterialData(strPath)))
		return E_FAIL;

	if (FAILED(Read_AnimaionData(strPath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
	/* �� ���� ��ȸ ���� */
	//Ready_HierarchyNodes(m_pAIScene->mRootNode, nullptr, 0);

	/* �� ���� ���� */


	if (TYPE_ANIM == m_eModelType)
	{
		_uint			iNumMeshes = 0;
		vector<CMesh*>	Meshes;

		/* �޽� ���� ���� */
		for (auto& pPrototype : m_Meshes)
		{
			CMesh* pMeshContainer = (CMesh*)pPrototype->Clone();
			if (nullptr == pMeshContainer)
				return E_FAIL;

			Meshes.push_back(pMeshContainer);

			Safe_Release(pPrototype);
		}
		m_Meshes.clear();
		m_Meshes = Meshes;


		///* �޽ø� ��ȸ�ϸ�, �޽ÿ� ���� �� ������ ��忡 �����Ѵ�. */
		//for (auto& pMeshContainer : m_Meshes)
		//{
		//	if (nullptr != pMeshContainer)
		//		pMeshContainer->SetUp_HierarchyNodes(this, m_pAIScene->mMeshes[iNumMeshes++]);
		//}
	}

	/* �ִϸ��̼� ���� ���� */
	vector<CAnimation*>		Animations;
	for (auto& pPrototype : m_Animations)
	{
		CAnimation* pAnimation = pPrototype->Clone(this);
		if (nullptr == pAnimation)
			return E_FAIL;

		Animations.push_back(pAnimation);

		Safe_Release(pPrototype);
	}
	m_Animations.clear();
	m_Animations = Animations;



	return S_OK;
}

HRESULT CModel::Read_BoneData(const string& strPath)
{
	string filePath = strPath + filesystem::path(strPath).filename().string() + ".bone";
	if (!Util_File::IsExistFile(filePath))
		return E_FAIL;

	shared_ptr<Util_File> file = make_shared<Util_File>();
	file->Open(Util_String::ToWString(filePath), FileMode::Read);

	/* 1�� ���� �Ľ� (�θ�� �ε����θ� �˰� �ִ�) */
	size_t iSize = file->Read<size_t>();
	for (size_t i = 0; i < iSize; i++)
	{
		CHierarchyNode* pHierarchyNode = CHierarchyNode::Create(file->Read<string>(), file->Read<Matrix>(), file->Read<Matrix>(), file->Read<int32>(), file->Read<int32>(), file->Read<_uint>());
		if (nullptr == pHierarchyNode)
			return E_FAIL;

		m_HierarchyNodes.push_back(pHierarchyNode);
	}

	/* 2�� ���� �Ľ� (�ε����� �������� �θ� �����ͷ� �ٽ� ����) */
	for (auto& node : m_HierarchyNodes)
	{
		if (nullptr == node)
			return E_FAIL;

		node->Set_Parent(Get_HierarchyNode(node->Get_ParentIndex()));
	}
	return S_OK;
}

HRESULT CModel::Read_MeshData(const string& strPath, Matrix PivotMatrix)
{
	string filePath = strPath + filesystem::path(strPath).filename().string() + ".mesh";
	if (!Util_File::IsExistFile(filePath))
		return E_FAIL;

	shared_ptr<Util_File> file = make_shared<Util_File>();
	file->Open(Util_String::ToWString(filePath), FileMode::Read);

	size_t iNumMeshes = file->Read<size_t>(); 
	for (size_t i = 0; i < iNumMeshes; i++)
	{
		/* Name, Type */
		string					strName = file->Read<string>();
		_bool					bAnim = file->Read<_bool>();

		vector<VTXMODEL>		StaticVertices;
		vector<VTXANIMMODEL>	AnimVertices;
		vector<_int>			Indiecs;
		_uint					iMaterialIndex;
		vector<CHierarchyNode*>	Bones;

		/* Vertices */
		if (bAnim)
		{
			size_t iNumVertices = file->Read<size_t>();
			AnimVertices.reserve(iNumVertices);

			for (size_t j = 0; j < iNumVertices; j++)
			{
				VTXANIMMODEL vertex;

				vertex.vPosition = file->Read<Vec3>();
				vertex.vNormal = file->Read<Vec3>();
				vertex.vTexture = file->Read<Vec2>();
				vertex.vTangent = file->Read<Vec3>();
				vertex.vBlendIndex = file->Read<XMUINT4>();
				vertex.vBlendWeight = file->Read<Vec4>();

				AnimVertices.push_back(vertex);
			}
		}
		else
		{
			size_t iNumVertices = file->Read<size_t>();
			StaticVertices.reserve(iNumVertices);

			for (size_t j = 0; j < iNumVertices; j++)
			{
				VTXMODEL vertex;

				vertex.vPosition = file->Read<Vec3>();
				vertex.vNormal = file->Read<Vec3>();
				vertex.vTexture = file->Read<Vec2>();
				vertex.vTangent = file->Read<Vec3>();

				StaticVertices.push_back(vertex);
			}
		}

		/* Indices */
		size_t iNumIndices = file->Read<size_t>();
		Indiecs.reserve(iNumIndices);
		for (size_t j = 0; j < iNumIndices; j++)
		{
			Indiecs.push_back(file->Read<_int>());
		}

		/* Material Index */
		iMaterialIndex = file->Read<_uint>();

		/* Bone Indices*/
		size_t iNumBoneIndices = file->Read<size_t>();
		Bones.reserve(iNumBoneIndices);
		for (size_t j = 0; j < iNumBoneIndices; j++)
		{
			Bones.push_back(Get_HierarchyNode(file->Read<_int>()));
		}

		/* Create Mesh */
		CMesh* pMesh = nullptr;

	
		if(bAnim)
			pMesh = CMesh::Create(m_pDevice, m_pContext, strName, AnimVertices, Indiecs, iMaterialIndex, Bones);
		else					 
			pMesh = CMesh::Create(m_pDevice, m_pContext, strName, StaticVertices, Indiecs, iMaterialIndex, Bones, PivotMatrix);

		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Read_MaterialData(const string& strPath)
{
	string filePath = strPath + filesystem::path(strPath).filename().string() + ".mat";
	if (!Util_File::IsExistFile(filePath))
		return E_FAIL;

	shared_ptr<Util_File> file = make_shared<Util_File>();
	file->Open(Util_String::ToWString(filePath), FileMode::Read);

	size_t iNumMaterials = file->Read<size_t>();
	for (size_t i = 0; i < iNumMaterials; i++)
	{
		MATERIALDESC		MaterialDesc;
		ZeroMemory(&MaterialDesc, sizeof(MATERIALDESC));
		{
			string path;
			
			path = file->Read<string>();
			MaterialDesc.pTexture[aiTextureType_DIFFUSE] = CTexture::Create(m_pDevice, m_pContext, Util_String::ToWString(path));

			path = file->Read<string>();
			MaterialDesc.pTexture[aiTextureType_SPECULAR] = CTexture::Create(m_pDevice, m_pContext, Util_String::ToWString(path));

			path = file->Read<string>();
			MaterialDesc.pTexture[aiTextureType_NORMALS] = CTexture::Create(m_pDevice, m_pContext, Util_String::ToWString(path));

		}
		m_Materials.push_back(MaterialDesc);
	}
	return S_OK;
}

HRESULT CModel::Read_AnimaionData(const string& strPath)
{
	string filePath = strPath + filesystem::path(strPath).filename().string() + ".anim";
	if (!Util_File::IsExistFile(filePath))
		return E_FAIL;

	shared_ptr<Util_File> file = make_shared<Util_File>();
	file->Open(Util_String::ToWString(filePath), FileMode::Read);

	/* ��� �ִϸ��̼� ��ȸ */
	size_t iNumAnims = file->Read<size_t>();
	for (size_t i = 0; i < iNumAnims; i++)
	{
		_float fDuration = file->Read<_float>();
		_float fTickPerSecond = file->Read<_float>();

		/* ��� ä�� ��ȸ */
		size_t iNumChannels = file->Read<size_t>();
		vector<CChannel*> Channels;
		Channels.reserve(iNumChannels);
		for (size_t j = 0; j < iNumChannels; j++)
		{
			string strName = file->Read<string>();

			/* ��� Ű������ ��ȸ */
			size_t iNumKeyframes = file->Read<size_t>();
			vector<KEYFRAME> Keyframes;
			Keyframes.reserve(iNumKeyframes);
			for (size_t k = 0; k < iNumKeyframes; k++)
			{
				KEYFRAME keyframe;

				keyframe.fTime = file->Read<_float>();
				keyframe.vScale = file->Read<Vec3>();
				keyframe.vRotation = file->Read<Vec4>();
				keyframe.vPosition = file->Read<Vec3>();

				Keyframes.push_back(keyframe);
			}

			CChannel* pChannel = CChannel::Create(strName, Keyframes);
			if (nullptr == pChannel)
				return E_FAIL;

			Channels.push_back(pChannel);
		}

		CAnimation* pAnimation = CAnimation::Create(fDuration, fTickPerSecond, Channels);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);
	}
	return S_OK;
}

CHierarchyNode * CModel::Get_HierarchyNode(const char * pNodeName)
{
	auto	iter = find_if(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [&](CHierarchyNode* pNode) 
	{
		return !strcmp(pNodeName, pNode->Get_Name());
	});

	if (iter == m_HierarchyNodes.end())
		return nullptr;

	return *iter;	
}

CHierarchyNode* CModel::Get_HierarchyNode(const _uint& iIndex)
{
	if(m_HierarchyNodes.size() < iIndex)
		return nullptr;

	return m_HierarchyNodes[iIndex];
}

_uint CModel::Get_MaterialIndex(_uint iMeshIndex)
{
	return m_Meshes[iMeshIndex]->Get_MaterialIndex();
}

HRESULT CModel::SetUp_OnShader(CShader * pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char * pConstantName)
{
	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;
	
	if (m_Materials[iMaterialIndex].pTexture[eTextureType] == nullptr)
		return S_OK;

	return m_Materials[iMaterialIndex].pTexture[eTextureType]->Bind_ShaderResource(pShader, pConstantName, 0);
}

HRESULT CModel::Update_Anim(_float fTimeDelta)
{
	if (m_iCurrentAnimIndex >= m_iNumAnimations) return E_FAIL;

	/* ���� �ִϸ��̼��� ��� ä�� Ű������ ���� (���� �θ� ����) - Relative */
	m_Animations[m_iCurrentAnimIndex]->Play_Animation(fTimeDelta);

	/* ��� ���� ��ȸ�ϸ� ��Ʈ ���� Ʈ�������� ����Ͽ� �����Ѵ�.(��Ʈ ����) - Global */
	/* cf. m_HierarchyNodes�� �θ𿡼� �ڽ� ������ ���������� ���ĵǾ� �ִ� ���´�. */
	for (auto& pHierarchyNode : m_HierarchyNodes)
		pHierarchyNode->Set_CombinedTransformation();

	return S_OK;
}

HRESULT CModel::Render(CShader* pShader, _uint iMeshIndex, _uint iPassIndex)
{
	/* TODO ������ ���� ���� */

	_float4x4		BoneMatrices[MAX_BONES];

	if (TYPE_ANIM == m_eModelType) 
	{
		/* ���� ���� Ʈ������ ��� : <������ * ��Ʈ ���� * ������ȯ> */
		m_Meshes[iMeshIndex]->SetUp_BoneMatrices(BoneMatrices, XMLoadFloat4x4(&m_PivotMatrix));

   		if (FAILED(pShader->Bind_RawValue("g_BoneMatrices", BoneMatrices, sizeof(_float4x4) * MAX_BONES)))
			return E_FAIL;
	}

	pShader->Begin(0);
	
	m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

//HRESULT CModel::Ready_Meshes(_fmatrix PivotMatrix)
//{
//	/* �޽��� ������ ���´�. */
//	m_iNumMeshes = m_pAIScene->mNumMeshes;
//	
//	/* �޽� ������ �Ľ��Ͽ� �޽ø� �����Ѵ�. */
//	for (_uint i = 0; i < m_iNumMeshes; ++i)
//	{
//		CMesh*		pMeshContainer = CMesh::Create(m_pDevice, m_pContext, m_eModelType, m_pAIScene->mMeshes[i], this, PivotMatrix);
//		if (nullptr == pMeshContainer)
//			return E_FAIL;
//
//		m_Meshes.push_back(pMeshContainer);
//	}
//
//	return S_OK;
//}

//HRESULT CModel::Ready_Materials(const char* pModelFilePath)
//{
//	/* ���¸����� ���� �޾��� �� �����ؾ��� ���� ������ ��Ÿ����. */
//	/* ���¸����� �ȼ����� ���ǵȴ� (�𵨸���X, ��������X) -> �ؽ�ó�� ǥ���ȴ�. */
//	/* �� �ϳ��� ���¸����� ��ǻ�� �ؽ�ó, �븻 �ؽ�ó, ����ŧ�� �ؽ�ó�� �ؽ�ó�� ���ռ��̴�. */
//	
//	if (nullptr == m_pAIScene)
//		return E_FAIL;
//
//	/* ���¸��� ������ ���´�. */
//	m_iNumMaterials = m_pAIScene->mNumMaterials;
//
//	/* ���� ������ �ִ� ���¸����� ��ȸ�ϸ�, �ش� ���¸����� ����ϴ� �ؽ�ó���� �޸𸮿� �����Ѵ�. */
//	for (_uint i = 0; i < m_iNumMaterials; ++i)
//	{
//		MATERIALDESC		MaterialDesc;
//		ZeroMemory(&MaterialDesc, sizeof(MATERIALDESC));
//
//		aiMaterial*			pAIMaterial = m_pAIScene->mMaterials[i];
//
//		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
//		{			
//			aiString		strPath;			
//
//			/* �ش� ������ ǥ���ϱ����� �ؽ����� ��θ� strPath�� �޾ƿ´�. */
//			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strPath)))
//				continue;		
//
//			char			szFullPath[MAX_PATH] = "";
//			char			szFileName[MAX_PATH] = "";
//			char			szExt[MAX_PATH] = "";
//
//			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);
//
//			strcpy_s(szFullPath, pModelFilePath);
//			strcat_s(szFullPath, szFileName);
//			strcat_s(szFullPath, szExt);
//
//			_tchar			szWideFullPath[MAX_PATH] = TEXT("");
//
//			/* �� ��ο� �ؽ�ó �̸��� ���� �ؽ�ó�� ������ ���� ��θ� �����Ѵ�. */
//			MultiByteToWideChar(CP_ACP, 0, szFullPath, _int(strlen(szFullPath)), szWideFullPath, MAX_PATH);
//
//			/* ���������� �ؽ�ó�� �����Ѵ�. */
//			MaterialDesc.pTexture[j] = CTexture::Create(m_pDevice, m_pContext, szWideFullPath);
//			if (nullptr == MaterialDesc.pTexture[j])
//				return E_FAIL;			
//		}	
//
//		m_Materials.push_back(MaterialDesc);
//	}
//
//	return S_OK;
//}

//HRESULT CModel::Ready_HierarchyNodes(aiNode* pNode, CHierarchyNode* pParent, _uint iDepth)
//{
//	/* pParent? : �θ� ��� �ּ�. CombinedTransformation���� �׸���. CombinedTransformation�� ������� ���� Transformation * �θ��� CombinedTranfsormation�� �����. */
//	/* �۷ι��� ǥ���ϴ� �� ����. */
//	/* ���� �ִϸ��̼� ����Ҷ� ���� CombinedTransformation�� �����. */
//	
//	/* CombinedTransformation������� �θ��� ���°� ��� ���ŵǾ��־�ߵ�. �� �θ��� �Ĺ��ε� �̿��ϴϱ� */
//	/* �θ���� �ڽ����� ���������� CombinedTransformation�� �������Ѵٶ�� �� �ǹ�.* /
//	/* ���� m_HierarchyNodes �����̳ʴ� �ֻ��� �θ� ���� �տ� �־���Ѵ�. �̳��� 1�� �ڽĵ��� �ι�°�� ��. ����¥�ĵ��� �״����Ҿ�. */
//
//	/* iDepth? : Ready_HierarchyNodes�Լ��� ������·� �θ��� �ֱ� ������ ����(����)���� �����س����� ������. */
//	/* �� ��帶�� ���̰�(�����ڽ��̳�?) �� �����صΰ� ���߿� �����Ѵ�. */
//
//	CHierarchyNode*		pHierarchyNode = CHierarchyNode::Create(pNode, pParent, iDepth++);
//	if (nullptr == pHierarchyNode)
//		return E_FAIL;
//
//	m_HierarchyNodes.push_back(pHierarchyNode);
//
//	/* ��� (���� ��ȸ) */
//	for (_uint i = 0; i < pNode->mNumChildren; ++i)
//		Ready_HierarchyNodes(pNode->mChildren[i], pHierarchyNode, iDepth);
//
//	return S_OK;
//}

HRESULT CModel::Ready_Animations()
{
	///* ���� ����ϴ� �ִϸ��̼��� ������ �޾ƿ´�. */
	//m_iNumAnimations = m_pAIScene->mNumAnimations;

	//for (_uint i = 0; i < m_pAIScene->mNumAnimations; ++i)
	//{
	//	aiAnimation*		pAIAnimation = m_pAIScene->mAnimations[i];

	//	CAnimation*			pAnimation = CAnimation::Create(pAIAnimation);
	//	if (nullptr == pAnimation)
	//		return E_FAIL;

	//	m_Animations.push_back(pAnimation);
	//}	
	return S_OK;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const string& strPath, _fmatrix PivotMatrix)
{
	CModel*			pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(strPath, PivotMatrix)))
	{
		MSG_BOX("Failed To Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
	
}

CComponent * CModel::Clone(void * pArg)
{
	CModel*			pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	/* HierarachyNode */
	for (auto& pHierarchyNode : m_HierarchyNodes)
		Safe_Release(pHierarchyNode);
	m_HierarchyNodes.clear();

	/* Material */
	for (auto& Material : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_Release(Material.pTexture[i]);
	}
	m_Materials.clear();

	/* Meshes */
	for (auto& pMeshContainer : m_Meshes)
		Safe_Release(pMeshContainer);
	m_Meshes.clear();

	/* Animations */
	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);
	m_Animations.clear();
}
