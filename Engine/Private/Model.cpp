#include "..\Public\Model.h"

#include "Bone.h"
#include "Mesh.h"
#include "Texture.h"
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
	, m_Bones(rhs.m_Bones)
	, m_Animations(rhs.m_Animations)
	, m_iCurrentAnimIndex(rhs.m_iCurrentAnimIndex)
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_iNumAnimations(rhs.m_iNumAnimations)
{
	/* Bones */
	for (auto& pBone : m_Bones)
		Safe_AddRef(pBone);

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

	if (TYPE_ANIM == m_eModelType)
	{
		if (FAILED(Read_AnimaionData(strPath)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
	/* 본 깊은 복사 */
	vector<CBone*>			Bones;
	Bones.reserve(m_Bones.size());
	for (auto& pPrototype : m_Bones)
	{
		CBone* pBone = (CBone*)pPrototype->Clone();
		if (nullptr == pBone)
			return E_FAIL;

		Bones.push_back(pBone);
		Safe_Release(pPrototype);
	}
	m_Bones.clear();
	m_Bones = Bones;

	/* 2차 정보 파싱 (인덱스를 바탕으로 부모를 포인터로 다시 연결) */
	for (auto& pBone : m_Bones)
	{
		if (nullptr == pBone)
			return E_FAIL;

		pBone->Set_Parent(Get_Bone(pBone->Get_ParentIndex()));
	}

	/* 메시 깊은 복사 */
	vector<CMesh*>	Meshes;
	for (auto& pPrototype : m_Meshes)
	{
		CMesh* pMesh = (CMesh*)pPrototype->Clone(this);
		if (nullptr == pMesh)
			return E_FAIL;

		Meshes.push_back(pMesh);

		Safe_Release(pPrototype);
	}
	m_Meshes.clear();
	m_Meshes = Meshes;

	if (TYPE_ANIM == m_eModelType)
	{
		/* 애니메이션 깊은 복사 */
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
	}

	return S_OK;
}

HRESULT CModel::Read_BoneData(const string& strPath)
{
	string folderName = Util_String::GetFinalFolderName(strPath);
	string filePath = (filesystem::path(strPath) / string(folderName + ".bone")).string();
	Util_String::Replace(filePath, "\\", "/");

	if (!Util_File::IsExistFile(filePath))
		return E_FAIL;

	shared_ptr<Util_File> file = make_shared<Util_File>();
	file->Open(Util_String::ToWString(filePath), FileMode::Read);

	/* 1차 정보 파싱 (부모는 인덱스로만 알고 있다) */
	size_t iSize = file->Read<size_t>();
	for (size_t i = 0; i < iSize; i++)
	{
		string strName = file->Read<string>();
		Matrix transformMat = file->Read<Matrix>();
		Matrix offsetMat = file->Read<Matrix>();
		int32 iBoneIndex = file->Read<int32>();
		int32 iParentIndex = file->Read<int32>();
		_uint iDepth = file->Read<_uint>();

		CBone* pHierarchyNode = CBone::Create(strName, transformMat, offsetMat, iBoneIndex, iParentIndex, iDepth);
		if (nullptr == pHierarchyNode)
			return E_FAIL;

		m_Bones.push_back(pHierarchyNode);
	}

	return S_OK;
}

HRESULT CModel::Read_MeshData(const string& strPath, Matrix PivotMatrix)
{
	string folderName = Util_String::GetFinalFolderName(strPath);
	string filePath = (filesystem::path(strPath) / string(folderName + ".mesh")).string();
	Util_String::Replace(filePath, "\\", "/");

	if (!Util_File::IsExistFile(filePath))
		return E_FAIL;

	shared_ptr<Util_File> file = make_shared<Util_File>();
	file->Open(Util_String::ToWString(filePath), FileMode::Read);

	size_t iNumMeshes = file->Read<size_t>(); 
	m_iNumMeshes = (_uint)iNumMeshes;
	for (size_t i = 0; i < iNumMeshes; i++)
	{
		/* Name, Type */
		string					strName = file->Read<string>();
		_bool					bAnim = file->Read<_bool>();
		if (bAnim)
			m_eModelType = TYPE_ANIM;
		else
			m_eModelType = TYPE_NONANIM;

		vector<VTXMODEL>		StaticVertices;
		vector<VTXANIMMODEL>	AnimVertices;
		vector<_int>			Indiecs;
		_uint					iMaterialIndex;
		vector<_int>			BoneIndexs;

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
		BoneIndexs.reserve(iNumBoneIndices);
		for (size_t j = 0; j < iNumBoneIndices; j++)
		{
			BoneIndexs.push_back(file->Read<_int>());
		}

		/* Create Mesh */
		CMesh* pMesh = nullptr;
		{
			if(bAnim)
				pMesh = CMesh::Create(m_pDevice, m_pContext, strName, AnimVertices, Indiecs, iMaterialIndex, BoneIndexs, this);
			else					 
				pMesh = CMesh::Create(m_pDevice, m_pContext, strName, StaticVertices, Indiecs, iMaterialIndex, BoneIndexs, PivotMatrix, this);

			if (nullptr == pMesh)
				return E_FAIL;
		}
		m_Meshes.push_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Read_MaterialData(const string& strPath)
{
	string folderName = Util_String::GetFinalFolderName(strPath);
	string filePath = (filesystem::path(strPath) / string(folderName + ".mat")).string();
	Util_String::Replace(filePath, "\\", "/");

	if (!Util_File::IsExistFile(filePath))
		return E_FAIL;

	shared_ptr<Util_File> file = make_shared<Util_File>();
	file->Open(Util_String::ToWString(filePath), FileMode::Read);

	size_t iNumMaterials = file->Read<size_t>();
	m_iNumMaterials = (_uint)iNumMaterials;
	for (size_t i = 0; i < iNumMaterials; i++)
	{
		MATERIALDESC		MaterialDesc;
		ZeroMemory(&MaterialDesc, sizeof(MATERIALDESC));
		{
			string path;
			string fileName;

			fileName = file->Read<string>();
			if (!fileName.empty())
			{
				path = strPath + "/" + fileName;
				MaterialDesc.pTexture[aiTextureType_DIFFUSE] = CTexture::Create(m_pDevice, m_pContext, Util_String::ToWString(path));
			}

			fileName = file->Read<string>();
			if (!fileName.empty())
			{
				path = strPath + "/" + fileName;
				MaterialDesc.pTexture[aiTextureType_SPECULAR] = CTexture::Create(m_pDevice, m_pContext, Util_String::ToWString(path));
			}

			fileName = file->Read<string>();
			if (!fileName.empty())
			{
				path = strPath + "/" + fileName;
				MaterialDesc.pTexture[aiTextureType_NORMALS] = CTexture::Create(m_pDevice, m_pContext, Util_String::ToWString(path));
			}
		}
		m_Materials.push_back(MaterialDesc);
	}
	return S_OK;
}

HRESULT CModel::Read_AnimaionData(const string& strPath)
{
	string folderName = Util_String::GetFinalFolderName(strPath);
	string filePath = (filesystem::path(strPath) / string(folderName + ".anim")).string();
	Util_String::Replace(filePath, "\\", "/");

	if (!Util_File::IsExistFile(filePath))
		return E_FAIL;

	shared_ptr<Util_File> file = make_shared<Util_File>();
	file->Open(Util_String::ToWString(filePath), FileMode::Read);

	/* 모든 애니메이션 순회 */
	size_t iNumAnims = file->Read<size_t>();
	m_iNumAnimations = (_uint)iNumAnims;
	for (size_t i = 0; i < iNumAnims; i++)
	{
		_float fDuration = file->Read<_float>();
		_float fTickPerSecond = file->Read<_float>();

		/* 모든 채널 순회 */
		size_t iNumChannels = file->Read<size_t>();
		vector<CChannel*> Channels;
		Channels.reserve(iNumChannels);
		for (size_t j = 0; j < iNumChannels; j++)
		{
			string strName = file->Read<string>();

			/* 모든 키프레임 순회 */
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

CBone * CModel::Get_Bone(const char * pNodeName)
{
	auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pNode) 
	{
		return !strcmp(pNodeName, pNode->Get_Name());
	});

	if (iter == m_Bones.end())
		return nullptr;

	return *iter;	
}

CBone* CModel::Get_Bone(const _int& iIndex)
{
	if(m_Bones.size() < iIndex || iIndex < 0)
		return nullptr;

	return m_Bones[iIndex];
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

	/* 현재 애니메이션의 모든 채널 키프레임 보간 (아직 부모 기준) - Relative */
	m_Animations[m_iCurrentAnimIndex]->Play_Animation(fTimeDelta);

	/* 모든 뼈를 순회하며 루트 기준 트랜스폼을 계산하여 세팅한다.(루트 기준) - Global */
	/* cf. m_Bones은 부모에서 자식 순으로 순차적으로 정렬되어 있는 상태다. */
	for (auto& pHierarchyNode : m_Bones)
		pHierarchyNode->Set_CombinedTransformation();

	return S_OK;
}

HRESULT CModel::Render(CShader* pShader, _uint iMeshIndex, _uint iPassIndex)
{
	/* TODO 프레임 저하 유발 */

	_float4x4		BoneMatrices[MAX_BONES];

	if (TYPE_ANIM == m_eModelType) 
	{
		/* 본의 최종 트랜스폼 계산 : <오프셋 * 루트 기준 * 사전변환> */
		m_Meshes[iMeshIndex]->SetUp_BoneMatrices(BoneMatrices, XMLoadFloat4x4(&m_PivotMatrix));

   		if (FAILED(pShader->Bind_RawValue("g_BoneMatrices", BoneMatrices, sizeof(_float4x4) * MAX_BONES)))
			return E_FAIL;
	}

	pShader->Begin(0);
	
	m_Meshes[iMeshIndex]->Render();

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
	for (auto& pHierarchyNode : m_Bones)
		Safe_Release(pHierarchyNode);
	m_Bones.clear();

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
