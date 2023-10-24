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

#include "GameObject.h"
#include "Transform.h"

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

	for (size_t i = 0; i < BONE_END; i++)
		m_AnimBoneIndecies[i] = rhs.m_AnimBoneIndecies[i];
}

HRESULT CModel::Initialize_Prototype(const string& strPath, _fmatrix PivotMatrix)
{
	// "../Bin/Resources/Models/Characters/Chai/"

	for (size_t i = 0; i < BONE_END; i++)
		m_AnimBoneIndecies[i] = -1;

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

HRESULT CModel::Update(_float fTimeDelta)
{
	if (TYPE::TYPE_ANIM != m_eModelType) 
		return S_OK;

	if (FAILED(Update_Anim(fTimeDelta)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Update_Anim(_float fTimeDelta)
{
	if (m_bStopAnim)
		return TRUE;

	m_TweenDesc.cur.fFrameAcc += fTimeDelta;
	m_TweenDesc.cur.fAnimAcc += fTimeDelta;

	CAnimation* pCurAnim = Get_Animation(m_TweenDesc.cur.iAnimIndex);
	if (nullptr != pCurAnim)
	{
		_float timePerFrame = 1 / (pCurAnim->Get_TickPerSecond() * m_TweenDesc.cur.fSpeed);

		/* �� ������ �� */
		if (m_TweenDesc.cur.fFrameAcc >= timePerFrame)
		{
			m_TweenDesc.cur.fFrameAcc = 0;
			m_TweenDesc.cur.iCurFrame = (m_TweenDesc.cur.iCurFrame + 1) % pCurAnim->Get_MaxFrameCount();
			m_TweenDesc.cur.iNextFrame = (m_TweenDesc.cur.iCurFrame + 1) % pCurAnim->Get_MaxFrameCount();
		}
		m_TweenDesc.cur.fRatio = (m_TweenDesc.cur.fFrameAcc / timePerFrame);

		/* ���� �ִϸ��̼� ����� */
		if (0 == m_TweenDesc.cur.iNextFrame)
		{			
			/* Ư�� ��Ʈ ������ �� : ���� �ִ� ��Ʈ �������� 0�� �ƴ϶�� */
			m_vPrevAnimRoot = Vec4(Get_AnimBoneLocal(m_TweenDesc.cur.iAnimIndex, m_TweenDesc.cur.iCurFrame, BONE_ROOT).m[3]);
			if (Vec4::UnitW != m_vPrevAnimRoot)
			{
				CTransform* pTransform = m_pOwner->Get_Transform();
				Vec4 vPos = pTransform->Get_FinalPosition();
				pTransform->Set_Position(vPos);
				pTransform->Set_RootPos(Vec4::Zero);
			}

			m_bFinishAnimation = TRUE;
			m_TweenDesc.cur.ClearAnim();
			m_TweenDesc.cur.fFrameAcc = 1.f;
		}
		else
		{
			m_bFinishAnimation = FALSE;
		}
	}


	/* ���� �ִϸ��̼��� ����Ǿ� �ִٸ� */
	if (m_TweenDesc.next.iAnimIndex >= 0)
	{
		m_TweenDesc.fTweenAcc += fTimeDelta;
		m_TweenDesc.fTweenRatio = m_TweenDesc.fTweenAcc / m_TweenDesc.fTweenDuration;
		
		/* Ʈ������ �����ٸ� */
		if (m_TweenDesc.fTweenRatio >= 1.f)
		{
			/* Ư�� ��Ʈ ������ �� : ���� �ִ� ��Ʈ �������� 0�� �ƴ϶�� */
			m_vPrevAnimRoot = Vec4(Get_AnimBoneLocal(m_TweenDesc.cur.iAnimIndex, m_TweenDesc.cur.iCurFrame, BONE_ROOT).m[3]);
			if (Vec4::UnitW != m_vPrevAnimRoot)
			{
				/* ���� �ִϸ��̼� ��Ʈ �������� 0 �̶��, ���� �ִϸ��̼��� ��Ʈ �����Ǹ��� ����Ѵ�. */
				Vec4 vRootPos, m_vNextAnimRoot;
				m_vNextAnimRoot = Vec4(Get_AnimBoneLocal(m_TweenDesc.next.iAnimIndex, m_TweenDesc.next.iCurFrame, BONE_ROOT).m[3]);
				if (Vec4::UnitW == m_vNextAnimRoot)
					vRootPos = Get_AnimBoneRootNoneLerp();
				else
					vRootPos = Get_AnimBonePos(BONE_ROOT);

				m_pOwner->Get_Transform()->Set_RootPos(vRootPos);

				CTransform* pTransform = m_pOwner->Get_Transform();
				Vec4 vPos = pTransform->Get_FinalPosition();
				pTransform->Set_Position(vPos);
				pTransform->Set_RootPos(Vec4::Zero);
			}

			m_TweenDesc.cur.ClearAnim();
			m_TweenDesc.cur = m_TweenDesc.next;
			m_TweenDesc.ClearNextAnim();

			m_bFinishTween = TRUE;

			return S_OK;
		}
		else
		{
			CAnimation* pNextAnim = Get_Animation(m_TweenDesc.next.iAnimIndex);
			if (nullptr != pNextAnim)
			{
				m_TweenDesc.next.fFrameAcc += fTimeDelta;

				_float timePerFrame = 1.f / (pNextAnim->Get_TickPerSecond() * m_TweenDesc.next.fSpeed);

				if (m_TweenDesc.next.fRatio >= 1.f)
				{
					m_TweenDesc.next.fFrameAcc = 0;

					m_TweenDesc.next.iCurFrame = (m_TweenDesc.next.iCurFrame + 1) % pNextAnim->Get_MaxFrameCount();
					m_TweenDesc.next.iNextFrame = (m_TweenDesc.next.iCurFrame + 1) % pNextAnim->Get_MaxFrameCount();
				}

				m_TweenDesc.next.fRatio = m_TweenDesc.next.fFrameAcc / timePerFrame;
			}

			/* �Ϲ� ��Ʈ ������ �� */
			Set_RootPosition_Tween();
		}
	}
	else
	{
		m_bFinishTween = FALSE;

		Set_RootPosition();
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

void CModel::Set_Animation(const _uint& iAnimIndex, const _float& fSpeed, const _float& fTweenDuration)
{
	/*if (iAnimIndex == m_TweenDesc.cur.iAnimIndex && -1 != m_TweenDesc.next.iAnimIndex || iAnimIndex == m_TweenDesc.next.iAnimIndex)
		return;*/

	m_TweenDesc.ClearNextAnim();

	m_TweenDesc.next.iAnimIndex = iAnimIndex % Get_AnimationCount();
	m_TweenDesc.next.fSpeed = fSpeed;
	m_TweenDesc.fTweenDuration = fTweenDuration;

	m_TweenDesc.cur.fSpeed = fSpeed;
}

void CModel::Set_AnimationSpeed(const _float& fSpeed)
{
	m_TweenDesc.cur.fSpeed = fSpeed;
}

void CModel::Set_BoneIndex(const BONE_TYPE& eType, const _int iIndex)
{
	switch (eType)
	{
	case BONE_ROOT:
		m_AnimBoneIndecies[BONE_ROOT] = iIndex;
		break;
	case BONE_SOCKET_LEFT:
		m_AnimBoneIndecies[BONE_SOCKET_LEFT] = iIndex;
		break;
	case BONE_SOCKET_RIGHT:
		m_AnimBoneIndecies[BONE_SOCKET_RIGHT] = iIndex;
		break;
	default:
		break;
	}
}

const _bool CModel::Is_TwoThirds_Animation()
{
	CAnimation* pCurAnim = Get_Animation(m_TweenDesc.cur.iAnimIndex);

	if (nullptr != pCurAnim)
	{
		if (0.67f < (m_TweenDesc.cur.iCurFrame / (_float)pCurAnim->Get_MaxFrameCount()))
			return TRUE;
	}
	return FALSE;
}

const _bool CModel::Is_Tween()
{
	if (0 <= m_TweenDesc.next.iAnimIndex && 1.f > m_TweenDesc.fTweenRatio)
		return TRUE;
	else
		return FALSE;
}

const _bool CModel::Is_Contain_InTween(const _uint& iAnimIndex)
{
	if (iAnimIndex == m_TweenDesc.cur.iAnimIndex || iAnimIndex == m_TweenDesc.next.iAnimIndex)
		return TRUE;

	return FALSE;
}

const _bool CModel::Is_Playing_Animation(const _uint& iAnimIndex)
{
	/* ���� �ִϸ��̼����� �����(2/3����)�̰ų�, ���� �ִϸ��̼����� ���� �Ǿ����� ��� TRUE */

	if (m_TweenDesc.cur.iAnimIndex != iAnimIndex && m_TweenDesc.next.iAnimIndex == iAnimIndex)
		return TRUE;
	else if (m_TweenDesc.cur.iAnimIndex == iAnimIndex && m_TweenDesc.next.iAnimIndex == iAnimIndex)
		return TRUE;
	else if (m_TweenDesc.cur.iAnimIndex == iAnimIndex && !Is_TwoThirds_Animation())
		return TRUE;

	return FALSE;
}

const _bool CModel::Is_Half_Animation()
{
	CAnimation* pCurAnim = Get_Animation(m_TweenDesc.cur.iAnimIndex);

	if (nullptr != pCurAnim)
	{
		if (0.5f < (m_TweenDesc.cur.iCurFrame / (_float)pCurAnim->Get_MaxFrameCount()))
			return TRUE;
	}
	return FALSE;
}

const _bool CModel::Is_OneThirds_Animation()
{
	CAnimation* pCurAnim = Get_Animation(m_TweenDesc.cur.iAnimIndex);

	if (nullptr != pCurAnim)
	{
		if (0.33f < (m_TweenDesc.cur.iCurFrame / (_float)pCurAnim->Get_MaxFrameCount()))
			return TRUE;
	}
	return FALSE;
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
		string	strName = file->Read<string>();
		Matrix	transformMat = file->Read<Matrix>();
		Matrix	offsetMat = file->Read<Matrix>();
		int32	iBoneIndex = file->Read<int32>();
		int32	iParentIndex = file->Read<int32>();
		_uint	iDepth = file->Read<_uint>();

		CBone* pBone = CBone::Create(strName, transformMat, offsetMat, iBoneIndex, iParentIndex, iDepth);
		if (nullptr == pBone)
			return E_FAIL;

		if(strName == "origin_$AssimpFbx$_Translation")
			m_AnimBoneIndecies[BONE_ROOT] = (_uint)i;
		else if (strName == "r_attach_hand_00" || strName == "r_hand_attach_00")
			m_AnimBoneIndecies[BONE_SOCKET_RIGHT] = (_uint)i;
		else if (strName == "l_attach_hand_00" || strName == "l_hand_attach_00")
			m_AnimBoneIndecies[BONE_SOCKET_LEFT] = (_uint)i;

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
		string strName = file->Read<string>();
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

		CAnimation* pAnimation = CAnimation::Create(fDuration, fTickPerSecond, Channels, strName); 
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

	/* 01. For m_AnimTransforms */
	/* �ش� ���� ����ϴ� ��� �ִϸ��̼ǰ� Bone�� ������ m_AnimTransforms�� �����Ѵ�. */
	vector<ANIM_TRANSFORM_CACHE>		m_AnimTransformsCache;

	_uint iBoneCount = (_uint)m_Bones.size();
	_uint iAnimCnt = Get_AnimationCount();
	_uint iAnimMaxFrameCount = 0;
	for (uint32 i = 0; i < iAnimCnt; i++)
	{
		_uint iCurAnimFrameCnt = m_Animations[i]->Get_MaxFrameCount();

		iAnimMaxFrameCount = iAnimMaxFrameCount < iCurAnimFrameCnt ? iCurAnimFrameCnt : iAnimMaxFrameCount;
	}
	{
		if (0 == iAnimCnt) return S_OK;

		m_AnimTransforms.resize(iAnimCnt);
		m_AnimTransformsCache.resize(iAnimCnt);

		for (uint32 i = 0; i < iAnimCnt; i++)
			Create_AnimationTransform(i, m_AnimTransforms);

		if (m_bRootAnimation)
		{
			for (uint32 i = 0; i < iAnimCnt; i++)
				Create_AnimationTransformCache(i, m_AnimTransformsCache);
		}
	}

	/* 02. For. m_pTexture */
	ID3D11Texture2D* pTexture = nullptr;
	{
		D3D11_TEXTURE2D_DESC desc;
		{
			ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
			desc.Width = iBoneCount * 4;			/* 4���� �ɰ� ���� ���� 4�� ����*/
			desc.Height = iAnimMaxFrameCount;
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
		const uint32 dataSize = iBoneCount * sizeof(Matrix);  /* ���� */
		const uint32 pageSize = dataSize * iAnimMaxFrameCount;			/* �� �� (���� * ����) */
		void* mallocPtr = ::malloc(pageSize * iAnimCnt);				/* �ؽ�ó �� ������ = n �� */

		/* _animTransforms�� ������ ���ۿ� ��� �Ҵ��Ѵ�. */
		for (uint32 c = 0; c < iAnimCnt; c++) /* �ִϸ��̼� ������ŭ �ݺ� (�� ��) */
		{
			uint32 startOffset = c * pageSize; /* �ִ� ī��Ʈ * �� �� /

			/* ����Ʈ ������ ���� �ϱ� ���� 1����Ʈ ¥���� ĳ���� */
			BYTE* pageStartPtr = reinterpret_cast<BYTE*>(mallocPtr) + startOffset;

			for (uint32 f = 0; f < iAnimMaxFrameCount; f++) /* Ű������ ������ŭ �ݺ� (���� ũ�⸸ŭ) */
			{
				void* ptr = pageStartPtr + dataSize * f;

				if (m_bRootAnimation)
					::memcpy(ptr, m_AnimTransformsCache[c].transforms[f].data(), dataSize); /* �ؽ�ó�� ���� 1�ٸ�ŭ ������ ���� */
				else
					::memcpy(ptr, m_AnimTransforms[c].transforms[f].data(), dataSize);
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

	/* Clear Cache*/
	if (FAILED(Clear_Cache()))
		return E_FAIL;

	return S_OK;
}

void CModel::Create_AnimationTransform(uint32 iAnimIndex, vector<ANIM_TRANSFORM>& pAnimTransform)
{
	/* ���� �ִϸ��̼ǿ� ���� �ؽ�ó �� ��(������ ��, �� ��)������ �����Ѵ�. */
	CAnimation* pAnimation = m_Animations[iAnimIndex];

	/* ��� ������ ��ȸ (�ؽ�ó ����) */
	for (uint32 iFrameIndex = 0; iFrameIndex < pAnimation->Get_MaxFrameCount(); iFrameIndex++)
	{
		/* ��� ä���� ���� ������ ���� */
		pAnimation->Calculate_Animation(iFrameIndex);

		/* ��� �� �۷ι� ��ȯ -> �ִϸ��̼� ��ȯ */
		
		for (uint32 iBoneIndex = 0; iBoneIndex < m_Bones.size(); iBoneIndex++)
		{
			m_Bones[iBoneIndex]->Set_CombinedTransformation();

			/* ��� �����̳ʿ��� ��Ʈ�� ���ϸ� ���� */
			if (m_AnimBoneIndecies[BONE_ROOT] == iBoneIndex)
			{
				pAnimTransform[iAnimIndex].transforms[iFrameIndex][BONE_ROOT]
					= m_Bones[iBoneIndex]->Get_OffSetMatrix() * m_Bones[iBoneIndex]->Get_CombinedTransformation() * Get_PivotMatrix();
			}
			else if (m_AnimBoneIndecies[BONE_SOCKET_LEFT] == iBoneIndex)
			{
				pAnimTransform[iAnimIndex].transforms[iFrameIndex][BONE_SOCKET_LEFT]
					= m_Bones[iBoneIndex]->Get_OffSetMatrix() * m_Bones[iBoneIndex]->Get_CombinedTransformation() * Get_PivotMatrix();
			}
			else if (m_AnimBoneIndecies[BONE_SOCKET_RIGHT] == iBoneIndex)
			{
				pAnimTransform[iAnimIndex].transforms[iFrameIndex][BONE_SOCKET_RIGHT]
					= m_Bones[iBoneIndex]->Get_OffSetMatrix() * m_Bones[iBoneIndex]->Get_CombinedTransformation() * Get_PivotMatrix();
			}
		}
	}
}

void CModel::Create_AnimationTransformCache(uint32 iAnimIndex, vector<ANIM_TRANSFORM_CACHE>& pAnimTransformCache)
{
	/* ���� �ִϸ��̼ǿ� ���� �ؽ�ó �� ��(������ ��, �� ��)������ �����Ѵ�. */
	CAnimation* pAnimation = m_Animations[iAnimIndex];

	/* ��� ������ ��ȸ (�ؽ�ó ����) */
	for (uint32 iFrameIndex = 0; iFrameIndex < pAnimation->Get_MaxFrameCount(); iFrameIndex++)
	{
		/* ��� ä���� ���� ������ ���� */
		pAnimation->Calculate_Animation(iFrameIndex);

		/* ��� �� �۷ι� ��ȯ -> �ִϸ��̼� ��ȯ -> ���� */

		for (uint32 iBoneIndex = 0; iBoneIndex < m_Bones.size(); iBoneIndex++)
		{
			if (iBoneIndex == m_AnimBoneIndecies[BONE_ROOT])
				m_Bones[iBoneIndex]->Set_Translate(Vec4(0, 0, 0, 1));
			
			m_Bones[iBoneIndex]->Set_CombinedTransformation();

			pAnimTransformCache[iAnimIndex].transforms[iFrameIndex][iBoneIndex]
				= m_Bones[iBoneIndex]->Get_OffSetMatrix() * m_Bones[iBoneIndex]->Get_CombinedTransformation() * Get_PivotMatrix();
		}
	}
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

const Matrix CModel::Get_AnimBoneMat(const BONE_TYPE& eType)
{
	/* ���̴����� ���� �ִϸ��̼�, �� ��� �ִϸ��̼�, ��� ������ ��Ʈ �������� 0, 0, 0�� �ִϸ��̼� ���´�. */
	/* �ҽ����� ��Ʈ ��ġ�� ������ ĳ���� �����ǿ� ������� �ش�. (ĳ���� �������� �ҽ��� ����, �ִϸ��̼��� �÷��̾� �����ǿ� ����) */

	/* ���̴����� ��Ʈ�� 0��, �� ���ڸ��� ������ �ִ� �ִϸ��̼��� ��ĵ��� �� ����*/
	/* m_AnimTransforms���� ���� �ִϸ��̼� ���� �������� ��Ʈ �������� ���� �� ����*/

	if(BONE_END <= eType)
		return Matrix();

	return Get_CurAnimBonefinal(eType);
}

const Matrix CModel::Get_AnimBoneMatNoneLerp(const BONE_TYPE& eType)
{
	if (BONE_END == eType)
		return Matrix::Identity;

	Matrix matCurLerp = Matrix::Lerp(Get_AnimBoneLocal(m_TweenDesc.cur.iAnimIndex, m_TweenDesc.cur.iCurFrame, eType)
		, Get_AnimBoneLocal(m_TweenDesc.cur.iAnimIndex, m_TweenDesc.cur.iNextFrame, eType)
		, m_TweenDesc.cur.fRatio);

	return matCurLerp;
}

const Vec4 CModel::Get_AnimBonePos(const BONE_TYPE& eType)
{
	if (BONE_END <= eType)
		return Vec4();

	return Vec4(Get_CurAnimBonefinal(eType).m[3]);
}

const Vec4 CModel::Get_AnimBoneRootNoneLerp()
{
	/* ���� �ִϸ��̼��� ��Ʈ �������� ��ȯ �Ѵ�. */
	Matrix matRootCurLerp = Matrix::Lerp(Get_AnimBoneLocal(m_TweenDesc.cur.iAnimIndex, m_TweenDesc.cur.iCurFrame, BONE_ROOT)
										, Get_AnimBoneLocal(m_TweenDesc.cur.iAnimIndex, m_TweenDesc.cur.iNextFrame, BONE_ROOT)
										, m_TweenDesc.cur.fRatio);


	return Vec4(matRootCurLerp.m[3]);
}

const Matrix CModel::Get_SocketBoneMat(const BONE_TYPE& eType)
{
	if(BONE_ROOT == eType || BONE_END == eType)
		return Matrix();

	/* ���� �ִϸ��̼� ��Ʈ �������� 0 �̶��, ���� �ִϸ��̼Ǹ� ���� . */
	/* �ƴ϶�� ���� �ִϸ��̼ǰ� ������ ��Ʈ������ �����Ѵ�. */

	/* ���� Ʈ�� ���̰� ���� ��Ʈ �������� 0�� �ƴϰ�, ���� �ִ� ��Ʈ �������� 0�̶��, ���� �������� ���� �ִϸ��̼��� ���� ������ �ʿ䰡 ���� (�ڷ� �������ϱ�) */
	/* ���� ���� �ִϸ��̼��� �� ��ġ�� �ݿ��ؾ� �ϴµ� */
	Matrix matBone;

	if (m_TweenDesc.next.iAnimIndex >= 0)
	{
		Vec4 m_vCurAnimRoot = Vec4(Get_AnimBoneLocal(m_TweenDesc.cur.iAnimIndex, m_TweenDesc.cur.iCurFrame, BONE_ROOT).m[3]);
		Vec4 m_vNextAnimRoot = Vec4(Get_AnimBoneLocal(m_TweenDesc.next.iAnimIndex, m_TweenDesc.next.iCurFrame, BONE_ROOT).m[3]);

		/* Attack, Dash -> IDLE, RUN */
		if (Vec4::UnitW != m_vCurAnimRoot && Vec4::UnitW == m_vNextAnimRoot)
		{
			matBone = Get_AnimBoneMat(eType);

			Matrix matCurBone = Matrix::Lerp(Get_AnimBoneLocal(m_TweenDesc.cur.iAnimIndex, m_TweenDesc.cur.iCurFrame, eType)
								, Get_AnimBoneLocal(m_TweenDesc.cur.iAnimIndex, m_TweenDesc.cur.iNextFrame, eType)
								, m_TweenDesc.cur.fRatio);

			matBone.m[3][2] = matCurBone.m[3][2];
		
			
			/*Matrix matNextBone = Matrix::Lerp(Get_AnimBoneLocal(m_TweenDesc.next.iAnimIndex, m_TweenDesc.next.iCurFrame, eType)
											, Get_AnimBoneLocal(m_TweenDesc.next.iAnimIndex, m_TweenDesc.next.iNextFrame, eType)
											, m_TweenDesc.next.fRatio);	*/		
		}
		else
			matBone = Get_AnimBoneMat(eType);

	}
	else
	{
		matBone = Get_AnimBoneMat(eType);
	}
	

	return matBone;
}

const Matrix CModel::Get_AnimBoneLocal(const _uint& iAnimIndex, const _uint& iFrameIndex, const BONE_TYPE& eBoneType)
{
	if (m_AnimTransforms.size() <= iAnimIndex || MAX_MODEL_KEYFRAMES < iFrameIndex || BONE_END <= eBoneType)
		return Matrix::Identity;

	Matrix matAnim = m_AnimTransforms[iAnimIndex].transforms[iFrameIndex][eBoneType];

	return matAnim;
}

const Matrix CModel::Get_CurAnimBonefinal(const BONE_TYPE& eBoneType)
{
	/* ���� ������ */
	Matrix matRootCurLerp = Matrix::Lerp(Get_AnimBoneLocal(m_TweenDesc.cur.iAnimIndex, m_TweenDesc.cur.iCurFrame, eBoneType)
										, Get_AnimBoneLocal(m_TweenDesc.cur.iAnimIndex, m_TweenDesc.cur.iNextFrame, eBoneType)
										, m_TweenDesc.cur.fRatio);

	/* ���� �������� ����Ǿ� �ִٸ� */
	if (0 <= m_TweenDesc.next.iAnimIndex)
	{
		Matrix matRootNextLerp = Matrix::Lerp(Get_AnimBoneLocal(m_TweenDesc.next.iAnimIndex, m_TweenDesc.next.iCurFrame, eBoneType)
											, Get_AnimBoneLocal(m_TweenDesc.next.iAnimIndex, m_TweenDesc.next.iNextFrame, eBoneType)
											, m_TweenDesc.next.fRatio);

		return Matrix::Lerp(matRootCurLerp, matRootNextLerp, m_TweenDesc.fTweenRatio);
	}

	return matRootCurLerp;
}

HRESULT CModel::Clear_Cache()
{
	/* Bone */
	for (auto& pBone : m_Bones)
		Safe_Release(pBone);
	m_Bones.clear();

	/* Clear Animation Member */
	for (uint32 i = 0; i < m_Animations.size(); i++)
	{
		m_Animations[i]->Clear_Channels();
		m_Animations[i]->Clear_Bones();
	}

	return S_OK;
}

void CModel::Set_RootPosition_Tween()
{
	if (!m_bRootAnimation) return;

	Vec4 vRootPos, m_vNextAnimRoot;

	m_vNextAnimRoot = Vec4(Get_AnimBoneLocal(m_TweenDesc.next.iAnimIndex, m_TweenDesc.next.iCurFrame, BONE_ROOT).m[3]);

	if (Vec4::UnitW == m_vNextAnimRoot)			/* ���� �ִ��� ���ڸ� �ִϸ��̼��̶�� */
		vRootPos = Get_AnimBoneRootNoneLerp();	/* ���� �ִ� ���� ���� ���� �ִ� ��Ʈ ���������� �� */
	else										/* �ƴ϶�� */
		vRootPos = Get_AnimBonePos(BONE_ROOT);	/* ���� �ִ԰� ���� �ִ��� ������ ��Ʈ ���������� �� */

	m_pOwner->Get_Transform()->Set_RootPos(vRootPos);
}

void CModel::Set_RootPosition()
{
	if (!m_bRootAnimation) return;

	m_pOwner->Get_Transform()->Set_RootPos(Get_AnimBonePos(BONE_ROOT));
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

	///* Bone */
	//for (auto& pBone : m_Bones)
	//	Safe_Release(pBone);
	//m_Bones.clear();

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

	/* Srv */
	Safe_Release(m_pSrv);
}
