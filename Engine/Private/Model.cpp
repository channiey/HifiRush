#include "..\Public\Model.h"

#include "Graphic_Device.h"

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
	, m_eModelType(rhs.m_eModelType)
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_Bones(rhs.m_Bones)
	, m_Meshes(rhs.m_Meshes)
	, m_Materials(rhs.m_Materials)
	, m_Animations(rhs.m_Animations)
{
	/* Bones */
	for (auto& pBone : m_Bones)
		Safe_AddRef(pBone);

	/* Meshes */
	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);

	/* Materials */
	for (auto& Material : m_Materials)
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i) /* TODO �̰� ������ 3���� ���µ� ? */
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
	/* Deep Copy - Bones */
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

	/* ���� �θ� �ε����� �������� ���� �θ� �����Ѵ�. */
	for (auto& pBone : m_Bones)
	{
		if (nullptr == pBone)
			return E_FAIL;

		pBone->Set_Parent(Get_Bone(pBone->Get_ParentIndex()));
	}

	/* Deep Copy - Meshes */
	vector<CMesh*>	Meshes;
	Meshes.reserve(m_Meshes.size());

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

	/* Deep Copy - Animations */
	if (TYPE_ANIM == m_eModelType)
	{
		vector<CAnimation*>		Animations;
		Animations.reserve(m_Animations.size());

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

	/* Create VTF */
	if (FAILED(Create_Texture()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CModel::Update_Anim(_float fTimeDelta)
{
	if (TYPE::TYPE_ANIM != m_eModelType) return S_OK;

	/* ���� �ִϸ��̼�  */
	m_TweenDesc.cur.fFrameAcc += fTimeDelta;
	m_TweenDesc.cur.fAnimAcc += fTimeDelta;
	CAnimation* pCurAnim = Get_Animation(m_TweenDesc.cur.iAnimIndex);
	if (nullptr != pCurAnim)
	{
		_float timePerFrame = 1 / pCurAnim->Get_TickPerSecond();
		/* �� ������ �� */
		if (m_TweenDesc.cur.fFrameAcc >= timePerFrame)
		{
			m_TweenDesc.cur.fFrameAcc = 0;
			m_TweenDesc.cur.iCurFrame = (m_TweenDesc.cur.iCurFrame + 1) % pCurAnim->Get_MaxFrameCount();
			m_TweenDesc.cur.iNextFrame = (m_TweenDesc.cur.iCurFrame + 1) % pCurAnim->Get_MaxFrameCount();
		}
		m_TweenDesc.cur.fRatio = (m_TweenDesc.cur.fFrameAcc / timePerFrame);

		/* ������ �ƴϰ� ���� �ִϸ��̼��� �� �����ٸ�*/
		if (!m_TweenDesc.cur.bLoop && 0 < m_iPrevAnimIndex && pCurAnim->Get_MaxFrameCount() - m_TweenDesc.cur.iCurFrame == 3)
		{
			Set_Animation(m_iPrevAnimIndex, TRUE);
			m_iPrevAnimIndex = -1;
		}
	}

	/* ���� �ִϸ��̼��� ��� ����Ǿ��ٸ� */
	if (0 == m_TweenDesc.cur.iCurFrame)
		m_TweenDesc.cur.fAnimAcc = 0.f;

	/* ���� �ִϸ��̼��� ����Ǿ� �ִٸ� */
	if (m_TweenDesc.next.iAnimIndex >= 0)
	{
		m_TweenDesc.fTweenAcc += fTimeDelta;
		m_TweenDesc.fTweenRatio = m_TweenDesc.fTweenAcc / m_TweenDesc.fTweenDuration;

		/* Ʈ������ �����ٸ� */
		if (m_TweenDesc.fTweenRatio >= 1.f)
		{
			m_TweenDesc.cur.ClearAnim();
			m_TweenDesc.cur = m_TweenDesc.next;
			m_TweenDesc.ClearNextAnim();
		}
		else
		{
			CAnimation* pNextAnim = Get_Animation(m_TweenDesc.next.iAnimIndex);

			m_TweenDesc.next.fFrameAcc += fTimeDelta;

			_float timePerFrame = 1.f / pNextAnim->Get_TickPerSecond();

			if (m_TweenDesc.next.fRatio >= 1.f)
			{
				m_TweenDesc.next.fFrameAcc = 0;

				m_TweenDesc.next.iCurFrame = (m_TweenDesc.next.iCurFrame + 1) % pNextAnim->Get_MaxFrameCount();
				m_TweenDesc.next.iNextFrame = (m_TweenDesc.next.iCurFrame + 1) % pNextAnim->Get_MaxFrameCount();
			}

			m_TweenDesc.next.fRatio = m_TweenDesc.next.fFrameAcc / timePerFrame;
		}
	}
	return S_OK;
}

HRESULT CModel::Render(CShader* pShader, _uint iMeshIndex, _uint iPassIndex)
{
	if (TYPE_ANIM == m_eModelType) 
	{
		if (FAILED(pShader->Bind_Texture("g_TransformMap", m_pSrv)))
			return E_FAIL;

		if (FAILED(pShader->Bind_RawValue("g_TweenFrames", &m_TweenDesc, sizeof(TWEEN_DESC))))
			return E_FAIL;
	}

	pShader->Begin(0);
	
	m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

HRESULT CModel::Bind_Material(CShader * pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char * pConstantName)
{
	if (iMaterialIndex >= m_Materials.size())
		return E_FAIL;
	
	if (m_Materials[iMaterialIndex].pTexture[eTextureType] == nullptr)
		return S_OK;

	return m_Materials[iMaterialIndex].pTexture[eTextureType]->Bind_ShaderResource(pShader, pConstantName, 0);
}

void CModel::Set_Animation(const _uint& iAnimIndex, const _bool& bLoop, const _float& fSpeed)
{
	m_TweenDesc.ClearNextAnim();

	m_TweenDesc.next.iAnimIndex = iAnimIndex % Get_AnimationCount();
	m_TweenDesc.next.bLoop = bLoop;
	m_TweenDesc.next.fSpeed = fSpeed;

	if(!m_TweenDesc.next.bLoop)
		m_iPrevAnimIndex = m_TweenDesc.cur.iAnimIndex;
}

HRESULT CModel::Read_BoneData(const string& strPath)
{
	/* ���� �¾� */
	string folderName, filePath;
	shared_ptr<Util_File> file = make_shared<Util_File>();
	{
		folderName = Util_String::GetFinalFolderName(strPath);
		filePath = (filesystem::path(strPath) / string(folderName + ".bone")).string();
		Util_String::Replace(filePath, "\\", "/");

		if (!Util_File::IsExistFile(filePath))
			return E_FAIL;

		file->Open(Util_String::ToWString(filePath), FileMode::Read);
	}

	/* ��� �� ��ȸ (�θ�� �ε����θ� ����, ���� �����ʹ� Ŭ�п��� ����) */
	size_t iSize = file->Read<size_t>();
	for (size_t i = 0; i < iSize; i++)
	{
		string	strName			= file->Read<string>();
		Matrix	transformMat	= file->Read<Matrix>();
		Matrix	offsetMat		= file->Read<Matrix>();
		int32	iBoneIndex		= file->Read<int32>();
		int32	iParentIndex	= file->Read<int32>();
		_uint	iDepth			= file->Read<_uint>();

		CBone* pBone = CBone::Create(strName, transformMat, offsetMat, iBoneIndex, iParentIndex, iDepth);
		if (nullptr == pBone)
			return E_FAIL;

		m_Bones.push_back(pBone);
	}

	return S_OK;
}

HRESULT CModel::Read_MeshData(const string& strPath, Matrix PivotMatrix)
{
	/* ���� �¾� */
	string folderName, filePath;
	shared_ptr<Util_File> file = make_shared<Util_File>();
	{
		folderName = Util_String::GetFinalFolderName(strPath);
		filePath = (filesystem::path(strPath) / string(folderName + ".mesh")).string();
		Util_String::Replace(filePath, "\\", "/");

		if (!Util_File::IsExistFile(filePath))
			return E_FAIL;

		file->Open(Util_String::ToWString(filePath), FileMode::Read);
	}

	/* ��� �޽� ��ȸ */
	size_t iNumMeshes = file->Read<size_t>(); 
	for (size_t i = 0; i < iNumMeshes; i++)
	{
		/* Name, Type */
		string					strName = file->Read<string>();
		_bool					bAnim = file->Read<_bool>();
		m_eModelType = bAnim ? TYPE_ANIM : TYPE_NONANIM;

		vector<VTXMODEL>		StaticVertices;
		vector<VTXANIMMODEL>	AnimVertices;
		vector<_int>			Indiecs;
		_uint					iMaterialIndex;
		vector<_int>			BoneIndices;

		/* Vertices */
		if (bAnim)
		{
			size_t iNumVertices = file->Read<size_t>();
			AnimVertices.reserve(iNumVertices);

			for (size_t j = 0; j < iNumVertices; j++)
			{
				VTXANIMMODEL vertex;

				vertex.vPosition	= file->Read<Vec3>();
				vertex.vNormal		= file->Read<Vec3>();
				vertex.vTexture		= file->Read<Vec2>();
				vertex.vTangent		= file->Read<Vec3>();
				vertex.vBlendIndex	= file->Read<XMUINT4>();
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

				vertex.vPosition	= file->Read<Vec3>();
				vertex.vNormal		= file->Read<Vec3>();
				vertex.vTexture		= file->Read<Vec2>();
				vertex.vTangent		= file->Read<Vec3>();

				StaticVertices.push_back(vertex);
			}
		}

		/* Indices */
		size_t iNumIndices = file->Read<size_t>();
		Indiecs.reserve(iNumIndices);
		for (size_t j = 0; j < iNumIndices; j++)
			Indiecs.push_back(file->Read<_int>());

		/* Material Index */
		iMaterialIndex = file->Read<_uint>();

		/* Bone Indices*/
		size_t iNumBoneIndices = file->Read<size_t>();
		BoneIndices.reserve(iNumBoneIndices);
		for (size_t j = 0; j < iNumBoneIndices; j++)
			BoneIndices.push_back(file->Read<_int>());

		/* Create Mesh */
		CMesh* pMesh = nullptr;
		{
			pMesh = (bAnim) ? CMesh::Create(m_pDevice, m_pContext, strName, AnimVertices, Indiecs, iMaterialIndex, BoneIndices, this) :
								CMesh::Create(m_pDevice, m_pContext, strName, StaticVertices, Indiecs, iMaterialIndex, BoneIndices, PivotMatrix, this);

			if (nullptr == pMesh)
				return E_FAIL;
		}
		m_Meshes.push_back(pMesh);
	}
	return S_OK;
}

HRESULT CModel::Read_MaterialData(const string& strPath)
{
	/* ���� �¾� */
	string folderName, filePath;
	shared_ptr<Util_File> file = make_shared<Util_File>();
	{
		folderName = Util_String::GetFinalFolderName(strPath);
		filePath = (filesystem::path(strPath) / string(folderName + ".mat")).string();
		Util_String::Replace(filePath, "\\", "/");

		if (!Util_File::IsExistFile(filePath))
			return E_FAIL;

		file->Open(Util_String::ToWString(filePath), FileMode::Read);
	}

	/* ��� ���¸��� ��ȸ */
	size_t iNumMaterials = file->Read<size_t>();
	for (size_t i = 0; i < iNumMaterials; i++)
	{
		MATERIALDESC		MaterialDesc;
		ZeroMemory(&MaterialDesc, sizeof(MATERIALDESC));
		{
			string path, fileName;

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
	/* ���� �¾� */
	string folderName, filePath;
	shared_ptr<Util_File> file = make_shared<Util_File>();
	{
		folderName = Util_String::GetFinalFolderName(strPath);
		filePath = (filesystem::path(strPath) / string(folderName + ".anim")).string();
		Util_String::Replace(filePath, "\\", "/");

		if (!Util_File::IsExistFile(filePath))
			return E_FAIL;

		file->Open(Util_String::ToWString(filePath), FileMode::Read);
	}


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

HRESULT CModel::Create_Texture()
{
	if (TYPE::TYPE_NONANIM == m_eModelType)
		return S_OK;

	//m_PivotMatrix *= Matrix::CreateRotationZ(DEG2RAD(90.f)) * Matrix::CreateRotationX(DEG2RAD(-90.f));

	/* 01. For m_AnimTransforms */
	/* �ش� ���� ����ϴ� ��� �ִϸ��̼ǰ� Bone�� ������ m_AnimTransforms�� �����Ѵ�. */
	_uint iAnimCnt = Get_AnimationCount();
	vector<ANIMTRANSFORM>	AnimTransforms;
	{
		if (0 == iAnimCnt) return S_OK;

		AnimTransforms.resize(iAnimCnt);

		for (uint32 i = 0; i < iAnimCnt; i++)
			Create_AnimationTransform(i, AnimTransforms);
	}

	/* 02. For. m_pTexture */
	ID3D11Texture2D* pTexture = nullptr;
	{
		D3D11_TEXTURE2D_DESC desc;
		{
			ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
			desc.Width = MAX_MODEL_TRANSFORMS * 4;			/* 4���� �ɰ� ���� ���� 4�� ����*/
			desc.Height = MAX_MODEL_KEYFRAMES;
			desc.ArraySize = iAnimCnt;						
			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;	/* 16����Ʈ */
			desc.Usage = D3D11_USAGE_IMMUTABLE;				/* ���� ������ �� ���� */
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.MipLevels = 1;
			desc.SampleDesc.Count = 1;
		}
		/* ������ ������ �ؽ�ó�� ä�δ� �ִ� 16����Ʈ���� �Ҵ� �����ѵ� �츮�� ��Ʈ���� �� 64����Ʈ�� �Ҵ��ؾ��Ѵ�. */
		/* ���� �� ä���� 4���� �ɰ��� ����Ѵ�. */

		/* �����͸� �Ҵ��� ���� ���� */
		const uint32 dataSize = MAX_MODEL_TRANSFORMS * sizeof(Matrix);  /* ���� */
		const uint32 pageSize = dataSize * MAX_MODEL_KEYFRAMES;			/* �� �� (���� * ����) */
		void* mallocPtr = ::malloc(pageSize * iAnimCnt);				/* �ؽ�ó �� ������ = n �� */

		/* _animTransforms�� ������ ���ۿ� ��� �Ҵ��Ѵ�. */
		for (uint32 c = 0; c < iAnimCnt; c++) /* �ִϸ��̼� ������ŭ �ݺ� (�� ��) */
		{
			uint32 startOffset = c * pageSize; /* �ִ� ī��Ʈ * �� �� /

			/* ����Ʈ ������ ���� �ϱ� ���� 1����Ʈ ¥���� ĳ���� */
			BYTE* pageStartPtr = reinterpret_cast<BYTE*>(mallocPtr) + startOffset;

			for (uint32 f = 0; f < MAX_MODEL_KEYFRAMES; f++) /* Ű������ ������ŭ �ݺ� (���� ũ�⸸ŭ) */
			{
				void* ptr = pageStartPtr + dataSize * f;
				::memcpy(ptr, AnimTransforms[c].transforms[f].data(), dataSize); /* �ؽ�ó�� ���� 1�ٸ�ŭ ������ ���� */
			}
		}

		/* �ؽ�ó�� ����� ���� D3D11_SUBRESOURCE_DATA ���� */
		vector<D3D11_SUBRESOURCE_DATA> subResources(iAnimCnt);
		for (uint32 c = 0; c < iAnimCnt; c++)
		{
			void* ptr = (BYTE*)mallocPtr + c * pageSize;
			subResources[c].pSysMem = ptr;
			subResources[c].SysMemPitch = dataSize;
			subResources[c].SysMemSlicePitch = pageSize;
		}

		/* �ؽ�ó ���� */
		if(FAILED(CGraphic_Device::GetInstance()->Get_Device()->CreateTexture2D(&desc, subResources.data(), &pTexture)))
			return E_FAIL;
		
		::free(mallocPtr);
	}

	/* 03. For. m_pSrv */
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.MipLevels = 1;
		desc.Texture2DArray.ArraySize = iAnimCnt;

		if(FAILED(CGraphic_Device::GetInstance()->Get_Device()->CreateShaderResourceView(pTexture, &desc, &m_pSrv)))
			return E_FAIL;	
	}

	return S_OK;
}

void CModel::Create_AnimationTransform(uint32 iAnimIndex, vector<AnimTransform>& pAnimTransform)
{
	/* ���� �ִϸ��̼ǿ� ���� �ؽ�ó �� ��(������ ��, �� ��)������ �����Ѵ�. */
	CAnimation* pAnimation = m_Animations[iAnimIndex];

	/* ��� ������ ��ȸ (�ؽ�ó ����) */
	for (uint32 iFrameIndex = 0; iFrameIndex < pAnimation->Get_MaxFrameCount(); iFrameIndex++)
	{
		/* ��� ä�� ���� */
		pAnimation->Calculate_Animation(iFrameIndex);

		/* ��� �� �۷ι� ��ȯ -> �ִϸ��̼� ��ȯ -> ���� */
		
		for (uint32 iBoneIndex = 0; iBoneIndex < m_Bones.size(); iBoneIndex++)
		{
			m_Bones[iBoneIndex]->Set_CombinedTransformation();

			pAnimTransform[iAnimIndex].transforms[iFrameIndex][iBoneIndex]
				= m_Bones[iBoneIndex]->Get_OffSetMatrix() * m_Bones[iBoneIndex]->Get_CombinedTransformation() * Get_PivotMatrix();
		}
	}

	pAnimation->Clear_Channels();
	pAnimation->Clear_Bones();
}

CBone* CModel::Get_Bone(const char * pNodeName)
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

CAnimation* CModel::Get_Animation(const _uint& iIndex)
{
	if(m_Animations.size() < iIndex)
		return nullptr;

	return m_Animations[iIndex];
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

	Safe_Release(m_pSrv);
}
