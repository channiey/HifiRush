#include "ConverterCustom.h"

CConverterCustom::CConverterCustom()
{
}

CConverterCustom::~CConverterCustom()
{
}

HRESULT CConverterCustom::Binarize_Model(string fileName, vector<string> meshFileNames, string savePath, const MODEL_TYPE& modelType)
{
	string fileModelPath{};
	vector <string> fileMeshPaths;
	fileMeshPaths.resize(meshFileNames.size());

	/* Read Asset Data */
	{
		fileModelPath = (filesystem::path(sourceUpperPath + savePath) / fileName).string() + ".fbx";
		Utils_String::Replace(fileModelPath, "\\", "/");

		if (!Utils_File::IsExistFile(fileModelPath))
			return E_FAIL;

		for (size_t i = 0; i < meshFileNames.size(); i++)
		{
			fileMeshPaths[i] = (filesystem::path(sourceUpperPath + savePath) / meshFileNames[i]).string() + ".fbx";
			Utils_String::Replace(fileMeshPaths[i], "\\", "/");

			if (!Utils_File::IsExistFile(fileMeshPaths[i]))
				return E_FAIL;
		}

		if (FAILED(Read_AssetFile(fileModelPath, fileMeshPaths, modelType)))
			return E_FAIL;
	}

	/* Export Bone Data */
	{
		Read_BoneData(_sceneModel->mRootNode, 0, -1, 0);
	}


	/* Export Mesh Data */
	{
		fileModelPath = (filesystem::path(destUpperPath + savePath) / fileName).string() + ".mesh";
		Utils_String::Replace(fileModelPath, "\\", "/");
		Utils_File::CheckOrCreatePath(fileModelPath);

		if (FAILED(Export_MeshData(fileModelPath, modelType)))
			return E_FAIL;

		/* 메시에 영향주는 본들의 작업을 마치고 본을 익스포트 해야한다. */
		{
			fileModelPath = (filesystem::path(destUpperPath + savePath) / fileName).string() + ".bone";
			Utils_String::Replace(fileModelPath, "\\", "/");
			Utils_File::CheckOrCreatePath(fileModelPath);

			if (FAILED(Write_BoneData(fileModelPath)))
				return E_FAIL;
		}
	}

	/* Export Materail Data */
	{
		/* Src Path */
		string srcPath = filesystem::path(sourceUpperPath + savePath).string();

		if (!Utils_File::IsExistFile(srcPath))
			return E_FAIL;

		/* Save Path */
		fileModelPath = filesystem::path(destUpperPath + savePath).string();
		Utils_File::CheckOrCreatePath(fileModelPath);

		if (FAILED(Export_MaterialData(fileName, srcPath, fileModelPath)))
			return E_FAIL;
	}

	/* Export Anim Data */
	if (MODEL_TYPE::ANIM == modelType)
	{
		fileModelPath = (filesystem::path(destUpperPath + savePath) / fileName).string() + ".anim";
		Utils_String::Replace(fileModelPath, "\\", "/");
		Utils_File::CheckOrCreatePath(fileModelPath);

		if (FAILED(Export_AnimData(fileModelPath)))
			return E_FAIL;
	}


	cout << "Complete (Flie Name : " << fileName + ".fbx)" << endl;
	return S_OK;
}

HRESULT CConverterCustom::Read_AssetFile(string modelPath, vector<string> meshePaths, const MODEL_TYPE& modelType)
{
	if (MODEL_TYPE::TYPEEND == modelType)
		return E_FAIL;

	/* Flag */
	int	iFlag = 0;
	{
		if (MODEL_TYPE::STATIC == modelType)
			iFlag |= aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace;
		else
			iFlag |= aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace;
	}

	/* Model */
	{
		_importerModel = make_shared<Assimp::Importer>();

		_sceneModel = _importerModel->ReadFile(modelPath, iFlag);

		if (nullptr == _sceneModel)
			ASSERT_LOG();
	}

	/* Meshes */
	_sceneMeshes.resize(meshePaths.size());
	for (size_t i = 0; i < meshePaths.size(); i++)
	{
		shared_ptr<Assimp::Importer>	importer = make_shared<Assimp::Importer>();
		_importerMeshes.push_back(importer);

		_sceneMeshes[i] = _importerMeshes[i]->ReadFile(meshePaths[i], iFlag);


		if (nullptr == _sceneMeshes[i])
			ASSERT_LOG();
	}

	return S_OK;
}

HRESULT CConverterCustom::Export_MeshData(string savePath, const MODEL_TYPE& modelType)
{
	if (FAILED(Read_OffsetMatrix()))
		return E_FAIL;

	if (FAILED(Read_MeshData(modelType)))
		return E_FAIL;

	if (FAILED(Write_MeshData(savePath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CConverterCustom::Export_MaterialData(string fileName, string srcPath, string savePath)
{
	if (FAILED(Read_MaterialData()))
		ASSERT_LOG();

	if (FAILED(Write_MaterialData(fileName, srcPath, savePath)))
		ASSERT_LOG();

	return S_OK;
}

HRESULT CConverterCustom::Export_AnimData(string savePath)
{
	if (FAILED(Read_AnimData()))
		return E_FAIL;

	if (FAILED(Write_AnimData(savePath)))
		return E_FAIL;

	return S_OK;
}

void CConverterCustom::Read_BoneData(aiNode* node, int32 index, int32 parent, int32 depth)
{
	shared_ptr<asBone> bone = make_shared<asBone>();
	{
		bone->name = node->mName.C_Str(); 

		bone->parent = parent;
		bone->index = index;
		bone->depth = depth;

		Matrix transform(node->mTransformation[0]);
		bone->transform = transform.Transpose();
	}

	_bones.push_back(bone);

	for (uint i = 0; i < node->mNumChildren; i++)
		Read_BoneData(node->mChildren[i], (int32)_bones.size(), index, bone->depth + 1);
}

HRESULT CConverterCustom::Write_BoneData(string savePath)
{
	shared_ptr<Utils_File> file = make_shared<Utils_File>();
	file->Open(Utils_String::ToWString(savePath), FileMode::Write);

	file->Write<size_t>(_bones.size());
	for (shared_ptr<asBone>& bone : _bones)
	{
		file->Write<string>(bone->name);
		file->Write<Matrix>(bone->transform);
		file->Write<Matrix>(bone->offsetTransform);
		file->Write<int32>(bone->index);
		file->Write<int32>(bone->parent);
		file->Write<uint>(bone->depth);
	}

	return S_OK;
}

HRESULT CConverterCustom::Read_MeshData(MODEL_TYPE modelType)
{
	uint iPrevMeshMatCount = 0;

	for (size_t iMeshCount = 0; iMeshCount < _sceneMeshes.size(); iMeshCount++)
	{
		auto& _scene = _sceneMeshes[iMeshCount];

		for (uint i = 0; i < _scene->mNumMeshes; i++)
		{
			shared_ptr<asMesh> mesh = make_shared<asMesh>();
			mesh->name = _scene->mMeshes[i]->mName.data;

			const aiMesh* srcMesh = _scene->mMeshes[i];

			/* Vertices*/
			if (modelType == MODEL_TYPE::ANIM)
			{
				mesh->isAinm = (UINT)modelType;

				mesh->verticesAnim.reserve(srcMesh->mNumVertices);
				for (size_t j = 0; j < srcMesh->mNumVertices; j++)
					mesh->verticesAnim.push_back(VTXANIM{});

				for (uint j = 0; j < mesh->verticesAnim.size(); j++)
				{
					memcpy(&mesh->verticesAnim[j].vPosition, &srcMesh->mVertices[j], sizeof(Vec3));
					memcpy(&mesh->verticesAnim[j].vNormal, &srcMesh->mNormals[j], sizeof(Vec3));
					memcpy(&mesh->verticesAnim[j].vTexture, &srcMesh->mTextureCoords[0][j], sizeof(Vec2));
					memcpy(&mesh->verticesAnim[j].vTangent, &srcMesh->mTangents[j], sizeof(Vec3));
				}

				for (uint j = 0; j < srcMesh->mNumBones; ++j)
				{
					aiBone* pAIBone = srcMesh->mBones[j];

					for (uint k = 0; k < pAIBone->mNumWeights; ++k)
					{
						uint		iVertexIndex = pAIBone->mWeights[k].mVertexId;

						if (0.0f == mesh->verticesAnim[iVertexIndex].vBlendWeight.x)
						{
							mesh->verticesAnim[iVertexIndex].vBlendIndex.x = Get_BoneIndex(pAIBone->mName.C_Str());
							mesh->verticesAnim[iVertexIndex].vBlendWeight.x = pAIBone->mWeights[k].mWeight;
						}

						else if (0.0f == mesh->verticesAnim[iVertexIndex].vBlendWeight.y)
						{
							mesh->verticesAnim[iVertexIndex].vBlendIndex.y = Get_BoneIndex(pAIBone->mName.C_Str());
							mesh->verticesAnim[iVertexIndex].vBlendWeight.y = pAIBone->mWeights[k].mWeight;
						}

						else if (0.0f == mesh->verticesAnim[iVertexIndex].vBlendWeight.z)
						{
							mesh->verticesAnim[iVertexIndex].vBlendIndex.z = Get_BoneIndex(pAIBone->mName.C_Str());
							mesh->verticesAnim[iVertexIndex].vBlendWeight.z = pAIBone->mWeights[k].mWeight;
						}

						else if (0.0f == mesh->verticesAnim[iVertexIndex].vBlendWeight.w)
						{
							mesh->verticesAnim[iVertexIndex].vBlendIndex.w = Get_BoneIndex(pAIBone->mName.C_Str());
							mesh->verticesAnim[iVertexIndex].vBlendWeight.w = pAIBone->mWeights[k].mWeight;
						}
					}
				}
			}

			/* Indices*/
			mesh->indices.reserve(srcMesh->mNumFaces * 3);
			for (uint j = 0; j < srcMesh->mNumFaces; j++)
			{
				aiFace& face = srcMesh->mFaces[j];

				for (uint k = 0; k < 3; k++)
				{
					mesh->indices.push_back(face.mIndices[k]);
				}
			}

			/* Material Index */
			mesh->materialIndex = (iPrevMeshMatCount) + srcMesh->mMaterialIndex;

			/* Bones (현재 메시에 영향을 주는 뼈들을 순회하며 행렬 정보를 저장하고 뼈들을 컨테이너에 모아둔다. */
			for (uint j = 0; j < srcMesh->mNumBones; j++)
			{
				aiBone* srcMeshBone = srcMesh->mBones[j];

				Matrix offsetMatrix;
				memcpy(&offsetMatrix, &srcMeshBone->mOffsetMatrix, sizeof(Matrix));

				uint32 boneIndex = Get_BoneIndex(srcMeshBone->mName.C_Str());

				mesh->bones.push_back(boneIndex);
			}

			_meshes.push_back(mesh);
		}

		iPrevMeshMatCount += _scene->mNumMaterials;
	}
	
	return S_OK;
}

HRESULT CConverterCustom::Read_OffsetMatrix()
{
	/* 원본 모델의 오프셋 매트릭스 정보를 가져온다. */

	for (uint i = 0; i < _sceneModel->mNumMeshes; i++)
	{
		const aiMesh* srcMesh = _sceneModel->mMeshes[i];

		/* 원본 모델 현재 메시에 영향을 주는 뼈들의 오프셋 매트릭스 정보를 저장한다 . */
		uint numBones = srcMesh->mNumBones;

		for (uint j = 0; j < numBones; j++)
		{
			aiBone* srcMeshBone = srcMesh->mBones[j];
			uint32	boneIndex = Get_BoneIndex(srcMeshBone->mName.C_Str());

			Matrix offsetMatrix;
			memcpy(&offsetMatrix, &srcMeshBone->mOffsetMatrix, sizeof(Matrix));

			_bones[boneIndex]->offsetTransform = offsetMatrix.Transpose();
		}
	}

	return S_OK;
}

HRESULT CConverterCustom::Write_MeshData(string savePath)
{
	shared_ptr<Utils_File> file = make_shared<Utils_File>();
	file->Open(Utils_String::ToWString(savePath), FileMode::Write);

	file->Write<size_t>(_meshes.size());
	for (shared_ptr<asMesh>& mesh : _meshes)
	{
		/* name */
		file->Write<string>(mesh->name);

		/* isAnim */
		file->Write<bool>(mesh->isAinm);

		/* Vertices */
		file->Write<size_t>(mesh->verticesAnim.size());
		for (VTXANIM& vertex : mesh->verticesAnim)
		{
			file->Write<Vec3>(vertex.vPosition);
			file->Write<Vec3>(vertex.vNormal);
			file->Write<Vec2>(vertex.vTexture);
			file->Write<Vec3>(vertex.vTangent);
			file->Write<XMUINT4>(vertex.vBlendIndex);
			file->Write<Vec4>(vertex.vBlendWeight);
		}

		/* Indices */
		file->Write<size_t>(mesh->indices.size());
		for (int& index : mesh->indices)
		{
			file->Write<int>(index);
		}

		/* Material Index */
		file->Write<uint>(mesh->materialIndex);

		/* Bone Indices */
		file->Write<size_t>(mesh->bones.size());
		for (int& index : mesh->bones)
		{
			file->Write<int>(index);
		}
	}
	return S_OK;
}

HRESULT CConverterCustom::Read_MaterialData()
{
	for (auto& _scene : _sceneMeshes)
	{
		/* 모델이 가지고 있는 매태리얼을 순회한다. */
		for (uint32 i = 0; i < _scene->mNumMaterials; i++)
		{
			aiMaterial* srcMaterial = _scene->mMaterials[i];
			shared_ptr<asMaterial> material = make_shared<asMaterial>();
			{
				/* 매태리얼이 사용하는 텍스처의 경로를 저장한다. */
				aiString file;
				string name;
				{
					srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
					name = file.C_Str();
					material->diffuseFilePath	= filesystem::path(name).filename().string();

					srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
					name = file.C_Str();
					material->specularFilePath	= filesystem::path(name).filename().string();

					srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
					name = file.C_Str();
					material->normalFilePath	= filesystem::path(name).filename().string();
				}
			}
			_materials.push_back(material);
		}
	}
	return S_OK;
}

HRESULT CConverterCustom::Write_MaterialData(string fileName, string srcPath, string savePath)
{
	string path;
	string finalSrcPath;
	string finalSavePath;

	/* 텍스처 복사 */

	int i = 0;
	for (shared_ptr<asMaterial> material : _materials)
	{
		/* Chai Arm 매티리얼 누락되는 현상 해결하기 위한 임시조치 */
		if (22 <= i)
		{
			/* Diffuse */
			finalSrcPath = srcPath + "/" + "T_cc0701_MetalArm_diffuse.png";
			finalSrcPath = filesystem::absolute(finalSrcPath).string();
			finalSavePath = filesystem::absolute(savePath).string();
			if (!Utils_File::IsExistFile(finalSavePath + "\\" + "T_cc0701_MetalArm_diffuse.png"))
				filesystem::copy(filesystem::path(finalSrcPath), filesystem::path(finalSavePath));
		}
		else
		{
			/* Diffuse */
			finalSrcPath = srcPath + "/" + material->diffuseFilePath;
			finalSrcPath = filesystem::absolute(finalSrcPath).string();
			finalSavePath = filesystem::absolute(savePath).string();
			if (!Utils_File::IsExistFile(finalSavePath + "\\" + material->diffuseFilePath))
				filesystem::copy(filesystem::path(finalSrcPath), filesystem::path(finalSavePath));

		}

		/* Normal */
		finalSrcPath = srcPath + "/" + material->normalFilePath;
		finalSrcPath = filesystem::absolute(finalSrcPath).string();
		finalSavePath = filesystem::absolute(savePath).string();
		if (!Utils_File::IsExistFile(finalSavePath + "\\" + material->normalFilePath))
			filesystem::copy(filesystem::path(finalSrcPath), filesystem::path(finalSavePath));

		/* Specular */
		finalSrcPath = srcPath + "/" + material->specularFilePath;
		finalSrcPath = filesystem::absolute(finalSrcPath).string();
		finalSavePath = filesystem::absolute(savePath).string();
		if (!Utils_File::IsExistFile(finalSavePath + "\\" + material->specularFilePath))
			filesystem::copy(filesystem::path(finalSrcPath), filesystem::path(finalSavePath));

		i++;
	}

	/* 매태리얼 정보 저장 */
	{
		shared_ptr<Utils_File> file = make_shared<Utils_File>();

		string filePath = (filesystem::path(savePath) / string(fileName + ".mat")).string();
		Utils_String::Replace(filePath, "\\", "/");
		file->Open(Utils_String::ToWString(filePath), FileMode::Write);

		file->Write<size_t>(_materials.size());

		int i = 0;
		for (shared_ptr<asMaterial> material : _materials)
		{
			/* Chai Arm 매티리얼 누락되는 현상 해결하기 위한 임시조치 */
			if (22 <= i)
			{
				file->Write<string>("T_cc0701_MetalArm_diffuse.png");
			}
			else
				file->Write<string>(material->diffuseFilePath);

			file->Write<string>(material->specularFilePath);
			file->Write<string>(material->normalFilePath);
			i++;
		}
	}

	return S_OK;
}

HRESULT CConverterCustom::Read_AnimData()
{
	/* 모델이 들고 있는 모든 애니메이션 순회 */
	for (uint32 i = 0; i < _sceneModel->mNumAnimations; i++)
	{
		aiAnimation* srcAnimation = _sceneModel->mAnimations[i];
		shared_ptr<asAnimation> animation = make_shared<asAnimation>();

		animation->name = srcAnimation->mName.C_Str();
		animation->fDuration = srcAnimation->mDuration;
		animation->fTickPerSecond = srcAnimation->mTicksPerSecond;

		/* 현재 애니메이션의 모든 채널 순회*/
		for (uint j = 0; j < srcAnimation->mNumChannels; j++)
		{
			shared_ptr<asChannel> channel = make_shared<asChannel>();
			aiNodeAnim* srcNode = srcAnimation->mChannels[j];

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

				if (k < srcNode->mNumScalingKeys)
				{
					memcpy(&vScale, &srcNode->mScalingKeys[k].mValue, sizeof(Vec3));
					keyframe->fTime = srcNode->mScalingKeys[k].mTime;
				}
				if (k < srcNode->mNumRotationKeys)
				{
					aiQuatKey key = srcNode->mRotationKeys[k];

					vRotation.x = key.mValue.x;
					vRotation.y = key.mValue.y;
					vRotation.z = key.mValue.z;
					vRotation.w = key.mValue.w;
					keyframe->fTime = (float)key.mTime;
				}
				if (k < srcNode->mNumPositionKeys)
				{
					memcpy(&vPosition, &srcNode->mPositionKeys[k].mValue, sizeof(Vec3));
					keyframe->fTime = srcNode->mPositionKeys[k].mTime;
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

HRESULT CConverterCustom::Write_AnimData(string savePath)
{
	shared_ptr<Utils_File> file = make_shared<Utils_File>();
	file->Open(Utils_String::ToWString(savePath), FileMode::Write);

	file->Write<size_t>(_animations.size());
	for (shared_ptr<asAnimation>& animation : _animations)
	{
		file->Write<string>(animation->name);
		file->Write<float>(animation->fDuration);
		file->Write<float>(animation->fTickPerSecond);

		file->Write<size_t>(animation->channels.size());
		for (shared_ptr<asChannel>& channel : animation->channels)
		{
			file->Write<string>(channel->name);

			file->Write<size_t>(channel->keyframes.size());
			for (shared_ptr<asKeyFrame>& keyframe : channel->keyframes)
			{
				file->Write<float>(keyframe->fTime);
				file->Write<Vec3>(keyframe->vScale);
				file->Write<Vec4>(keyframe->vRotation);
				file->Write<Vec3>(keyframe->vPosition);
			}
		}
	}
	return S_OK;
}

uint32 CConverterCustom::Get_BoneIndex(const string& name)
{
	for (shared_ptr<asBone>& bone : _bones)
	{
		if (bone->name == name)
			return bone->index;
	}

	assert(false);
	return 0;
}

int32 CConverterCustom::Get_BoneIndex(const char* name)
{
	for (int32 i = 0; i < _bones.size(); ++i)
	{
		if (!strcmp(_bones[i]->name.c_str(), name))
			return i;
	}
	assert(false);
	return 0;
}