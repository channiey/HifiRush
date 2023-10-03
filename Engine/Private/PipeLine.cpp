#include "..\Public\PipeLine.h"

#include "Shader.h"

IMPLEMENT_SINGLETON(CPipeLine)

CPipeLine::CPipeLine()
{
}

HRESULT CPipeLine::Initialize()
{
	for (size_t i = 0; i < STATE_END; i++)
	{
		m_TransformMatrices[i] = Matrix::Identity;
		m_TransformMatrices_Inverse[i] = Matrix::Identity;
	}

	return S_OK;
}

void CPipeLine::Tick()
{
	/* ��, ���� ������� ����Ѵ�. */
	for (size_t i = 0; i < STATE_END; i++)
		m_TransformMatrices_Inverse[i] = m_TransformMatrices[i].Invert();

	/* �� �����̽� ��ȯ ����� ī�޶� ���� ��ȯ ������̹Ƿ� �佺���̽� ��ȯ ����� ������� ī�޶� ���� ��ȯ ��� */
	memcpy(&m_vCamPosition, &m_TransformMatrices_Inverse[STATE_VIEW].m[3][0], sizeof(Vec4));	
}

void CPipeLine::Set_Transform(const CPipeLine::TRANSFORM_STATE& eState, Matrix TransformMatrix)
{
	if (eState >= STATE_END)
		return;
	m_TransformMatrices[eState] = TransformMatrix;
}

Matrix CPipeLine::Get_Transform(const CPipeLine::TRANSFORM_STATE& eState) const
{
	if (eState >= STATE_END)
		return Matrix();

	return m_TransformMatrices[eState];	
}

Matrix CPipeLine::Get_Transform_Inverse(const CPipeLine::TRANSFORM_STATE& eState) const
{
	if (eState >= STATE_END)
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
