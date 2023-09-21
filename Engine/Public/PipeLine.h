#pragma once

#include "Base.h"

/* Tick �Լ����� ��� ������ �����, ������� ���ϰ� ��������� �����Ѵ�. */

BEGIN(Engine)

class CPipeLine final : public CBase
{
	DECLARE_SINGLETON(CPipeLine)

public:
	enum TRANSFORM_STATE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };

private:
	CPipeLine();
	virtual ~CPipeLine() = default;

public:
	HRESULT Initialize();
	void	Tick();

public:
	Matrix	Get_Transform(const CPipeLine::TRANSFORM_STATE& eState) const;
	Matrix	Get_Transform_Inverse(const CPipeLine::TRANSFORM_STATE& eState) const;

	/* ī�޶��� ���� �������� �޾ƿ´�. */
	Vec4	Get_CamPosition() const { return m_vCamPosition; }

public:
	void	Set_Transform(const CPipeLine::TRANSFORM_STATE& eState, Matrix TransformMatrix);
	
public:
	/* ���̴��� �����, ��������� �����Ѵ�. */
	HRESULT Bind_TransformToShader(class CShader* pShader, const char* pConstantName, CPipeLine::TRANSFORM_STATE eState);	


private:
	/* ��, ���� ����� �����Ѵ�. */
	Matrix	m_TransformMatrices[D3DTS_END];

	/* ��, ���� ������� �����Ѵ�. */
	Matrix	m_TransformMatrices_Inverse[D3DTS_END];

	/* ī�޶� ���� ������ */
	Vec4	m_vCamPosition = { 0.0f, 0.f, 0.f, 1.f };

public:
	virtual void Free() override;
};

END