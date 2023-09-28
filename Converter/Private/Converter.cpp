#include "Converter.h"
#include "Utils_String.h"
#include "Utils_File.h"

CConverter::CConverter()
{
}

CConverter::~CConverter()
{
}

HRESULT CConverter::Read_AssetFile(wstring filePath, const MODEL_TYPE modelType)
{
	if (MODEL_TYPE::TYPEEND == modelType)
		return E_FAIL;

	m_eModelType = modelType;

	int	iFlag = 0;
	{
		if (MODEL_TYPE::STATIC == m_eModelType)
			iFlag |= aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace;
		else
			iFlag |= aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace;
	}

	m_pAiScene = m_pImporter->ReadFile(Utils_String::ToString(filePath), iFlag);
	{
		if (nullptr == m_pAiScene)
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

HRESULT CConverter::Read_MeshData()
{
	if (nullptr == m_pAiScene)
		return E_FAIL;

	/* 모델이 가진 메시를 전부 순회하며 메시 구조체를 만들어 m_Meshes에 푸시한다. */
	for (uint i = 0; i < m_pAiScene->mNumMeshes; i++)
	{
		shared_ptr<asMesh> mesh = make_shared<asMesh>();
		mesh->name = m_pAiScene->mMeshes[i]->mName.data;

		const aiMesh* srcMesh = m_pAiScene->mMeshes[i];

		/* Vertices - Static */ 
		if (m_eModelType == MODEL_TYPE::STATIC) 
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
		else if (m_eModelType == MODEL_TYPE::ANIM)
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
		m_Meshes.push_back(mesh);
	}

	return S_OK;
}

HRESULT CConverter::Write_MeshData(wstring savePath)
{


	return S_OK;
}

HRESULT CConverter::Read_MaterialData()
{
	return S_OK;
}

HRESULT CConverter::Write_MaterialData(wstring savePath)
{
	return S_OK;
}
