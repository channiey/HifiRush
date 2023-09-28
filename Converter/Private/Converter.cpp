#include "Converter.h"


CConverter::CConverter()
{
}

CConverter::~CConverter()
{
}

HRESULT CConverter::Binarize_Model_Static(wstring srcPath, wstring savePath)
{
	if (FAILED(Read_AssetFile(srcPath, MODEL_TYPE::STATIC)))
		return E_FAIL;


	return S_OK;
}

HRESULT CConverter::Binarize_Model_Anim(wstring srcPath, wstring savePath)
{
	if (FAILED(Read_AssetFile(srcPath, MODEL_TYPE::ANIM)))
		return E_FAIL;


	return S_OK;
}


HRESULT CConverter::Read_AssetFile(wstring srcPath, const MODEL_TYPE modelType)
{
	if (MODEL_TYPE::TYPEEND == modelType)
		return E_FAIL;

	_modelType = modelType;

	int	iFlag = 0;
	{
		if (MODEL_TYPE::STATIC == _modelType)
			iFlag |= aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace;
		else
			iFlag |= aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace;
	}

	_scene = _importer->ReadFile(Utils_String::ToString(srcPath), iFlag);
	{
		if (nullptr == _scene)
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CConverter::Export_MeshData(wstring savePath)
{
	if (FAILED(Read_MeshData()))
		return E_FAIL;

	if (FAILED(Write_MeshData(savePath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CConverter::Export_MaterialData(wstring savePath)
{
	if (FAILED(Read_MaterialData()))
		return E_FAIL;

	if (FAILED(Write_MaterialData(savePath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CConverter::Export_AnimData(wstring savePath)
{
	if (FAILED(Read_AnimData()))
		return E_FAIL;

	if (FAILED(Write_AnimData(savePath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CConverter::Export_BoneData(wstring savePath)
{
	if(FAILED(Read_BoneData(_scene->mRootNode, -1, -1, 0)))
		return S_OK;


	return S_OK;
}

HRESULT CConverter::Read_MeshData(MODEL_TYPE modelType)
{
	if (nullptr == _scene)
		return E_FAIL;

	/* 모델이 가진 메시를 전부 순회하며 메시 구조체를 만들어 _meshes에 푸시한다. */
	for (uint i = 0; i < _scene->mNumMeshes; i++)
	{
		shared_ptr<asMesh> mesh = make_shared<asMesh>();
		mesh->name = _scene->mMeshes[i]->mName.data;

		const aiMesh* srcMesh = _scene->mMeshes[i];

		/* Vertices - Static */ 
		if (modelType == MODEL_TYPE::STATIC) 
		{
			mesh->verticesStatic.reserve(srcMesh->mNumVertices);

			VTXSTATIC vertex{};
			for (uint j = 0; j < mesh->verticesStatic.size(); j++)
			{
				memcpy(&vertex.vPosition, &srcMesh->mVertices[j], sizeof(Vec3));
				memcpy(&vertex.vNormal, &srcMesh->mNormals[j], sizeof(Vec3));
				memcpy(&vertex.vTexture, &srcMesh->mTextureCoords[0][j], sizeof(Vec2));
				memcpy(&vertex.vTangent, &srcMesh->mTangents[j], sizeof(Vec3));
				
				mesh->verticesStatic.push_back(vertex);	
			}
		}
		/* Vertices - Amim */
		else if (modelType == MODEL_TYPE::ANIM)
		{
			mesh->verticesAnim.reserve(srcMesh->mNumVertices);
			for (size_t j = 0; j < mesh->verticesAnim.size(); j++)
				mesh->verticesAnim.push_back(VTXANIM{});

			for (uint j = 0; j < mesh->verticesAnim.size(); j++)
			{
				memcpy(&mesh->verticesAnim[j].vPosition, &srcMesh->mVertices[j], sizeof(Vec3));
				memcpy(&mesh->verticesAnim[j].vNormal, &srcMesh->mNormals[j], sizeof(Vec3));
				memcpy(&mesh->verticesAnim[j].vTexture, &srcMesh->mTextureCoords[0][j], sizeof(Vec2));
				memcpy(&mesh->verticesAnim[j].vTangent, &srcMesh->mTangents[j], sizeof(Vec3));

				/* Static과 달리 해당 메시에 영향을 주는 뼈의 정보를 저장한다. */
				for (uint k = 0; k < srcMesh->mNumBones; ++k)
				{
					aiBone* pAIBone = srcMesh->mBones[k];

					for (uint m = 0; m < pAIBone->mNumWeights; ++m)
					{
						uint		iVertexIndex = pAIBone->mWeights[m].mVertexId;

						if (0.0f == mesh->verticesAnim[iVertexIndex].vBlendWeight.x)
						{
							mesh->verticesAnim[iVertexIndex].vBlendIndex.x = k;
							mesh->verticesAnim[iVertexIndex].vBlendWeight.x = pAIBone->mWeights[m].mWeight;
						}

						else if (0.0f == mesh->verticesAnim[iVertexIndex].vBlendWeight.y)
						{
							mesh->verticesAnim[iVertexIndex].vBlendIndex.y = k;
							mesh->verticesAnim[iVertexIndex].vBlendWeight.y = pAIBone->mWeights[m].mWeight;
						}

						else if (0.0f == mesh->verticesAnim[iVertexIndex].vBlendWeight.z)
						{
							mesh->verticesAnim[iVertexIndex].vBlendIndex.z = k;
							mesh->verticesAnim[iVertexIndex].vBlendWeight.z = pAIBone->mWeights[m].mWeight;
						}

						else if (0.0f == mesh->verticesAnim[iVertexIndex].vBlendWeight.w)
						{
							mesh->verticesAnim[iVertexIndex].vBlendIndex.w = k;
							mesh->verticesAnim[iVertexIndex].vBlendWeight.w = pAIBone->mWeights[m].mWeight;
						}
					}
				}
			}
		}
		/* Indices*/
		mesh->indices.reserve(srcMesh->mNumFaces * 3);
		for (uint j = 0; j < srcMesh->mNumFaces; j++)
		{
			aiFace& face = srcMesh->mFaces[j];

			for (uint  k = 0; k < 3; k++)
			{
				mesh->indices.push_back(face.mIndices[k]);
			}
		}
		_meshes.push_back(mesh);

		/* Bones (현재 메시에 영향을 주는 뼈들을 순회하며 행렬정보를 저장하고 뼈들을 컨테이너에 모아둔다. */
		uint numBones = srcMesh->mNumBones;

		for (uint j = 0; j < numBones; j++)
		{
			aiBone* srcMeshBone = srcMesh->mBones[j];

			Matrix offsetMatrix;
			memcpy(&offsetMatrix, &srcMeshBone->mOffsetMatrix, sizeof(Matrix));

			uint32 boneIndex = Get_BoneIndex(srcMeshBone->mName.C_Str());
			_bones[boneIndex]->offsetTransform = offsetMatrix.Transpose();

			mesh->bones.push_back(boneIndex);
		}
	}

	return S_OK;
}

HRESULT CConverter::Write_MeshData(wstring savePath)
{


	return S_OK;
}

HRESULT CConverter::Read_MaterialData()
{
	/* 모델이 가지고 있는 매태리얼을 순회한다. */
	for (uint32 i = 0; i < _scene->mNumMaterials; i++)
	{
		aiMaterial* srcMaterial = _scene->mMaterials[i];
		shared_ptr<asMaterial> material = make_shared<asMaterial>();
		{
			/* 매태리얼이 사용하는 텍스처의 경로를 저장한다. */
			aiString file;
			{
				srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
				material->diffuseFilePath = file.C_Str();

				srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
				material->specularFilePath = file.C_Str();

				srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
				material->normalFilePath = file.C_Str();
			}

		}
		_materials.push_back(material);
	}
	return S_OK;
}

HRESULT CConverter::Write_MaterialData(wstring savePath)
{
	/* 매태리얼이 사용하는 텍스처 파일을 srcPath에서 destPath로 복사한다. */

	for (shared_ptr<asMaterial> material : _materials)
	{
		string destPath;

		::CopyFileA(material->diffuseFilePath.c_str(), destPath.c_str(), false);
		::CopyFileA(material->normalFilePath.c_str(), destPath.c_str(), false);
		::CopyFileA(material->specularFilePath.c_str(), destPath.c_str(), false);
	}
	return S_OK;
}

HRESULT CConverter::Read_AnimData()
{
	/* 모델이 들고 있는 모든 애니메이션 순회 */
	for (uint32 i = 0; i < _scene->mNumAnimations; i++)
	{
		aiAnimation* srcAnimation = _scene->mAnimations[i];
		shared_ptr<asAnimation> animation = make_shared<asAnimation>();

		animation->fDuration = srcAnimation->mDuration;
		animation->fTickPerSecond = srcAnimation->mTicksPerSecond;

		/* 현재 애니메이션의 모든 채널 순회*/
		for (uint j = 0; j < srcAnimation->mNumChannels; j++)
		{
			shared_ptr<asChannel> channel = make_shared<asChannel>();
			aiNodeAnim* srcNode = srcAnimation->mChannels[i];

			channel->name = srcNode->mNodeName.data;

			uint numKeyframes = max(srcNode->mNumScalingKeys, srcNode->mNumRotationKeys);
			numKeyframes = max(numKeyframes, srcNode->mNumPositionKeys);

			Vec3			vScale{};
			Vec4			vRotation{};
			Vec3			vPosition{};

			/* 현재 채널의 모든 키프레임 순회 */
			for (uint k = 0; k < numKeyframes; k++)
			{
				shared_ptr<asKeyFrame>	keyframe = make_shared<asKeyFrame>();

				if (i < srcNode->mNumScalingKeys)
				{
					memcpy(&vScale, &srcNode->mScalingKeys[i].mValue, sizeof(Vec3));
					keyframe->fTime = srcNode->mScalingKeys[i].mTime;
				}
				if (i < srcNode->mNumRotationKeys)
				{
					aiQuatKey key = srcNode->mRotationKeys[k];

					vRotation.x = key.mValue.x;
					vRotation.y = key.mValue.y;
					vRotation.z = key.mValue.z;
					vRotation.w = key.mValue.w;
					keyframe->fTime = (float)key.mTime;
				}
				if (i < srcNode->mNumPositionKeys)
				{
					memcpy(&vPosition, &srcNode->mPositionKeys[i].mValue, sizeof(Vec3));
					keyframe->fTime = srcNode->mPositionKeys[i].mTime;
				}

				keyframe->vScale = vScale;
				keyframe->vRotation = vRotation;
				keyframe->vPosition = vPosition;

				channel->keyframes.push_back(keyframe);
			}
			animation->channels.push_back(channel);
		}
		_animations.push_back(animation);
	}
	return S_OK;
}

HRESULT CConverter::Write_AnimData(wstring savePath)
{
	return S_OK;
}

uint32 CConverter::Get_BoneIndex(const string& name)
{
	for (shared_ptr<asBone>& bone : _bones)
	{
		if (bone->name == name)
			return bone->index;
	}

	assert(false);
	return 0;
}


HRESULT CConverter::Read_BoneData(aiNode* node, int32 index, int32 parent, int32 depth)
{
	shared_ptr<asBone> bone = make_shared<asBone>();
	{
		bone->name = node->mName.C_Str(); /* 노드 이름 = 뼈 이름 */

		bone->parent = parent;
		bone->index = index;
		bone->depth = ++depth;
	
		Matrix transform(node->mTransformation[0]);
		bone->transform = transform.Transpose();
	}

	_bones.push_back(bone);

	for (uint i = 0; i < node->mNumChildren; i++)
		Read_BoneData(node->mChildren[i], (int32)_bones.size(), index, bone->depth);

	return S_OK;
}

HRESULT CConverter::Write_BoneData(wstring savePath)
{
	return S_OK;
}
