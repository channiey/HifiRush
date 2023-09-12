#include "..\Public\Shader.h"

CShader::CShader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CShader::CShader(const CShader & rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
	, m_InputLayouts(rhs.m_InputLayouts)
{
	for (auto& pInputLayout : m_InputLayouts)
		Safe_AddRef(pInputLayout);

	Safe_AddRef(m_pEffect);
}

HRESULT CShader::Initialize_Prototype(const wstring & strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	_uint		iHlslFlag = 0;

#ifdef _DEBUG
	iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;

#endif	

	/* 해당 경로에 셰이더 파일을 컴파일하여 객체화한다. */
	if (FAILED(D3DX11CompileEffectFromFile(strShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHlslFlag, 0, m_pDevice, &m_pEffect, nullptr)))
		return E_FAIL;

	/* 셰이더 안에 있는 모든 패스가 내가 그리려고 하는 정점을 잘 받아줄 수 있는지 확인한다*/
	/* 이게 확인 되면 ID3D11InputLayout을 생성한다.* /
	/* 결론적으로 ID3D11InputLayout을 생성해주기위해 밑에 일을 한다 .*/

	/* 패스를 얻기위한 작업을 선행한다. 패스는 테크니커 안에 선언되어있다. */
	/* 이 셰이더 안에 정의된 테크니커의 정보를 얻어온다. (나는 테크니커를 하나만 정의했기때문에 0번째 인덱스로 받아온거다. )*/
	ID3DX11EffectTechnique*			pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
		return E_FAIL;
	
	/* 테크니커의 정보를 얻어온다. */
	D3DX11_TECHNIQUE_DESC			TechniqueDesc;	
	pTechnique->GetDesc(&TechniqueDesc);

	/* TechniqueDesc.Passes : 패스의 갯수. */	
	for (_uint i = 0; i < TechniqueDesc.Passes; i++)
	{
		/* pTechnique->GetPassByIndex(i) : i번째 패스객체를 얻어온다. */
		ID3DX11EffectPass*		pPass = pTechnique->GetPassByIndex(i);


		/* 패스의 정보를 얻어온다. */
		D3DX11_PASS_DESC		PassDesc;
		pPass->GetDesc(&PassDesc);

		ID3D11InputLayout*			pInputLayout = { nullptr };

		/* 패스안에 선언된 정점정보와 내가 던져준 정점정보가 일치한다면 ID3D11InputLayout를 생성해준다. */
		if (FAILED(m_pDevice->CreateInputLayout(pElements, iNumElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout)))
			return E_FAIL;

		m_InputLayouts.push_back(pInputLayout);
	}

	return S_OK;
}

HRESULT CShader::Initialize(void * pArg)
{
	return S_OK;
}


HRESULT CShader::Bind_Matrix(const char* pConstantName, const _float4x4* pMatrix) const
{
	/* pConstantName이름에 해당하는 타입을 고려하지않은 전역변수를 컨트롤하는 객체를 얻어온다 .*/

	/* 셰이더로부터 상수테이블에 존재하는 전역 변수를 얻어오기 위한 과정 */

	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable*		pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	return pMatrixVariable->SetMatrix((const _float*)pMatrix);
}

HRESULT CShader::Bind_Matrices(const char * pConstantName, const _float4x4* pMatrices, _uint iNumMatrices)const 
{
	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;	

	ID3DX11EffectMatrixVariable*		pMatrix = pVariable->AsMatrix();
	if (nullptr == pMatrix)
		return E_FAIL;

	return pMatrix->SetMatrixArray((_float*)pMatrices, 0, iNumMatrices);
}

HRESULT CShader::Bind_Texture(const char * pConstantName, ID3D11ShaderResourceView* pSRV) const
{
	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable*	pSRVariable = pVariable->AsShaderResource();
	if (nullptr == pSRVariable)
		return E_FAIL;

	return pSRVariable->SetResource(pSRV);	
}

HRESULT CShader::Bind_Textures(const char * pConstantName, ID3D11ShaderResourceView** ppSRVs, _uint iNumTextures) const
{
	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable*	pSRVariable = pVariable->AsShaderResource();
	if (nullptr == pSRVariable)
		return E_FAIL;

	return pSRVariable->SetResourceArray(ppSRVs, 0, iNumTextures);	
}

HRESULT CShader::Begin(_uint iPassIndex)
{
	if (iPassIndex >= m_InputLayouts.size())
		return E_FAIL;

	m_pContext->IASetInputLayout(m_InputLayouts[iPassIndex]);

	ID3DX11EffectTechnique*	pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
		return E_FAIL;

	/* iPassIndex번째 패스로 그리기를 시작합니다. */
	pTechnique->GetPassByIndex(iPassIndex)->Apply(0, m_pContext);	

	return S_OK;
}

CShader * CShader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring & strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	CShader*	pInstance = new CShader(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(strShaderFilePath, pElements, iNumElements)))
	{
		MSG_BOX("Failed to Created : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CShader::Clone(void * pArg)
{
	CShader*	pInstance = new CShader(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CShader::Free()
{
	__super::Free();

	for (auto& pInputLayout : m_InputLayouts)
		Safe_Release(pInputLayout);

	Safe_Release(m_pEffect);
}
