#include "..\Public\Model.h"
#include "MeshContainer.h"
#include "Texture.h"
#include "HierarchyNode.h"
#include "Animation.h"
#include "Shader.h"


CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, m_pAIScene(rhs.m_pAIScene)
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
	for (auto& pMeshContainer : m_Meshes)
		Safe_AddRef(pMeshContainer);


	for (auto& Material : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_AddRef(Material.pTexture[i]);
	}

	for (auto& pAnimation : m_Animations)
		Safe_AddRef(pAnimation);

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

_uint CModel::Get_MaterialIndex(_uint iMeshIndex)
{
	return m_Meshes[iMeshIndex]->Get_MaterialIndex();
}

HRESULT CModel::Initialize_Prototype(TYPE eType, const char * pModelFilePath, const char * pModelFileName, _fmatrix PivotMatrix)
{
	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

	char		szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, pModelFilePath);
	strcat_s(szFullPath, pModelFileName);

	_uint		iFlag = 0;

	m_eModelType = eType;

	// aiProcess_PreTransformVertices : 모델을 구성하는 메시 중, 이 메시의 이름과 뼈의 이름이 같은 상황이라면 이 뼈의 행렬을 메시의 정점에 다 곱해서 로드한다. 
	// 모든 애니메이션 정보는 폐기된다. 
	if (TYPE_NONANIM == eType)
		iFlag |= aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace;
	else
		iFlag |= aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace;

	m_pAIScene = m_Importer.ReadFile(szFullPath, iFlag);

	if (nullptr == m_pAIScene)
		return E_FAIL;	

	if (FAILED(Ready_MeshContainers(PivotMatrix)))
		return E_FAIL;

	if (FAILED(Ready_Materials(pModelFilePath)))
		return E_FAIL;

	if (FAILED(Ready_Animations()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Initialize(void * pArg)
{
	/* 이 모델 전체의 뼈의 정보(aiNode)를 로드한다. (단순 노드 연결) */
	Ready_HierarchyNodes(m_pAIScene->mRootNode, nullptr, 0);

	/* Deep Copy + SetUp Hierarchy (노드별 본과 메시의 연결 및 세팅) */
	if (TYPE_ANIM == m_eModelType)
	{
		_uint		iNumMeshes = 0;

		vector<CMeshContainer*>		MeshContainers; 

		for (auto& pPrototype : m_Meshes)
		{
			CMeshContainer*		pMeshContainer = (CMeshContainer*)pPrototype->Clone();
			if (nullptr == pMeshContainer)
				return E_FAIL;

			MeshContainers.push_back(pMeshContainer);

			Safe_Release(pPrototype);
		}

		m_Meshes.clear();

		m_Meshes = MeshContainers;

		/* 메시를 순회하며, 메시에 대한 뼈 정보를 노드에 세팅한다. */
		for (auto& pMeshContainer : m_Meshes)
		{
			if (nullptr != pMeshContainer)
				pMeshContainer->SetUp_HierarchyNodes(this, m_pAIScene->mMeshes[iNumMeshes++]);
		}
	}

	/* Deep Copy */
	vector<CAnimation*>		Animations;
	for (auto& pPrototype : m_Animations)
	{
		CAnimation*		pAnimation = pPrototype->Clone(this);
		if (nullptr == pAnimation)
			return E_FAIL;

		Animations.push_back(pAnimation);

		Safe_Release(pPrototype);
	}

	m_Animations.clear();

	m_Animations = Animations;

	return S_OK;
}

HRESULT CModel::SetUp_OnShader(CShader * pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char * pConstantName)
{
	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;
	
	if (m_Materials[iMaterialIndex].pTexture[eTextureType] == nullptr)
		return S_OK;

	return m_Materials[iMaterialIndex].pTexture[eTextureType]->Bind_ShaderResource(pShader, pConstantName, 0);
}

HRESULT CModel::Play_Animation(_float fTimeDelta)
{
	/* 애니메이션을 재생한다. */
	/* 1. 해당 애니메이션에서 사용하는 모든 뼈들의 Transformation 행렬을 갱신한다. */
	/* 2. Transformation을 최상위 부모로부터 자식으로 계속 누적시켜간다.(CombinedTransformation) */
	/* 3. 애니메이션에 의해 움직인 뼈들의 CombinedTransfromation을 세팅한다. */

	if (m_iCurrentAnimIndex >= m_iNumAnimations) return E_FAIL;
	/* Relative */
	/* 현재 재생하고자하는 애니메이션이 제어해야할 뼈들의 지역행렬을 갱신해낸다. */
	m_Animations[m_iCurrentAnimIndex]->Play_Animation(fTimeDelta);

	/* Global */
	/* 지역행렬을 순차적으로(부모에서 자식으로) 누적하여 m_CombinedTransformation를 만든다. */
	/* m_HierarchyNodes은 부모에서 자식 순으로 순차적으로 정렬되어 있는 상태다. */
	for (auto& pHierarchyNode : m_HierarchyNodes)
		pHierarchyNode->Set_CombinedTransformation();

	return S_OK;
}

HRESULT CModel::Render(CShader* pShader, _uint iMeshIndex, _uint iPassIndex)
{
	_float4x4		BoneMatrices[600];

	if (TYPE_ANIM == m_eModelType) 
	{
		m_Meshes[iMeshIndex]->SetUp_BoneMatrices(BoneMatrices, XMLoadFloat4x4(&m_PivotMatrix));

		/* 모델 정점의 스키닝. */
   		if (FAILED(pShader->Bind_RawValue("g_BoneMatrices", BoneMatrices, sizeof(_float4x4) * 600)))
			return E_FAIL;
	}

	pShader->Begin(0);
	
	m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

HRESULT CModel::Ready_MeshContainers(_fmatrix PivotMatrix)
{
	/* 모델 -> 메시(여러개) -> 각 메시의 정점, 인덱스*/
	
	/* 모델을 구성하는 메시들을 만든다. */
	/* 모델은 여러개의 메시로 구성되어있다. */
	/* 각 메시의 정점들과 인덱스들을 구성한다. */

	/* 메시의 갯수를 얻어온다. */
	m_iNumMeshes = m_pAIScene->mNumMeshes;
	
	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		CMeshContainer*		pMeshContainer = CMeshContainer::Create(m_pDevice, m_pContext, m_eModelType, m_pAIScene->mMeshes[i], this, PivotMatrix);
		if (nullptr == pMeshContainer)
			return E_FAIL;

		m_Meshes.push_back(pMeshContainer);
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(const char* pModelFilePath)
{
	/* 머테리얼정보다.(빛을 받았을때 리턴해야할 색상정보.) */
	/* 모델마다정의?, 정점마다정의? 픽셀마다 정의(o) 텍스쳐로 표현된다. */

	if (nullptr == m_pAIScene)
		return E_FAIL;

	/* 이 모델은 몇개의 머테리얼 정보를 이용하는가. */
	/* 머테리얼(MATERIALDESC) : 텍스쳐[디퓨즈or앰비언트or노말or이미시브 등등등 ] */
	m_iNumMaterials = m_pAIScene->mNumMaterials;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		MATERIALDESC		MaterialDesc;
		ZeroMemory(&MaterialDesc, sizeof(MATERIALDESC));

		aiMaterial*			pAIMaterial = m_pAIScene->mMaterials[i];

		/* AI_TEXTURE_TYPE_MAX:디퓨즈or앰비언트or노말or이미시브 등등등 */
		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{			
			aiString		strPath;			

			/* 해당 재질을 표현하기위한 텍스쳐의 경로를 strPath에 받아온다. */
			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strPath)))
				continue;		

			char			szFullPath[MAX_PATH] = "";
			char			szFileName[MAX_PATH] = "";
			char			szExt[MAX_PATH] = "";

			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			strcpy_s(szFullPath, pModelFilePath);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szExt);

			_tchar			szWideFullPath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szFullPath, _int(strlen(szFullPath)), szWideFullPath, MAX_PATH);

			/* 최종적으로 텍스처를 생성한다. */
			MaterialDesc.pTexture[j] = CTexture::Create(m_pDevice, m_pContext, szWideFullPath);
			if (nullptr == MaterialDesc.pTexture[j])
				return E_FAIL;			
		}	

		m_Materials.push_back(MaterialDesc);
	}

	return S_OK;
}

HRESULT CModel::Ready_HierarchyNodes(aiNode* pNode, CHierarchyNode* pParent, _uint iDepth)
{
	/* pParent? : 부모 노드 주소. CombinedTransformation으로 그린다. CombinedTransformation을 만들려면 나의 Transformation * 부모의 CombinedTranfsormation로 만든다. */
	/* 글로벌을 표현하는 것 같다. */
	/* 이후 애니메이션 재생할때 뼈의 CombinedTransformation를 만든다. */
	
	/* CombinedTransformation만들려면 부모의 상태가 모두 갱신되어있어야돼. 왜 부모의 컴바인드 이용하니까 */
	/* 부모부터 자식으로 순차적으로 CombinedTransformation를 만들어야한다라는 걸 의미.* /
	/* 따라서 m_HierarchyNodes 컨테이너는 최상위 부모가 가장 앞에 있어야한다. 이놈의 1차 자식들이 두번째에 쫙. 삼차짜식들이 그다음쫘악. */

	/* iDepth? : Ready_HierarchyNodes함수를 재귀형태로 부르고 있기 때문에 한쪽(깊이)으로 생성해나가기 때문에. */
	/* 각 노드마다 깊이값(몇차자식이냐?) 을 저장해두고 나중에 정렬한다. */

	CHierarchyNode*		pHierarchyNode = CHierarchyNode::Create(pNode, pParent, iDepth++);

	if (nullptr == pHierarchyNode)
		return E_FAIL;

	m_HierarchyNodes.push_back(pHierarchyNode);

	for (_uint i = 0; i < pNode->mNumChildren; ++i)
		Ready_HierarchyNodes(pNode->mChildren[i], pHierarchyNode, iDepth);

	return S_OK;
}

HRESULT CModel::Ready_Animations()
{
	/* 애니메이션의 정보를 읽어서 저장한다.  */
	/* 애니메이션 정보 : 애니메이션이 재생되는데 걸리는 총 시간(Duration),  애니메이션의 재생속도(mTickPerSecond), 몇개의 채널(mNumChannels) 에 영향르 주는가. 각채널의 정보(aiNodeAnim)(mChannels) */
	/* mChannel(aiNodeAnim, 애니메이션이 움직이는 뼈) 에 대한 정보를 구성하여 객체화한다.(CChannel) */
	/* 채널 : 뼈. 이 뼈는 한 애니메이션 안에서 사용된다. 그 애니메이션 안에서 어떤 시간, 시간, 시간, 시간대에 어떤 상태를 표현하면 되는지에 대한 정보(keyframe)들을 다므낟. */
	/* keyframe : 어떤시간?, 상태(vScale, vRotation, vPosition) */

	m_iNumAnimations = m_pAIScene->mNumAnimations;

	/* 모델이 들고 있는 애니메이션 갯수만큼 애니메이션을 생성한다. */
	for (_uint i = 0; i < m_pAIScene->mNumAnimations; ++i)
	{
		aiAnimation*		pAIAnimation = m_pAIScene->mAnimations[i];

		/*I 애니메이션 마다 객체화 하는 이유 : 현재 재생 시간에 맞는 채널들의 뼈 상태를 셋팅한다. (조난 빡세다) 함수로 만들어야지뭐. */
		CAnimation*			pAnimation = CAnimation::Create(pAIAnimation);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);
	}	
	return S_OK;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, TYPE eType, const char * pModelFilePath, const char * pModelFileName, _fmatrix PivotMatrix)
{
	CModel*			pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType, pModelFilePath, pModelFileName, PivotMatrix)))
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

	for (auto& pHierarchyNode : m_HierarchyNodes)
		Safe_Release(pHierarchyNode);

	m_HierarchyNodes.clear();

	for (auto& Material : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_Release(Material.pTexture[i]);
	}
	m_Materials.clear();

	for (auto& pMeshContainer : m_Meshes)
		Safe_Release(pMeshContainer);

	m_Meshes.clear();

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);

	m_Animations.clear();

	m_Importer.FreeScene();
}
