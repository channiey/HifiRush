#include "..\Public\PipeLine.h"

#include "Shader.h"

IMPLEMENT_SINGLETON(CPipeLine)

CPipeLine::CPipeLine()
{
}

HRESULT CPipeLine::Initialize()
{
	for (size_t i = 0; i < D3DTS_END; i++)
	{
		m_TransformMatrices[i] = Matrix::Identity;
		m_TransformMatrices_Inverse[i] = Matrix::Identity;
	}

	return S_OK;
}

void CPipeLine::Tick()
{
	for (size_t i = 0; i < D3DTS_END; i++)
		m_TransformMatrices_Inverse[i] = m_TransformMatrices[i].Invert();
		//XMStoreFloat4x4(&m_TransformMatrices_Inverse[i], XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_TransformMatrices[i])));

	memcpy(&m_vCamPosition, &m_TransformMatrices_Inverse[D3DTS_VIEW].m[3][0], sizeof(Vec4));	
}

void CPipeLine::Set_Transform(const CPipeLine::TRANSFORM_STATE& eState, Matrix TransformMatrix)
{
	if (eState >= D3DTS_END)
		return;
	m_TransformMatrices[eState] = TransformMatrix;
}

Matrix CPipeLine::Get_Transform(const CPipeLine::TRANSFORM_STATE& eState) const
{
	if (eState >= D3DTS_END)
		return Matrix();

	return m_TransformMatrices[eState];	
}

Matrix CPipeLine::Get_Transform_Inverse(const CPipeLine::TRANSFORM_STATE& eState) const
{
	if (eState >= D3DTS_END)
		return Matrix();

	return m_TransformMatrices_Inverse[eState];
}

HRESULT CPipeLine::Bind_TransformToShader(CShader * pShader, const char * pConstantName, CPipeLine::TRANSFORM_STATE eState)
{	
	return pShader->Bind_Matrix(pConstantName, &m_TransformMatrices[eState]);	
}

void CPipeLine::Free()
{
	__super::Free();
}
